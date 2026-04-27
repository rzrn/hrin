#!/bin/sh -x

DEPFILE=bin/makedepend.mk

if [ ! -e ${DEPFILE} ]
then
  touch ${DEPFILE}
  find src/ -name '*.c' -exec makedepend -f ${DEPFILE} -Iinclude/ -- {} +
fi

if [ -z "${OPTLEVEL}" ]
then
  OPTLEVEL=0
fi

make -f Makefile -f ${DEPFILE} CC=cc CFLAGS="-g -std=c99 -O${OPTLEVEL} -Wall -Wextra -pedantic" "$@"
