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

#include <stdlib.h>

#include <hrin/file.h>

int fileTakeChar(File * file) {
    return fgetc(file->fd);
}

void fileGiveChar(File * file, int recv) {
    ungetc(recv, file->fd);
}

void fileSaveChar(File * file, char ch) {
    if (file->size <= file->length) {
        file->size += 512;
        file->buffer = realloc(file->buffer, file->size);
    }

    file->buffer[file->length++] = ch;
}

int fileNextChar(File * file) {
    int recv = fgetc(file->fd);
    fileSaveChar(file, recv);
    return recv;
}

char * fileTakeBuffer(File * file) {
    char * retbuf = realloc(file->buffer, file->length);
    retbuf[file->length - 1] = '\0';

    file->size   = 0;
    file->length = 0;
    file->buffer = NULL;

    return retbuf;
}

void fileDropBuffer(File * file) {
    free(file->buffer);

    file->size   = 0;
    file->length = 0;
    file->buffer = NULL;
}

void fileStandardInput(File * file) {
    file->fd     = stdin;
    file->size   = 0;
    file->length = 0;
    file->buffer = NULL;
}

int fileReadOnly(const char * filepath, File * file) {
    FILE * fd = fopen(filepath, "r");
    if (fd == NULL) return -1;

    file->fd     = fd;
    file->size   = 0;
    file->length = 0;
    file->buffer = NULL;

    return 0;
}

void fileClose(File * file) {
    fileDropBuffer(file);
    fclose(file->fd);
}
