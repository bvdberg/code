#!/bin/bash

DIR=$1

nmdepend $DIR/*.o > /dev/null
sed -i "s/\.o//" object.dot
dot -T png object.dot > image.png
rm -f object.dot package.dot
feh image.png &
