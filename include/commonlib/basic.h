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

#ifndef COMMONLIB_BASIC_H
#define COMMONLIB_BASIC_H

#include <string.h>
#include <stdlib.h>

#define UNUSED(x) ((void) x)

static inline char * dup(const char * xs) {
    size_t size = strlen(xs) + 1;

    char * ys = malloc(size);
    return ys == NULL ? NULL : memcpy(ys, xs, size);
}

#define min(x, y) (((x) < (y)) ? (x) : (y))
#define max(x, y) (((x) > (y)) ? (x) : (y))

#endif
