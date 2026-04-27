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

#ifndef HRIN_FILE_H
#define HRIN_FILE_H

typedef struct _File File;

int fileRowNo(File *);
int fileColNo(File *);
const char * fileName(File *);

int fileTakeChar(File *);
void fileGiveChar(File *, int);

void fileSaveChar(File *, char);
int fileNextChar(File *);

char * fileTakeBuffer(File *);
void fileDropBuffer(File *);

File * fileStandardInput(void);
File * fileReadOnly(const char *);
void fileClose(File *);

#endif
