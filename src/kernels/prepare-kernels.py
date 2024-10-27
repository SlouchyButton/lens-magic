#!/bin/python
import sys
import os
from pathlib import Path

# Load glsl files and convert the contents to single C header file with char* consts

header = ""
for path in sys.argv[1:-1]:
    filename = Path(path).stem.replace('.', '_').replace('-', '_')
    header += "const char* "+ filename +" =\n"
    with open(path, 'r') as file:
        for line in file.readlines():
            header += "    \""+line.replace("\n", "").replace("\\", "\\\\").replace("\"", "\\\"")+"\\n\"\\\n"
    header += "    \"\";\n\n"

with open(sys.argv[-1], 'w') as file:
    file.write(header)
