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

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include <commonlib/trie.h>

#define NEXT(n, b) do { n = n->next[b & 1]; if (n == NULL) return NULL; } while (false)

static inline BinaryTreeValue * nextValueTree(BinaryTreeValue * n, uint8_t k) {
    BinaryTreeNode * curr = &n->node;

    NEXT(curr, k >> 7);
    NEXT(curr, k >> 6);
    NEXT(curr, k >> 5);
    NEXT(curr, k >> 4);
    NEXT(curr, k >> 3);
    NEXT(curr, k >> 2);
    NEXT(curr, k >> 1);

    return curr->next[k & 1];
}

static void * findBinaryTree(BinaryTreeValue * n, const char * key) {
    for (;;) {
        if (n == NULL) return NULL;
        if (*key == '\0') return n->value;

        n = nextValueTree(n, *(key++));
    }
}

static inline BinaryTreeNode * newBinaryTreeNode(Trie * T) {
    BinaryTreeNode * const nsucc = T->head1;
    BinaryTreeNode * n = malloc(sizeof(BinaryTreeNode));

    T->head1   = n;
    n->pred    = NULL;
    n->succ    = nsucc;
    n->next[0] = NULL;
    n->next[1] = NULL;

    return n;
}

static inline BinaryTreeValue * newBinaryTreeValue(Trie * T) {
    BinaryTreeValue * const nvsucc = T->head2;
    BinaryTreeValue * nv = malloc(sizeof(BinaryTreeValue));

    BinaryTreeNode * const n = &nv->node;

    T->head2   = nv;
    n->pred    = NULL;
    n->succ    = nvsucc;
    n->next[0] = NULL;
    n->next[1] = NULL;
    nv->value  = NULL;

    return nv;
}

#define NEW(T, n, b) do { if (n->next[b & 1] == NULL) n->next[b & 1] = newBinaryTreeNode(T); n = n->next[b & 1]; } while (false)

static inline BinaryTreeValue * newValueTree(Trie * T, BinaryTreeValue * nv, uint8_t k) {
    BinaryTreeNode * n = &nv->node;

    NEW(T, n, k >> 7);
    NEW(T, n, k >> 6);
    NEW(T, n, k >> 5);
    NEW(T, n, k >> 4);
    NEW(T, n, k >> 3);
    NEW(T, n, k >> 2);
    NEW(T, n, k >> 1);

    if (n->next[k & 1] == NULL) n->next[k & 1] = newBinaryTreeValue(T);

    return n->next[k & 1];
}

static void setBinaryTree(Trie * T, BinaryTreeValue * n, const char * key, void * value) {
    while (*key != '\0') n = newValueTree(T, n, *(key++));

    n->value = value;
}

static void * copyTree(Trie * T, void * destAddr, const void * srcAddr, int nbit) {
    if (destAddr == NULL) {
        if (nbit == 0)
            destAddr = newBinaryTreeValue(T);
        else
            destAddr = newBinaryTreeNode(T);
    }

    if (nbit == 0) {
        BinaryTreeValue * dest = destAddr;
        const BinaryTreeValue * src = srcAddr;

        if (dest->value == NULL) dest->value = src->value;
        nbit = 8;
    }

    BinaryTreeNode * dest = destAddr;
    const BinaryTreeNode * src = srcAddr;

    if (src->next[0] != NULL) dest->next[0] = copyTree(T, dest->next[0], src->next[0], nbit - 1);
    if (src->next[1] != NULL) dest->next[1] = copyTree(T, dest->next[1], src->next[1], nbit - 1);

    return destAddr;
}

Trie newTrie(void) {
    return (Trie) {.root = NULL};
}

void * findTrie(Trie * T, const char * key) {
    return findBinaryTree(T->root, key);
}

void setTrie(Trie * T, const char * key, void * value) {
    if (T->root == NULL) T->root = newBinaryTreeValue(T);

    setBinaryTree(T, T->root, key, value);
}

void freeTrie(Trie * T) {
    for (BinaryTreeNode * n = T->head1; n != NULL;)
    { BinaryTreeNode * nsucc = n->succ; free(n); n = nsucc; }

    for (BinaryTreeNode * n = &T->head2->node; n != NULL;)
    { BinaryTreeNode * nsucc = n->succ; free(n); n = nsucc; }
}

void copyTrie(Trie * dest, const Trie * src) {
    if (src->root == NULL) return;

    dest->root = copyTree(dest, dest->root, src->root, 0);
}
