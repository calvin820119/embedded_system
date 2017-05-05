#!/bin/bash
sensors | grep Core | cut -c 18-21 | sed ':a;N;$!ba;s/\n/ /g'>> figure/data.txt;

