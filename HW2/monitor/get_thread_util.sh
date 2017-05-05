#!/bin/bash

top -b -d 1 | grep --line-buffered firefox | cut -c 1-6 >> 1.txt

exit 0
