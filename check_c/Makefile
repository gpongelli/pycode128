# Makefile (GNU make)

CC = gcc

# dependencies for test running code
CHECK_DEPS = $(wildcard *.c)
# use python3-config to get python compiler and linker flags for use when
# linking python into external C code (our test runner)
PY_CFLAGS ?= -fPIE $(shell python3-config --cflags) -I${CURDIR}/installed/include
# --embed is required on ubuntu or -lpythonx.y is omitted by --ldflags
#PY_LDFLAGS := $(shell python3-config --embed --ldflags)
PATCHED_PY_LD_FLAGS := $(shell python3-config --embed --ldflags | sed -e 's/lpthread/pthread/')


# linker flags specifically for compiling the test runner (libcheck)
# adding standard lib folder for python installation from sources
CHECK_LDFLAGS = $(PATCHED_PY_LD_FLAGS) -L$(LD_LIBRARY_PATH) -L${CURDIR}/installed/lib64 -lcheck

CHECK_TOOL = check-0.15.2

# extract tar.gz and install check locally
extract:
	tar -xzvf $(shell find *.tar.gz)  > /dev/null 2>&1
	mkdir -p ${CURDIR}/installed $(CHECK_TOOL)/build
	pwd
	ls -l
	cd $(CHECK_TOOL)/build && cmake --install-prefix ${CURDIR}/installed ../ && make && make install

# cd ./$(CHECK_TOOL) && autoupdate && autoreconf -i && ./configure --disable-build-docs --disable-subunit --prefix=${CURDIR}/installed && make && make install

# build test runner
build: $(CHECK_DEPS)
	@$(CC) $(PY_CFLAGS) -o runner $(CHECK_DEPS) $(CHECK_LDFLAGS)
	pwd
	ls -l

#cp -f runner $(shell poetry env info --path)/bin

# https://stackoverflow.com/questions/16467718/how-to-print-out-a-variable-in-makefile
print-%  : ; @echo $* = $($*)
