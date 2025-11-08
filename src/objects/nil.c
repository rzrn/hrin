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

#include <common.h>

#include <objects/nil.h>

static size_t showNil(char * buf, size_t size, void * value) {
    UNUSED(value); UNUSED(size);
    strcpy(buf, "nil"); return 3;
}

ExprTag exprNilTag = {
    .eval   = trivEval,
    .apply  = applyThrowError,
    .show   = showNil,
    .delete = trivDelete,
    .move   = trivMove,
    .equal  = equalByRef,
    .size   = 0
};

ExprNil exprNil;

void initNilTag(Region * region) {
    newExprImmortal(&exprTag, &exprNilTag, NULL);
    newExprImmortal(&exprNilTag, &exprNil, NULL);

    setVar(region->scope, "nil", &exprNil);
}
