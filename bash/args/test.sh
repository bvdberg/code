#!/bin/bash

function help_function
{
    echo ""
    echo "Usage $0 -a <value> -b <value>"
    exit 1
}

while getopts "a:b:cd" opt
do
    case "$opt" in
      a ) varA="$OPTARG" ;;
      b ) varB="$OPTARG" ;;
      c ) varC=1 ;;
      d ) varD=1 ;;
      ? ) help_function ;;
    esac
done

echo "a[${varA}]  b[${varB}]  c[${varC}]  d[${varD}]"
