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

#ifndef COMMONLIB_TRIE_H
#define COMMONLIB_TRIE_H

typedef struct {
    void * next[2];
    void * pred;
    void * succ;
} BinaryTreeNode;

typedef struct {
    BinaryTreeNode node;
    void * value;
} BinaryTreeValue;

typedef struct {
    BinaryTreeValue * root;
    BinaryTreeNode  * head1;
    BinaryTreeValue * head2;
} Trie;

Trie newTrie(void);
void * findTrie(Trie *, const char *);
void setTrie(Trie *, const char *, void *);
void freeTrie(Trie *);
void copyTrie(Trie * dest, const Trie * src);

#endif
