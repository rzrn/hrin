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

#ifndef ERROR_H
#define ERROR_H

typedef char * ErrorTag;

void * throw(ErrorTag tag, const char * format, ...);

const char * getErrorBuffer(void);
ErrorTag getThrownError(void);

extern ErrorTag EOFErrorTag, SyntaxErrorTag, ApplyErrorTag, UnknownErrorTag, NameErrorTag, OOMErrorTag, TypeErrorTag, RegionErrorTag;

#define ARITY(expected, given) do { if ((expected) != (given)) return throw(TypeErrorTag, "expected %zu argument(s) but %zu were given", (expected), (given)); } while (false)

#endif
