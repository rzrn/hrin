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

#include <stdio.h>
#include <ctype.h>

#include <common.h>

#include <objects/byte.h>

static size_t showByte(char * buf, size_t size, void * o) {
    char c = BYTE(o)->value;

    if (isprint(c)) return snprintf(buf, size, "'%c'", c);

    if (size <= 6) return ellipsis(buf);
    return snprintf(buf, size, "'\\x%02hhX'", c);
}

static bool equalByte(void * o1, void * o2) {
    return BYTE(o1)->value == BYTE(o2)->value;
}

ExprTag exprByteTag = {
    .eval   = trivEval,
    .apply  = applyThrowError,
    .show   = showByte,
    .delete = trivDelete,
    .move   = trivMove,
    .equal  = equalByte,
    .size   = sizeof(ExprByte)
};

void initByteTag(Region * region) {
    UNUSED(region);

    newExprImmortal(&exprTag, &exprByteTag, NULL);
    setVar(region->scope, "byte", &exprByteTag);
}
