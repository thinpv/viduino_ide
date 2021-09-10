import sys
import lzma

sys.argv.pop(0)
inName = sys.argv.pop(0)
outName = sys.argv.pop(0)

print(inName)
print(outName)

with open(inName, "rb") as inFile:
	with lzma.open(outName, "wb", format=lzma.FORMAT_ALONE) as outFile:
		outFile.write(inFile.read())