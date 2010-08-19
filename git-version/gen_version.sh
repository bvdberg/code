#!/bin/sh
echo 'const char* git_version="'`git log --pretty=oneline --no-color -n 1`'";' > git_version.h
