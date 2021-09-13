import sys
import os

sys.argv.pop(0)
inName = sys.argv.pop(0)
print("   text    data     bss     dec     hex filename")
print("  27280    1676     720  ", os.path.getsize(inName), "  b8c88 build/firmware.elf")