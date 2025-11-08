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

#include <stdlib.h>
#include <io.h>

static size_t capacity = 0, length = 0;
static char * outbuf = NULL;

void fpush(char value) {
    if (capacity <= length) {
        capacity += 512;
        outbuf = realloc(outbuf, capacity);
    }

    outbuf[length++] = value;
}

int fnext(FILE * stream) {
    int recv = fgetc(stream);
    fpush(recv);
    return recv;
}

char * fdup(void) {
    char * retbuf = realloc(outbuf, length);
    retbuf[length - 1] = '\0';

    capacity = length = 0; outbuf = NULL;
    return retbuf;
}

void fdrop(void) {
    capacity = length = 0;
    free(outbuf); outbuf = NULL;
}
