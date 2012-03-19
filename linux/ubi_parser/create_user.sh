#!/bin/bash
#+MKUBIFS_ARGS = "-m 1 -e 130944 -c 192"
#+UBINIZE_ARGS = "-m 1 -p 128KiB"

UBIFS_IMG=user.ubifs.img
UBI_IMG=user.ubi
rm -f $UBI_IMG $UBIFS_IMG
mkfs.ubifs -r image_root -o $UBIFS_IMG -m 1 -e 130944 -c 764
ubinize -o $UBI_IMG -m 1 -p 128KiB ubinize.cfg

