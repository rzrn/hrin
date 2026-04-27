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

#include <stdio.h>
#include <stdlib.h>

#include <hrin/file.h>

struct _File {
    FILE * fd;
    size_t size;
    size_t length;
    char * buffer;
    int row, col;
    const char * name;
};

int fileRowNo(File * file) {
    return file->row;
}

int fileColNo(File * file) {
    return file->col;
}

const char * fileName(File * file) {
    return file->name;
}

int fileTakeChar(File * file) {
    int recv = fgetc(file->fd);

    if (recv == '\n') {
        file->col = 1;
        file->row++;
    } else {
        file->col++;
    }

    return recv;
}

void fileGiveChar(File * file, int recv) {
    if (recv == '\n') {
        file->col = 1;
        file->row--;
    } else {
        file->col--;
    }

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
    int recv = fileTakeChar(file);
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

File * fileStandardInput(void) {
    File * file = malloc(sizeof(File));
    if (file == NULL) return NULL;

    file->fd     = stdin;
    file->size   = 0;
    file->length = 0;
    file->buffer = NULL;
    file->row    = 1;
    file->col    = 1;
    file->name   = "<stdin>";

    return file;
}

File * fileReadOnly(const char * filepath) {
    FILE * fd = fopen(filepath, "r");
    if (fd == NULL) return NULL;

    File * file = malloc(sizeof(File));
    if (file == NULL) return NULL;

    file->fd     = fd;
    file->size   = 0;
    file->length = 0;
    file->buffer = NULL;
    file->row    = 1;
    file->col    = 1;
    file->name   = filepath;

    return file;
}

void fileClose(File * file) {
    fileDropBuffer(file);
    fclose(file->fd);
    free(file);
}
