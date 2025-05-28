#!/bin/bash
set -x

unset LD_LIBRARY_PATH
export BASED_DIR=/data/nhinguyen/_optee		# User need to change this path
export WORK_DIR=${BASED_DIR}/gen4		# User need to change this path

export OPTEE_OS=${WORK_DIR}/_for_V4H/v430/optee_os	# User need to change this path
export OPTEE_CLIENT=/data/nhinguyen/_optee/gen4/v4_3_0/optee_client	# User need to change this path

export TA_DEV_KIT_DIR="${OPTEE_OS}/out/arm-plat-rcar_gen4/export-ta_arm64"
export SDK=${BASED_DIR}/sdk_adas			# User need to export correct sdk for adas
source ${SDK}/environment-setup-aarch64-poky-linux

export OPTEE_CLIENT_EXPORT=${SDK}/sysroots/aarch64-poky-linux/usr
export TEEC_EXPORT=${OPTEE_CLIENT}/out/export/usr    #Fix error not find the <tee_client_api.h> file

export PATH=/shsv/Android/SoftIP/10_DienPham/GitlabCI/software/python_dir/bin:${PATH}	# User export the installed python version
export SDKTARGETSYSROOT=${SDK}/sysroots/aarch64-poky-linux
##-- Other configs
export CFLAGS="${CFLAGS} --sysroot=${SDKTARGETSYSROOT}"  ##Fix error: "aarch64-poky-linux-ld.bfd: cannot find libgcc.a"
export TA_CROSS_COMPILE=aarch64-poky-linux-
export HOST_CROSS_COMPILE=aarch64-poky-linux-

export CFLAGS+=" -O -Wno-missing-braces " ## "-O" fixes error for: features.h:397:4: error: #warning _FORTIFY_SOURCE requires compiling with optimization (-O) [-Werror=cpp]
export CFLAGS+=" -DCFG_REE_FS=1 "  ## Fix for error: regression_6000.c:87:1: error: no return statement in function returning non-void [-Werror=return-type]
## Build_TA dynamic and host test
make O=out clean
make

