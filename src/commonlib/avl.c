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

#include <stddef.h>
#include <stdlib.h>

#include <commonlib/basic.h>
#include <commonlib/avl.h>

static inline int height(TreeNode * n)
{ return n == NULL ? 0 : n->height; }

static inline void updateHeightTreeNode(TreeNode * n)
{ n->height = max(height(n->left), height(n->right)) + 1; }

static TreeNode * rotrr(TreeNode * n) {
    TreeNode * nl  = n->left;
    TreeNode * nlr = nl->right;

    nl->right = n;
    n->left   = nlr;

    updateHeightTreeNode(nl);
    updateHeightTreeNode(n);

    return nl;
}

static TreeNode * rotll(TreeNode * n) {
    TreeNode * nr  = n->right;
    TreeNode * nrl = nr->left;

    nr->left = n;
    n->right = nrl;

    updateHeightTreeNode(nr);
    updateHeightTreeNode(n);

    return nr;
}

static inline TreeNode * rotlr(TreeNode * n) {
    n->left = rotll(n->left);
    return rotrr(n);
}

static inline TreeNode * rotrl(TreeNode * n) {
    n->right = rotrr(n->right);
    return rotll(n);
}

static inline int bf(TreeNode * n)
{ return n == NULL ? 0 : height(n->left) - height(n->right); }

static inline TreeNode * rebalance(TreeNode * n) {
    if (n == NULL) return NULL;
    updateHeightTreeNode(n);

    int balance = bf(n);

    if (balance > 1) return bf(n->left) > 0 ? rotrr(n) : rotlr(n);

    if (balance < -1) return bf(n->right) > 0 ? rotrl(n) : rotll(n);

    return n;
}

static inline TreeNode * newTreeNode(AVLTree * T, void * value) {
    TreeNode * const nsucc = T->head;
    TreeNode * n = malloc(sizeof(TreeNode));

    T->head   = n;
    n->pred   = NULL;
    n->succ   = nsucc;
    n->left   = NULL;
    n->right  = NULL;
    n->value  = value;
    n->height = 1;

    if (nsucc != NULL)
        nsucc->pred = n;

    return n;
}

static TreeNode * insertTreeNode(AVLTree * T, TreeNode * n, void * value) {
    if (n == NULL) return newTreeNode(T, value);

    if (value < n->value)
        n->left = insertTreeNode(T, n->left, value);
    else if (value > n->value)
        n->right = insertTreeNode(T, n->right, value);
    else
        return n;

    return rebalance(n);
}

static inline TreeNode * minTreeNode(TreeNode * n) {
    while (n->left != NULL)
        n = n->left;

    return n;
}

static inline void linkTreeNode(TreeNode * n1, TreeNode * n2) {
    if (n1 != NULL) n1->succ = n2;
    if (n2 != NULL) n2->pred = n1;
}

static TreeNode * deleteTreeNode(AVLTree * T, TreeNode * n, void * value) {
    if (n == NULL) return NULL;

    if (value < n->value)
        n->left = deleteTreeNode(T, n->left, value);
    else if (value > n->value)
        n->right = deleteTreeNode(T, n->right, value);
    else if (n->left == NULL) {
        TreeNode * nr = n->right;

        linkTreeNode(n->pred, n->succ);
        if (T->head == n) T->head = n->succ;

        free(n); return nr;
    } else if (n->right == NULL) {
        TreeNode * nl = n->left;

        linkTreeNode(n->pred, n->succ);
        if (T->head == n) T->head = n->succ;

        free(n); return nl;
    } else {
        TreeNode * nsucc = minTreeNode(n->right);

        n->value = nsucc->value;
        n->right = deleteTreeNode(T, n->right, nsucc->value);
    }

    return rebalance(n);
}

static bool findTreeNode(TreeNode * n, void * value) {
    while (n != NULL) {
        if (value < n->value)
            n = n->left;
        else if (value > n->value)
            n = n->right;
        else
            return true;
    }

    return false;
}

AVLTree newAVLTree(void) {
    return (AVLTree) {.root = NULL, .head = NULL};
}

bool findAVLTree(AVLTree * T, void * value) {
    return findTreeNode(T->root, value);
}

void insertAVLTree(AVLTree * T, void * value) {
    T->root = insertTreeNode(T, T->root, value);
}

void deleteAVLTree(AVLTree * T, void * value) {
    T->root = deleteTreeNode(T, T->root, value);
}
