#!/bin/bash

function checkExists {
    NAME=$1
    echo "checking ${NAME}"
    if [ -f ${NAME} ]
    then
        echo "  -> exists"
    else
        echo "  -> does NOT exist"
    fi
}

checkExists test.sh
checkExists foo.sh

