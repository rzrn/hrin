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

#include <stdarg.h>
#include <stdio.h>

#include <error.h>

static char errorMessageBuffer[2048] = {0};
static ErrorTag thrownError;

void * throw(ErrorTag tag, const char * format, ...) {
    thrownError = tag;

    if (format == NULL) {
        errorMessageBuffer[0] = '\0';
    } else {
        va_list argv;

        va_start(argv, format);
        vsnprintf(errorMessageBuffer, sizeof(errorMessageBuffer), format, argv);
        va_end(argv);
    }

    return NULL;
}

const char * getErrorBuffer(void) {
    return errorMessageBuffer;
}

ErrorTag getThrownError(void) {
    ErrorTag retval = thrownError;

    thrownError = NULL;
    return retval;
}

ErrorTag EOFErrorTag     = "EOFError";
ErrorTag SyntaxErrorTag  = "SyntaxError";
ErrorTag ApplyErrorTag   = "ApplyError";
ErrorTag UnknownErrorTag = "UnknownError";
ErrorTag NameErrorTag    = "NameError";
ErrorTag OOMErrorTag     = "OOMError";
ErrorTag TypeErrorTag    = "TypeError";
ErrorTag RegionErrorTag  = "RegionError";
