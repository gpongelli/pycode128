## 2.2.0rc0 (2023-03-15)

### Feat

- when building wheel from downloaded source, setup.cfg must be found

### Fix

- ARM image not working
- yamllint was removed

## 2.1.0 (2023-03-09)

### Feat

- install packages called within poetry
- using TOX basepython that has already correct python name depending on platform
- reorganize tox skip installation and use commands_pre
- removed unused package, correct includes

## 2.0.0 (2023-03-07)

### Fix

- renamed file to avoid naming clash with build package
- tox platform specification
- generate setup.py file used by pip, called from cibuildwheel

## 1.0.0 (2023-02-27)

### Feat

- new tests on optional cli parameters
- using cloup
- flake8 skips darglint on CLI tool. using tab alignment
- skip darglint on private methods, main or pycode128 method
- unify tox env
- cli implementation and tests, fix twine
- manage FNC3 and remove unused macros
- tests for FNC3 string
- new tests on generated image
- add typed files

### Fix

- tox os name using github action matrix
- remove generated file for test
- test
- build and install package to avoid pylint E0401: Unable to import 'pycode128.pycode128' (import-error)
- disable uppercase variable
- march not supported by clang
- tox-direct no more needed
- add windows call for poetry and reuse

## 0.4.0 (2023-02-17)

### Feat

- tests on encoded data
- added PIL, updated other deps
- class to convert bytes to PIL image
- licensing check_c subfolder
- new tests
- reuse common checks
- build a byte array instead of string
- new tests
- support cyclic garbage collection

### Fix

- macOS 12 has issue building with python 3.8
- macos environment variable minimum target
- lint stage passed
- missing sentinel
- input_data is not a tuple
- module decref
- allocate members
- avoid same method name
- README update

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
