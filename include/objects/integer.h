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

#ifndef INTEGER_H
#define INTEGER_H

#include <limits.h>

#include <expr.h>

typedef long long int Integer;

#define INTEGER_MIN LLONG_MIN
#define INTEGER_MAX LLONG_MAX

typedef struct { Expr _expr; Integer value; } ExprInteger;

#define INTEGER(o) ((ExprInteger *) (o))

extern ExprTag exprIntegerTag;
void initIntegerTag(Region *);

static inline void * newInteger(Region * region, Integer value) {
    ExprInteger * retval = newExpr(region, &exprIntegerTag);
    if (retval == NULL) return NULL;

    retval->value = value;

    return retval;
}

static inline void * evalEnsureInteger(Region * region, void * value) {
    void * o = eval(region, value); IFNRET(o);

    if (tagof(o) != &exprIntegerTag) return throw(
        TypeErrorTag, "%s expected to be an integer", showExpr(o)
    );

    return o;
}

#endif
