#!/bin/bash

if [ -z "$1" ]; then
	echo "Error: You need to enter 2 parameters when running the script."
	echo "Usage: ./build.sh <OPTEE_OS>"
	exit 1
fi

unset LD_LIBRARY_PATH

## Prepare environment variable to build TA
export OPTEE_OS=$1
export TA_DEV_KIT_DIR="${OPTEE_OS}/out/arm-plat-rcar_gen4/export-ta_arm64"
export SDK=/data/tutran/sdk		# Users need to adjust to SDK version 3.1.11
export OPTEE_CLIENT_EXPORT=${SDK}/sysroots/aarch64-poky-linux/usr
. ${SDK}/environment-setup-aarch64-poky-linux
export PATH=/shsv/Android/SoftIP/10_DienPham/GitlabCI/software/python_dir/bin:${PATH} #User need to adjust the python version
export SDKTARGETSYSROOT=${SDK}/sysroots/aarch64-poky-linux

##-- Other configs
export CFLAGS="${CFLAGS} --sysroot=${SDKTARGETSYSROOT}"  ##Fix error: "aarch64-poky-linux-ld.bfd: cannot find libgcc.a"
export TA_CROSS_COMPILE=aarch64-poky-linux-

export CFLAGS+=" -O -Wno-missing-braces " ## "-O" fixes error for: features.h:397:4: error: #warning _FORTIFY_SOURCE requires compiling with optimization (-O) [-Werror=cpp]
export CFLAGS+=" -DCFG_REE_FS=1"  ## Fix for error: regression_6000.c:87:1: error: no return statement in function returning non-void [-Werror=return-type]

## Build dynamic TA
make O=out clean
make

