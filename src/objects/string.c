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

#include <objects/integer.h>
#include <objects/string.h>
#include <objects/byte.h>

static void * applyString(Region * region, void * o, Array * xs) {
    ARITY(1, xs->size);

    ExprInteger * i = evalEnsureInteger(region, getArray(xs, 0)); IFNRET(i);
    return newByte(region, STRING(o)->value[i->value]); // TODO: bounds check
}

static size_t showString(char * buf, size_t size, void * value) {
    ExprString * expr = value;

    size_t length = strlen(expr->value);

    if (size <= length + 2) return ellipsis(buf);
    else return snprintf(buf, size, "\"%s\"", expr->value);
}

static void deleteString(void * value) {
    ExprString * expr = value;
    free(expr->value);
}

static bool equalString(void * value1, void * value2) {
    ExprString * expr1 = value1, * expr2 = value2;
    return strcmp(expr1->value, expr2->value) == 0;
}

ExprTag exprStringTag = {
    .eval   = trivEval,
    .apply  = applyString,
    .show   = showString,
    .delete = deleteString,
    .move   = trivMove,
    .equal  = equalString,
    .size   = sizeof(ExprString)
};

void initStringTag(Region * region) {
    UNUSED(region);

    newExprImmortal(&exprTag, &exprStringTag, NULL);
    setVar(region->scope, "string", &exprStringTag);
}
