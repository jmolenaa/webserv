#!/usr/bin/env python3

import os

# make file current directory
print("hello world")
value = os.environ.get("QUERY_STRING")
print(value)
