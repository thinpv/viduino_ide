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

with open(inName, "rb") as inFile:
	with lzma.open(inName + ".temp", "wb", format=lzma.FORMAT_ALONE) as outFile:
		outFile.write(inFile.read())

inSize = os.path.getsize(inName + ".temp")
inMd5 = md5(inName)

outSize = os.path.getsize(inName)

var = struct.pack('8sII16s', str.encode('viduino'), inSize, outSize, inMd5)
print(var.hex())

with open(inName + ".temp", "rb") as inFile:
	with open(outName, "wb") as outFile:
		outFile.write(var)
		outFile.write(inFile.read())

#os.remove(inName + ".temp")