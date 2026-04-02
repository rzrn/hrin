/*
    Copyright © 2024–2026 rzrn

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stddef.h>
#include <ctype.h>

#include <commonlib/basic.h>

#include <hrinlib/error.h>
#include <hrinlib/atom.h>
#include <hrinlib/nil.h>
#include <hrinlib/cc.h>

#include <baselib/integer.h>
#include <baselib/string.h>

#include <hrin/parser.h>
#include <hrin/lexer.h>

static inline bool tokenLookahead(FILE * stream, int expected) {
    int token = popToken(stream);
    if (token == expected) return true;

    pushToken(token); return false;
}

void * takeExprSepBy1(Region * region, FILE * stream, void * headval, int toksep, int tokuntil) {
    ExprCC * retval = newCC(region, headval, &exprNil);

    if (tokenLookahead(stream, tokuntil)) {
        retval->cdr = &exprNil;
        return retval;
    }

    ExprCC * curr = retval;

    for (;;) {
        Expr * argval = takeExpr(region, stream);
        if (argval == NULL) return NULL;

        int separator = popToken(stream);

        if (separator == toksep) {
            ExprCC * next = newCC(region, argval, &exprNil);
            curr->cdr = next;
            curr = next;
        } else if (separator == tokuntil) {
            curr->cdr = newCC(region, argval, &exprNil);
            return retval;
        } else return throw(SyntaxErrorTag, "expected semicolon or right bracket");
    }
}

void * takeExpr(Region * region, FILE * stream) {
    for (;;) {
        int token = popToken(stream);

        if (token == TokenIdent) {
            char * outbuf = getLexerBuffer();

            void * headval = NULL;

            if (isdigit(outbuf[0]) || outbuf[0] == '-' || outbuf[0] == '+') {
                char * endptr; long long int num = strtoll(outbuf, &endptr, 10);
                if (endptr[0] == '\0') headval = newInteger(region, num);
                else throw(SyntaxErrorTag, "invalid integer: %s", outbuf);

                free(outbuf);
            } else headval = newAtom(region, outbuf);

            if (headval == NULL) return NULL;

            int nextToken = popToken(stream);

            if (nextToken == TokenLbracket)
                return takeExprSepBy1(region, stream, headval, TokenSemicolon, TokenRbracket);
            else {
                pushToken(nextToken);
                return headval;
            }
        }

        if (token == TokenLbracket) {
            void * headval = newAtom(region, dup("list"));
            return takeExprSepBy1(region, stream, headval, TokenSemicolon, TokenRbracket);
        }

        if (token == TokenLiteral) {
            ExprString * retval = newExpr(region, &exprStringTag);
            retval->value = getLexerBuffer();
            return retval;
        }

        if (token == TokenEof) return throw(EOFErrorTag, NULL);

        return throw(SyntaxErrorTag, "unexpected token");
    }
}

void * takeExprToplevel(Region * region, FILE * stream) {
    void * retval = takeExpr(region, stream);
    if (retval == NULL) return NULL;

    if (popToken(stream) == TokenSemicolon) return retval;

    for (;;) switch (popToken(stream)) {
        case TokenEof: case TokenSemicolon: return throw(SyntaxErrorTag, "expected semicolon");
        case TokenIdent: case TokenLiteral: free(getLexerBuffer());
    }
}
