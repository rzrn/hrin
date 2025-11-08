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

#include <common.h>

#include <objects/extern.h>

static void * applyExtern(Region * region, void * value, Array * xs) {
    ExprExtern * expr = value;
    return expr->value(region, xs);
}

static size_t showExtern(char * buf, size_t size, void * value) {
    if (size <= 26) return ellipsis(buf);

    ExprExtern * expr = value;
    return snprintf(buf, size, "<#NATIVE %016lx>", (uintptr_t) expr->value);
}

ExprTag exprExternTag = {
    .eval   = trivEval,
    .apply  = applyExtern,
    .show   = showExtern,
    .delete = trivDelete,
    .move   = trivMove,
    .equal  = equalByRef,
    .size   = sizeof(ExprExtern)
};

void initExternTag(Region * region) {
    UNUSED(region);

    newExprImmortal(&exprTag, &exprExternTag, NULL);
}
