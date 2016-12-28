#!/bin/sh

NAME="aubio-0.4.3"

rm -rf "$NAME"
unzip "../$NAME.zip" >/dev/null 2>&1
cp ../waf "$NAME"

cp ../aubionotes.c "$NAME/examples"
cp ../utils.c "$NAME/examples"
cd "$NAME"

set -x

export CC=${CK_CC}
export CXX=${CK_CXX}
export CFLAGS="${CK_PROG_COMPILER_VARS} -I${CK_ENV_LIB_RTL_XOPENME_INCLUDE} ${CK_PROG_COMPILER_FLAGS}"
export LDFLAGS="${CK_PROG_LINKER_LIBS} ${CK_EXTRA_LIB_M}"
export LIBS="${CK_PROG_LINKER_LIBS} ${CK_EXTRA_LIB_M}"

./waf configure
./waf build

cp build/examples/aubio* ../
