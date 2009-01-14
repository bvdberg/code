#!/bin/bash

for i in `seq -w 1 $1`;
do
touch file-$i.xml
done  

