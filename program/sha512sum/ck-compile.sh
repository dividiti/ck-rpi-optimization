#!/bin/sh

NAME="coreutils-8.26"

rm -rf "$NAME"
tar -xf "../$NAME.tar.xz" >/dev/null 2>&1

cp ../md5sum.c "$NAME/src"
cd "$NAME"

set -x

export CC=${CK_CC}
export CXX=${CK_CXX}
export CFLAGS="${CK_PROG_COMPILER_VARS} -I${CK_ENV_LIB_RTL_XOPENME_INCLUDE} ${CK_PROG_COMPILER_FLAGS}"
export LDFLAGS="${CK_PROG_LINKER_LIBS} ${CK_EXTRA_LIB_M}"
export LIBS="${CK_PROG_LINKER_LIBS} ${CK_EXTRA_LIB_M}"

./configure
make -j 4

set +x

cp src/sha512sum ../a.out
