#!/usr/bin/env bash

echo `expr $1 - $2`

echo "in hex"
printf "%X\n" $(($1 - $2))
