import sys
import lzma
import os
import struct

import hashlib
def md5(fname):
  hash_md5 = hashlib.md5()
  with open(fname, "rb") as f:
    for chunk in iter(lambda: f.read(4096), b""):
      hash_md5.update(chunk)
  return hash_md5.digest()

sys.argv.pop(0)
inName = sys.argv.pop(0)
outName = sys.argv.pop(0)

inSize = os.path.getsize(inName)
inMd5 = md5(inName)

var = struct.pack('8sII16s', str.encode('viduino'), inSize, 0, inMd5)
print(var.hex())

with open(inName, "rb") as inFile:
	with open(outName, "wb") as outFile:
		outFile.write(var)
		outFile.write(inFile.read())