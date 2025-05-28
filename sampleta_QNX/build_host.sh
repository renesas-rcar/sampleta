#!/bin/bash
unset LD_LIBRARY_PATH

export WORKDIR=$(pwd)
export OPTEE_CLIENT=/data/nhinguyen/_optee/gen4/_for_V4H/v430/optee_client #user need to adjust to the correct path 
export QNX_SDP_DIR=/shsv/SS2/RSS1/qnx/toolchain/qnx710_base                #user need to adjust to the correct path

### Environment parameters of Client App
source ${QNX_SDP_DIR}/qnxsdp-env.sh
export TEEC_EXPORT=${OPTEE_CLIENT}/out/export/usr
export HOST_CROSS_COMPILE=aarch64-unknown-nto-qnx7.1.0-

### Build Client App
cd $WORKDIR/host
make clean
make all

