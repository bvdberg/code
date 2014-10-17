#!/bin/bash
set -e
set -o pipefail
set -u

_term() {
    kill -TERM $child1 2> /dev/null
    kill -TERM $child2 2> /dev/null
}

trap _term 15

exec ./writer 2>&1 | ./reader > /dev/null 2>&1 &

child2=$!
echo "child2=$child2";
child1=`jobs -p`
echo "child1=$child1"

echo "waiting"
wait $child1

