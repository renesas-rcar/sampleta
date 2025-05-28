#!/bin/bash
unset LD_LIBRARY_PATH

if [ -z "$1" ] || [ -z "$2" ]; then
	echo "Error: You need to enter 2 parameters when running the script."
	echo "Usage: ./build.sh <OPTEE_CLIENT> <QNX_SDP_DIR>"
	exit 1
fi

export WORKDIR=$(pwd)
export OPTEE_CLIENT=$1
export QNX_SDP_DIR=$2

### Environment parameters of Client App
source ${QNX_SDP_DIR}/qnxsdp-env.sh
export TEEC_EXPORT=${OPTEE_CLIENT}/out/export/usr
export HOST_CROSS_COMPILE=aarch64-unknown-nto-qnx7.1.0-

### Build Client App
cd $WORKDIR/host
make clean
make all

