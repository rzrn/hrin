# Copyright © 2026 rzrn

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

HRINLIBS = bin/hrin.a bin/hrinlib.a bin/baselib.a bin/commonlib.a

bin/hrin: $(HRINLIBS)
	$(CC) $(LDFLAGS) $(HRINLIBS) -o $@

all: bin/hrin

bin/baselib.a: src/baselib/byte.o src/baselib/integer.o src/baselib/string.o
	$(AR) $(ARFLAGS) $@ $?

bin/commonlib.a: src/commonlib/array.o src/commonlib/avl.o src/commonlib/trie.o
	$(AR) $(ARFLAGS) $@ $?

bin/hrinlib.a: src/hrinlib/atom.o src/hrinlib/basic.o src/hrinlib/boolean.o\
               src/hrinlib/cc.o src/hrinlib/error.o src/hrinlib/expr.o\
               src/hrinlib/extern.o src/hrinlib/lambda.o src/hrinlib/nil.o
	$(AR) $(ARFLAGS) $@ $?

bin/hrin.a: src/hrin/hrin.o src/hrin/io.o src/hrin/lexer.o src/hrin/parser.o
	$(AR) $(ARFLAGS) $@ $?

clean:
	@find src/ -name '*.o' -exec echo rm -f {} \; -exec rm -rf {} \;
	@find bin/ -name '*.a' -exec echo rm -f {} \; -exec rm -rf {} \;
	rm -f bin/hrin

.c.o:
	$(CC) $(CFLAGS) -Iinclude/ -c $< -o $@
