#!/bin/bash

set -e

function usage {
    echo "Usage: $0 [fast|slow]"
    exit 1
}

[[ $# -ne 1 ]] && usage

CMD=$1

if [ $CMD == "fast" ]; then
    echo "setting power to fast"
    sudo cpufreq-set -g performance -r
    exit 0
fi
if [ $CMD == "slow" ]; then
    echo "setting power to slow"
    sudo cpufreq-set -g ondemand -r
    exit 0
fi
usage

