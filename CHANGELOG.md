## 0.3.0 (2023-02-09)

### Feat

- custom manylinux image speeds up workflow

## 0.2.0 (2023-02-08)

### Feat

- test on object deletion
- missing libs for python, copy runner after poetry env was made
- moved before test steps in github action to build python from sources
- build check with cmake, arch is always 64bit
- use more recent manylinux, install packages, skip unwanted archs
- linux builder will launch check tool
- makefile to extract and build check tool
- add check tool

### Fix

- using manylinux2014 that has gcc toolchain 10 to avoid issue with libsubunit
- disable subunit check tool or compile does not work in manylinux

## 0.1.2 (2023-02-01)

### Fix

- python < 3 not supported
- wheels are downloaded into subfolder

## 0.1.1 (2023-02-01)

### Fix

- install dependencies to have tox available

## 0.1.0 (2023-02-01)

### Feat

- dynamic compiler switch

### Fix

- lint stage
- parameter name
