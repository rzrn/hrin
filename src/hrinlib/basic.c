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

#include <hrinlib/basic.h>

#include <hrinlib/boolean.h>
#include <hrinlib/extern.h>
#include <hrinlib/lambda.h>
#include <hrinlib/atom.h>
#include <hrinlib/nil.h>
#include <hrinlib/cc.h>

Region * initHrinlib(void) {
    Scope * global = newScope(NULL);
    global->lexical = false;

    Region * region = newRegion(NULL);
    region->scope = global;

    initExternTag(region); // Everything below can rely on `newExtern`.

    initNilTag(region);
    initCCTag(region);
    initBooleanTag(region);
    initAtomTag(region);
    initLexicalTags(region);

    return region;
}
