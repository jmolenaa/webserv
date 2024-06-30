#!/usr/bin/env python3

import os, time
import sys

# make file current directory
print("hello world")
try:
    value = os.environ.get("QUERY_STRING")
except Exception as e:
    sys.exit(12)

print(value)
# file = open("test", "w")
