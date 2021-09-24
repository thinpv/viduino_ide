import sys
import lzma
import os

sys.argv.pop(0)
inName = sys.argv.pop(0)
outName = sys.argv.pop(0)

with open(inName, "rb") as inFile:
	with lzma.open(outName, "wb", format=lzma.FORMAT_ALONE) as outFile:
		outFile.write(inFile.read())

inSize = os.path.getsize(inName)
outSize = os.path.getsize(outName)
print("Compressed", inSize, "bytes into", outSize, "bytes ==> size reduced", "%.2f" % (inSize/outSize), "times")