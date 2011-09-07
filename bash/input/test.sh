#!/bin/bash

echo -n "Please type something: "
read -e INPUT
echo "You entered $INPUT as input"
if [ $INPUT == "y" ]; then
    echo "YES"
else
    echo "NO"
fi
