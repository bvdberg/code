#!/bin/bash

#TODO also catch tags/branches output (seems to come from different process)

set -e

PWD=`pwd`
DATE=`date +%Y-%m-%d`
LOGFILE="$PWD/log-$DATE.txt"

function update {
    DIR=$1
    echo "updating $DIR"
    echo "" >> $LOGFILE
    echo "====== updating $DIR ======" >> $LOGFILE
    pushd $DIR > /dev/null
    git pull 2>&1 >> $LOGFILE
    popd > /dev/null
}

if [ -e $LOGFILE ]; then
    echo "already done for today"
    exit 0
fi

touch $LOGFILE

update linux-2.6
update lld
update llvm
update llvm/tools/clang
update flatbuffers
update ldc
update free_electrons-training-material
update systemd
update wayland
update weston

