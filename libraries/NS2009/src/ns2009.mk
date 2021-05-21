CSRCS += mean.c
CSRCS += median.c
CSRCS += ns2009.c
CSRCS += tscal.c
CSRCS += tsfilter.c

SRC_CPP+= touchscreen.cpp

DEPPATH += --dep-path $(NS2009_DIR)/src
VPATH += :$(NS2009_DIR)/src

CFLAGS += "-I$(NS2009_DIR)/src"
