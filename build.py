# SPDX-FileCopyrightText: 2023 Gabriele Pongelli
#
# SPDX-License-Identifier: MIT

"""Module to build C/C++ extension with Poetry."""

import os
import platform
from glob import glob
from os.path import join, relpath, splitext

from setuptools.command.develop import develop
from setuptools.extension import Extension

# arch information comes from
# gcc -v
# gcc -dumpmachine

# info https://stackoverflow.com/questions/6928110/how-may-i-override-the-compiler-gcc-flags-that-setup-py-uses-by-default

_is_windows = 'Windows' in platform.system()  # pylint: disable=C0103
_arch = 'x86-64'  # pylint: disable=C0103
if _is_windows:
    _lflags = f"-Wl,--subsystem,windows,--out-implib,libcode128_{_arch}.a"
else:
    _lflags = "-Wl,-soname,libcode128.so"  # pylint: disable=C0103
    if 'arm' in platform.machine():
        _arch = "armv7"  # pylint: disable=C0103

# list of tuples (python_module, dict of C/C++ library build data ),
#   python_module is the extension that uses those C/C++ library info.
# the dict can be extended with all the kwargs needed by Extension to correctly build C/C++ sourcecode
_LIBS = [
    (
        'pycode128',
        {
            "libs": ['libs/code128'],
            # Building library part
            "lib_cflags": ["-O2", "-std=c99", "-Wall", "-fpic", "-Wextra", f"-march={_arch}", "-DADD_EXPORTS"],
            "lib_lflags": ["-shared", _lflags],
            # Python extension part
            # always define PY_SSIZE_T_CLEAN , see https://docs.python.org/3/extending/extending.html
            "extension_define_macros": [("PY_SSIZE_T_CLEAN", None), ("ADD_EXPORTS", None)],
            "extension_language": "c99",
            "extension_extra_compile_args": [],
            "extension_extra_link_args": [],
        },
    )
]


class CustomDevelop(develop):
    """Custom install procedure.

    When declaring a ``build.py`` poetry switches to setuptools during
    installation, i.e., it generates a temporary ``setup.py`` and then calls
    ``setup.py develop`` on it when you call ``poetry install``.
    Consequentially, we can hook into the develop command and customize the
    build to compile our source :) Note that this is only needed for the
    ``develop`` command, because the ``build`` command (``poetry build``)
    already includes ``build_clib``.

    This class then is the hook that will compile the source when we call
    ``poetry install``.

    """

    def run(self) -> None:  # type: ignore
        # build archives (.lib) these are declared in the `libraries` kwarg of
        # setup(). Extensions may depend on these, so we have to build the libs
        # them first.
        self.run_command("build_clib")
        super().run()


# custom_extension = Extension(
#     "pycode128.pycode128",
#     sources=["pycode128/pycode128.c"],
#     # define_macros=[("PY_SSIZE_T_CLEAN",)],
#     # we need to declare the extenal dependencies
#     include_dirs=["libs/code128"],
#     libraries=["code128"],  # see below
# )


# setuptools.extension requires unix separator
def _unix_form(file_path: str) -> str:
    return file_path.replace('\\', '/')


def build(setup_kwargs):
    """
    This is a callback for poetry used to hook in our extensions.
    """

    setup_kwargs.update(
        {
            # declare archives (.lib) to build. These will be linked to
            # statically by extensions, cython, ...
            "libraries": [
                (
                    "code128",
                    {
                        "sources": [
                            _unix_form(path)
                            for _, _source_libs in _LIBS
                            for _source_folder in _source_libs['libs']
                            for root, _, _ in os.walk(os.sep.join([_source_folder]))
                            for path in glob(join(root, '*.c'))
                            if 'code128png' not in path
                        ],
                        # flags and dependencies of this library
                        # "include_dirs": ...
                        # "libraries": ...
                        "cflags": [
                            itm
                            for pylib, _source_libs in _LIBS
                            for itm in _source_libs['lib_cflags']
                            if pylib == 'pycode128'
                        ],
                        "lflags": [
                            itm
                            for pylib, _source_libs in _LIBS
                            for itm in _source_libs['lib_lflags']
                            if pylib == 'pycode128'
                        ],
                    },
                ),
            ],
            "ext_modules": [
                Extension(
                    splitext(relpath(_unix_form(path), start='.').replace(os.sep, '.'))[0],
                    sources=[_unix_form(path)],
                    define_macros=_source_libs['extension_define_macros'],
                    include_dirs=_source_libs['libs'],
                    language=_source_libs["extension_language"],
                    extra_compile_args=_source_libs["extension_extra_compile_args"],
                    extra_link_args=_source_libs["extension_extra_link_args"],
                )
                for _py_lib, _source_libs in _LIBS
                for root, _, _ in os.walk(os.sep.join([_py_lib]))
                for path in glob(join(root, '*.c'))
            ],
            # hook into the build process to build our external sources before
            # we build and install the package.
            "cmdclass": {"develop": CustomDevelop},
        }
    )


#
# if __name__ == '__main__':
#     # _LIBS = [('py', 'libs/code128')]
#
#     for _py_lib, _source_libs in _LIBS:
#         for _source_folder in _source_libs['libs']:
#             for root, _, _ in os.walk(os.sep.join([_source_folder])):
#                 print(root)
#                 for path in glob(join(root, '*.c')):
#                     print(path)
#                     print(dirname(path))
#
#     _a = [_unix_form(path)
#           for _py_lib, _source_lib in _LIBS
#           for source_fold in _source_lib['libs']
#           for root, _, _ in os.walk(os.sep.join([source_fold]))
#           for path in glob(join(root, '*.c'))
#           if 'code128png' not in path]
#
#     for _py_lib, _ in _LIBS:
#         for root, _, _ in os.walk(os.sep.join([_py_lib])):
#             print(root)
#             for path in glob(join(root, '*.c')):
#                 print(path)
#                 print(dirname(path))
#                 print(splitext(relpath(path, start='.').replace(os.sep, '.'))[0])
#
#     ext = [Extension(splitext(relpath(_unix_form(path), start='.').replace(os.sep, '.'))[0],
#                      sources=[_unix_form(path)],
#                      define_macros=_source_libs['define_macros'],
#                      include_dirs=_source_libs['libs'])
#            for _py_lib, _source_libs in _LIBS
#            for root, _, _ in os.walk(os.sep.join([_py_lib]))
#            for path in glob(join(root, '*.c'))
#            ]
#     print(ext)
#
#
