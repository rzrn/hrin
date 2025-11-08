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

#ifndef ATOM_H
#define ATOM_H

#include <expr.h>

typedef struct { Expr _expr; char * value; } ExprAtom;

#define ATOM(o) ((ExprAtom *) (o))

extern ExprTag exprAtomTag;
void initAtomTag(Region *);

static inline void * newAtom(Region * region, char * buf) {
    ExprAtom * retval = newExpr(region, &exprAtomTag); IFNRET(retval);

    retval->value = buf;

    return retval;
}

#endif
