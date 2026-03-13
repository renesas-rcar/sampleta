#!/bin/bash

unset LD_LIBRARY_PATH

## Prepare environment variable to build TA
export OPTEE_OS=/data1/nhinguyen/01_WORK_DIR/gen5/OPTEE/optee_os	# Users need to adjust this path
export OPTEE_CLIENT=/data1/nhinguyen/01_WORK_DIR/gen5/OPTEE/optee_client	# Users need to adjust this path
export TA_DEV_KIT_DIR="${OPTEE_OS}/out/arm-plat-rcar_gen5/export-ta_arm64"
export SDK=/data1/nhinguyen/01_WORK_DIR/sdk_adas_x5h		# Users need to adjust to SDK version 3.1.11

source ${SDK}/environment-setup-aarch64-poky-linux
export OPTEE_CLIENT_EXPORT=${SDK}/sysroots/aarch64-poky-linux/usr
export TEEC_EXPORT=${OPTEE_CLIENT}/out/export/usr    #Fix error not find the <tee_client_api.h> file

. ${SDK}/environment-setup-aarch64-poky-linux
export PATH=/shsv/hpc_sw1/HCTE/00_Common/GitlabCI/software/python_dir/bin:${PATH}	# Users need to export to the installed python
export SDKTARGETSYSROOT=${SDK}/sysroots/aarch64-poky-linux

##-- Other configs
export CFLAGS="${CFLAGS} --sysroot=${SDKTARGETSYSROOT}"  ##Fix error: "aarch64-poky-linux-ld.bfd: cannot find libgcc.a"
export TA_CROSS_COMPILE=aarch64-poky-linux-
export HOST_CROSS_COMPILE=aarch64-poky-linux-

export CFLAGS+=" -O -Wno-missing-braces " ## "-O" fixes error for: features.h:397:4: error: #warning _FORTIFY_SOURCE requires compiling with optimization (-O) [-Werror=cpp]
export CFLAGS+=" -DCFG_REE_FS=1"  ## Fix for error: regression_6000.c:87:1: error: no return statement in function returning non-void [-Werror=return-type]

## Build dynamic TA and host test
make O=out clean
make

