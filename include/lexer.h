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

#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

enum {
    TokenEof,
    TokenError,
    TokenLparen,
    TokenRparen,
    TokenLbracket,
    TokenRbracket,
    TokenSemicolon,
    TokenComma,
    TokenIdent,
    TokenLiteral
};

char * getLexerBuffer(void);

void pushToken(int token);
int popToken(FILE *);

#endif
