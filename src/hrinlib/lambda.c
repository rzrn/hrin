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

#include <commonlib/basic.h>
#include <commonlib/array.h>
#include <commonlib/trie.h>

#include <hrinlib/lambda.h>
#include <hrinlib/extern.h>
#include <hrinlib/atom.h>

static void * applyMacro(Region * region, void * value, Array * xs) {
    ExprLexical * expr = value;

    ARITY(expr->vars.size, xs->size);

    Region * nested = newRegion(region); IFNRET(nested);
    nested->rho = newRho(expr->rho); // TODO

    void * retval = NULL;

    for (size_t i = 0; i < xs->size; i++)
        setVar(nested->rho, getArray(&expr->vars, i), getArray(xs, i));

    void * o = eval(nested, expr->value);
    if (o == NULL) goto finally;

    retval = eval(region, o);

    if (retval == NULL) goto finally;
    if (move(region, retval) == NULL) retval = NULL;

    finally: deleteRho(nested->rho); deleteRegion(nested);

    return retval;
}

static void * applyLambda(Region * region, void * value, Array * xs) {
    ExprLexical * expr = value;

    ARITY(expr->vars.size, xs->size);

    Region * nested = newRegion(region); IFNRET(nested);
    nested->rho = newRho(expr->rho); // TODO

    void * retval = NULL;

    for (size_t i = 0; i < xs->size; i++) {
        void * o = eval(region, getArray(xs, i));
        if (o == NULL) goto finally;

        setVar(nested->rho, getArray(&expr->vars, i), o);
    }

    retval = eval(nested, expr->value);

    if (retval == NULL) goto finally;
    if (move(region, retval) == NULL) retval = NULL;

    finally: deleteRho(nested->rho); deleteRegion(nested);

    return retval;
}

static size_t showLambda(char * buf, size_t size, void * value) {
    //ExprLexical * expr = value;

    if (size <= 26) return ellipsis(buf);
    return snprintf(buf, size, "<#LAMBDA %016lx>", (uintptr_t) value);
}

static size_t showMacro(char * buf, size_t size, void * value) {
    //ExprLexical * expr = value;

    if (size <= 25) return ellipsis(buf);
    return snprintf(buf, size, "<#MACRO %016lx>", (uintptr_t) value);
}

static void deleteLexical(void * value) {
    ExprLexical * expr = value;

    if (expr->rho != NULL)
        deleteRho(expr->rho);

    for (size_t i = 0; i < expr->vars.size; i++)
        free(getArray(&expr->vars, i));

    freeArray(&expr->vars);
}

static inline void * moveTrie(Region * region, Trie * T) {
    void * retptr = T;

    for (BinaryTreeValue * nv = T->head2; nv != NULL; nv = nv->node.succ) {
        if (nv->value != NULL && move(region, nv->value) == NULL)
            retptr = NULL;
    }

    return retptr;
}

static void * moveLexical(Region * dest, void * value) {
    ExprLexical * expr = value; void * retptr = value;

    if (moveTrie(dest, &expr->rho->trie) == NULL)
        retptr = NULL;

    if (move(dest, expr->value) == NULL)
        retptr = NULL;

    return retptr;
}

ExprTag exprLambdaTag = {
    .eval   = trivEval,
    .apply  = applyLambda,
    .show   = showLambda,
    .delete = deleteLexical,
    .move   = moveLexical,
    .equal  = equalByRef,
    .size   = sizeof(ExprLexical)
};

ExprTag exprMacroTag = {
    .eval   = trivEval,
    .apply  = applyMacro,
    .show   = showMacro,
    .delete = deleteLexical,
    .move   = moveLexical,
    .equal  = equalByRef,
    .size   = sizeof(ExprLexical)
};

Rho * global = NULL; // TODO

void * newLexical(ExprTag * tag, Region * region, Array vars, void * value) {
    ExprLexical * retval = newExpr(region, tag);
    retval->rho   = newRho(global); // TODO
    retval->vars  = vars;
    retval->value = value;

    Rho * curr = region->rho;
    while (curr != NULL) {
        if (curr->lexical)
            copyRho(retval->rho, curr);

        curr = curr->next;
    }

    return retval;
}

void * externLexical(ExprTag * tag, Region * region, Array * xs) {
    if (xs->size <= 0) return throw(TypeErrorTag, "no arguments were given");

    for (size_t i = 0; i < xs->size - 1; i++) {
        if (tagof(getArray(xs, i)) != &exprAtomTag)
            return throw(TypeErrorTag, "%s expected to be an atom", showExpr(getArray(xs, i)));
    }

    Array vars = newArray(xs->size - 1);

    for (size_t j = 0; j < xs->size - 1; j++) {
        ExprAtom * i = getArray(xs, j);
        setArray(&vars, j, dup(i->value));
    }

    return newLexical(tag, region, vars, getArray(xs, xs->size - 1));
}

void * externLambda(Region * region, Array * xs) {
    return externLexical(&exprLambdaTag, region, xs);
}

void * externMacro(Region * region, Array * xs) {
    return externLexical(&exprMacroTag, region, xs);
}

void * externExpand(Region * region, Array * xs) {
    ARITY(1, xs->size);

    ExprLexical * o = eval(region, getArray(xs, 0));
    if (o == NULL) return NULL;

    if (tagof(o) != &exprLambdaTag && tagof(o) != &exprMacroTag)
        return throw(TypeErrorTag, "%s expected to be a closure", showExpr(o));

    return o->value;
}

void initLexicalTags(Region * region) {
    global = region->rho;

    newExprImmortal(&exprTag, &exprLambdaTag, &exprMacroTag, NULL);

    setVars(
        region->rho,
        "λ",      newExtern(region, externLambda),
        "Λ",      newExtern(region, externMacro),
        "expand", newExtern(region, externExpand),
        NULL
    );
}
