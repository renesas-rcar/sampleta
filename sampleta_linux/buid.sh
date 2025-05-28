#!/bin/bash
set -x

unset LD_LIBRARY_PATH
#export BASED_DIR=/shsv/Android/SoftIP/36_NhiNguyen
#export WORK_DIR=${BASE_DIR}/01_WORK_DIR
export BASED_DIR=/data/nhinguyen/_optee
export WORK_DIR=${BASED_DIR}/gen3/v430
#export OPTEE_OS=${WORK_DIR}/gen4/_optee_os_upgrade_for_gen4/optee_os
export OPTEE_OS=${WORK_DIR}/_upgrade/optee_os
export ARM_CRYPTO_EXTENSION=${WORK_DIR}/sampleta
#export TA_DEV_KIT_DIR="${OPTEE_OS}/out/arm-plat-rcar_gen4/export-ta_arm64"
export TA_DEV_KIT_DIR="${OPTEE_OS}/out/arm-plat-rcar/export-ta_arm64"
export SDK=${BASED_DIR}/sdk_5
#source ${SDK}/environment-setup-aarch64-poky-linux
source ${SDK}/environment-setup-cortexa57-cortexa53-poky-linux
#export OPTEE_CLIENT_EXPORT=${SDK}/sysroots/aarch64-poky-linux/usr
export OPTEE_CLIENT_EXPORT=${SDK}/sysroots/cortexa57-cortexa53-poky-linux/usr
#. ${SDK}/environment-setup-aarch64-poky-linux
export PATH=/shsv/Android/SoftIP/10_DienPham/GitlabCI/software/python_dir/bin:${PATH}
export SDKTARGETSYSROOT=${SDK}/sysroots/cortexa57-cortexa53-poky-linux
##-- Other configs
export CFLAGS="${CFLAGS} --sysroot=${SDKTARGETSYSROOT}"  ##Fix error: "aarch64-poky-linux-ld.bfd: cannot find libgcc.a"
export TA_CROSS_COMPILE=aarch64-poky-linux-
export HOST_CROSS_COMPILE=aarch64-poky-linux-

export CFLAGS+=" -O -Wno-missing-braces " ## "-O" fixes error for: features.h:397:4: error: #warning _FORTIFY_SOURCE requires compiling with optimization (-O) [-Werror=cpp]
export CFLAGS+=" -DCFG_REE_FS=1 "  ## Fix for error: regression_6000.c:87:1: error: no return statement in function returning non-void [-Werror=return-type]
## Build_TA dynamic and host test
make O=out clean
make

##Copy Images to ROOFSz
#scp -f ${ARM_CRYPTO_EXTENSION}/host/sampleca nhiynguyen@172.29.145.230:/tftpboot/nhinguyen/S4/usr/bin
#scp -f ${ARM_CRYPTO_EXTENSION}/ta/2fa77580-ec0a-11e5-a47f0002a5d5c51b.ta nhiynguyen@172.29.145.230:/tftpboot/nhinguyen/S4/lib/optee_armtz
#cp -p ${ARM_CRYPTO_EXTENSION}/host/sampleca /tftpboot/nhinguyen/m3/usr/bin
#cp -p ${ARM_CRYPTO_EXTENSION}/ta/2fa77580-ec0a-11e5-a47f0002a5d5c51b.ta /tftpboot/nhinguyen/m3/lib/optee_armtz
