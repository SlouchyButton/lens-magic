#!/bin/python
import sys
import os

#print(os.getcwd(), file=sys. stderr)
#print(sys.argv[1], file=sys. stderr)
#print(sys.argv[2], file=sys. stderr)

header = "const char *KernelSource =\n"
with open(sys.argv[1], 'r') as file:
    for line in file.readlines():
        header += "    \""+line.replace("\n", "")+"\"\\\n"
header += "    \"\";\n"

#print(header)
with open(sys.argv[2], 'w') as file:
    file.write(header)


