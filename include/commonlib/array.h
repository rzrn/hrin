/*
    Copyright © 2024–2025 rzrn

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

#ifndef COMMONLIB_ARRAY_H
#define COMMONLIB_ARRAY_H

#include <stdlib.h>

typedef struct {
    void ** data;
    size_t size;
} Array;

Array newArray(size_t);
void freeArray(Array *);

static inline Array * emptyArray(void)
{ return calloc(1, sizeof(Array)); }

static inline void extendArray(Array * A, size_t size) {
    if (A->size < size) {
        A->data = realloc(A->data, size * sizeof(void *));
        A->size = size;
    }
}

static inline Array sliceArray(Array * A, size_t i, size_t j)
{ return (Array) {.data = A->data == NULL ? NULL : A->data + i, .size = j - i}; }

static inline void * getArray(Array * A, size_t index)
{ return A->data[index]; }

static inline void setArray(Array * A, size_t index, void * value)
{ A->data[index] = value; }

#endif
