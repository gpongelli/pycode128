# Makefile (GNU make)

CC = gcc
#PYTHON        ?= python3
# dependencies for test running code
CHECK_DEPS = $(wildcard *.c)
# use python3-config to get python compiler and linker flags for use when
# linking python into external C code (our test runner)
PY_CFLAGS ?= -fPIE $(shell python3-config --cflags) -I${CURDIR}/installed/include
# --embed is required on ubuntu or -lpythonx.y is omitted by --ldflags
#PY_LDFLAGS := $(shell python3-config --embed --ldflags)
PATCHED_PY_LD_FLAGS := $(shell python3-config --embed --ldflags | sed -e 's/lpthread/pthread/')


# linker flags specifically for compiling the test runner (libcheck)
CHECK_LDFLAGS = $(PATCHED_PY_LD_FLAGS) -L${CURDIR}/installed/lib -lcheck

CHECK_TOOL = check-0.15.2

# build C extension module inplace. change dependencies as needed.
#inplace: demo.c
#    @$(PYTHON) setup.py build_ext --inplace

# build test runner and run unit tests using check
#
check: $(CHECK_DEPS)
	@$(CC) $(PY_CFLAGS) -o runner $(CHECK_DEPS) $(CHECK_LDFLAGS)
	@LD_LIBRARY_PATH=${CURDIR}/installed/lib ./runner

########### COPY check into poetry env to find pytest
## poetry env info --path
## cp runner into $(poetry env info --path)/bin
## export LD_LIBRARY_PATH=$(pwd)/check_c/installed/lib/
## poetry run runner -->> STARTS!!



# https://stackoverflow.com/questions/16467718/how-to-print-out-a-variable-in-makefile
print-%  : ; @echo $* = $($*)

# apt install -y libthreadpool-dev texinfo autotools build-essentials
# autoreconf -i
# ./configure --prefix=$(pwd)/../check_installed

#pro:
#	export PATCHED_PV=$(shell python3-config --embed --ldflags | sed -e 's/lpthread/pthread/')


extract:
	tar -xzvf $(shell find *.tar.gz)  > /dev/null 2>&1
	mkdir -p installed
	cd ./$(CHECK_TOOL) && autoreconf -i && ./configure --prefix=${CURDIR}/installed && make && make install
