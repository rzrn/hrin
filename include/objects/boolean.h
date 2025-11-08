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

#ifndef BOOLEAN_H
#define BOOLEAN_H

#include <expr.h>

typedef Expr ExprBoolean;

extern ExprBoolean exprTrue, exprFalse;

extern ExprTag exprBooleanTag;
void initBooleanTag(Region *);

static inline void * newBool(bool value)
{ return value ? &exprTrue : &exprFalse; }

#endif
