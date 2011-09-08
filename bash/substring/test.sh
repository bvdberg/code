#!/bin/bash

input=abcdefgh1234.tar.gz

# delete from end
echo "substring = ${input%.tar.gz}"

# delete from beginning
echo "substring = ${input#abcd}"

