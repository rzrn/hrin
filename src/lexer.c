/*
    Copyright © 2024–2025 rzrn

    This file is part of Hrin.

    Hrin is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Hrin is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Hrin.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <ctype.h>

#include <io.h>
#include <lexer.h>

static int lookahead(int recv) {
    if (isspace(recv)) return TokenError;

    switch (recv) {
        case EOF: return TokenEof;
        case '(': return TokenLparen;
        case ')': return TokenRparen;
        case '[': return TokenLbracket;
        case ']': return TokenRbracket;
        case ';': return TokenSemicolon;
        case ',': return TokenComma;
        case '"': return TokenLiteral;
        default:  return -1;
    }
}

int lexerNextToken(FILE * stream, char * outbuf[]) {
    int recv; do recv = fgetc(stream); while (isspace(recv));

    int token = lookahead(recv);

    if (token < 0) {
        fpush(recv);

        while (lookahead(recv) < 0)
            recv = fnext(stream);

        ungetc(recv, stream);

        *outbuf = fdup(); return TokenIdent;
    } else if (token == TokenLiteral) {
        for (;;) switch (fnext(stream)) {
            case EOF: fdrop(); return TokenError;
            case '"': *outbuf = fdup(); return TokenLiteral;
        }
    } else return token;
}

static int bufferedToken = -1;
static char * lexerBuffer = NULL;

char * getLexerBuffer(void)
{ return lexerBuffer; }

void pushToken(int token) {
    if (bufferedToken < 0) bufferedToken = token;
}

int popToken(FILE * stream) {
    if (bufferedToken < 0) return lexerNextToken(stream, &lexerBuffer);

    int retval = bufferedToken; bufferedToken = -1; return retval;
}
