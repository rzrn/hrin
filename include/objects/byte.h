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

#ifndef BYTE_H
#define BYTE_H

#include <expr.h>

typedef struct { Expr _expr; char value; } ExprByte;

#define BYTE(o) ((ExprByte *) (o))

extern ExprTag exprByteTag;
void initByteTag(Region *);

static inline void * newByte(Region * region, char value) {
    ExprByte * retval = newExpr(region, &exprByteTag); IFNRET(retval);

    retval->value = value;

    return retval;
}

#endif
