#!/usr/bin/env bash

## So we find files by a pattern, print0 and use xargs -0.
## then we form our command:
#### "mv <list> <list>" and then pipe that string to sed where we can modify it however we want
#### Since we only want to modify the output filename, we use 2 for second occurence in our sed command
## Lastly, we pipe the command to bash
## NOTE: This doesn't work on files with white space because "mv" thinks they are different params
find ./ -name "*.pdf.jpg" -print0 |xargs -0 -I {} echo mv {} {}|sed 's/.pdf.jpg/.jpg/2' | bash


find ./ -name "*.pdf.jpg" -exec echo mv "{}" "{}" |sed 's/.pdf.jpg/.jpg/2' | bash

