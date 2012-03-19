#!/bin/bash
UBIFS_IMG=user.ubifs.img
UBI_IMG=user.ubi
ROOT_DIR=image_root
USER_SIZE=764
rm -rf $UBI_IMG $UBIFS_IMG $ROOT_DIR
mkdir $ROOT_DIR
mkfs.ubifs -r $ROOT_DIR -o $UBIFS_IMG -m 1 -e 130944 -c $USER_SIZE
ubinize -o $UBI_IMG -m 1 -p 128KiB ubinize.cfg

