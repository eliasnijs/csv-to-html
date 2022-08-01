#!/bin/sh

NAME="personal website, project page compiler"
VERSION="0.0"

SOURCE="./compiler/compiler.cpp"
OUTPUT="./build/compiler"

INCS=""
LIBS=""

DFLAGS="-DMODE_DEBUG"
CPPFLAGS="-D_DEFAULT_SOURCE -D_BSD_SOURCE -D_POSIX_C_SOURCE=2 -DVERSION=\"${VERSION}\""
CFLAGS="-g -Wall -Wno-deprecated-declarations -O0 -O2 ${INCS} ${CPPFLAGS}"
LDFLAGS=${LIBS}

CC="g++" 
# building
cd $(dirname $0)
${CC} $CFLAGS $DFLAGS -o $OUTPUT $SOURCE $LDFLAGS
