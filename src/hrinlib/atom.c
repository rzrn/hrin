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

#include <commonlib/basic.h>

#include <hrinlib/atom.h>

static void * evalAtom(Region * region, void * value) {
    ExprAtom * expr = value;

    Expr * o = getVar(region->rho, expr->value);
    return o == NULL ? throw(NameErrorTag, "%s is undefined", expr->value) : o;
}

static size_t showAtom(char * buf, size_t size, void * value) {
    ExprAtom * expr = value;

    size_t length = strlen(expr->value);
    if (size <= length) return ellipsis(buf);

    strcpy(buf, expr->value); return length;
}

static void deleteAtom(void * value) {
    ExprAtom * expr = value;
    free(expr->value);
}

static bool equalAtom(void * value1, void * value2) {
    ExprAtom * expr1 = value1, * expr2 = value2;
    return strcmp(expr1->value, expr2->value) == 0;
}

ExprTag exprAtomTag = {
    .eval   = evalAtom,
    .apply  = applyThrowError,
    .show   = showAtom,
    .delete = deleteAtom,
    .move   = trivMove,
    .equal  = equalAtom,
    .size   = sizeof(ExprAtom)
};

void initAtomTag(Region * region) {
    UNUSED(region);

    newExprImmortal(&exprTag, &exprAtomTag, NULL);
    setVar(region->rho, "atom", &exprAtomTag);
}
