#!/bin/python
import sys
import os
from pathlib import Path

#print(os.getcwd(), file=sys. stderr)
#print(sys.argv[1], file=sys. stderr)
#print(sys.argv[2], file=sys. stderr)

header = ""
for path in sys.argv[1:-1]:
    filename = Path(path).stem.replace('.', '_')
    header += "const char* "+ filename +" =\n"
    with open(path, 'r') as file:
        for line in file.readlines():
            header += "    \""+line.replace("\n", "").replace("\\", "\\\\").replace("\"", "\\\"")+"\\n\"\\\n"
    header += "    \"\";\n\n"

#print(header)
with open(sys.argv[-1], 'w') as file:
    file.write(header)


