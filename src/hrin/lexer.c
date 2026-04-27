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

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include <hrin/lexer.h>
#include <hrin/file.h>

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

static int lexerNextToken(File * file, char * outbuf[]) {
    int recv; do recv = fileTakeChar(file); while (isspace(recv));

    int token = lookahead(recv);

    if (token < 0) {
        fileSaveChar(file, recv);

        while (lookahead(recv) < 0)
            recv = fileNextChar(file);

        fileGiveChar(file, recv);

        *outbuf = fileTakeBuffer(file); return TokenIdent;
    } else if (token == TokenLiteral) {
        for (;;) switch (fileNextChar(file)) {
            case EOF: fileDropBuffer(file); return TokenError;
            case '"': *outbuf = fileTakeBuffer(file); return TokenLiteral;
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

int popToken(File * file) {
    if (bufferedToken < 0) return lexerNextToken(file, &lexerBuffer);

    int retval = bufferedToken; bufferedToken = -1; return retval;
}
