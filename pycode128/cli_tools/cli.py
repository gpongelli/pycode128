# SPDX-FileCopyrightText: 2023 Gabriele Pongelli
#
# SPDX-License-Identifier: MIT

"""Console script for pycode128."""

import inspect
import types
from typing import Any, Callable, TypeVar, cast

import click
from cloup import option, option_group

from python_active_versions.utility import configure_logger

from pycode128 import __version__
from pycode128.pycode128 import PyCode128  # pylint: disable=ungrouped-imports   # pylint: disable=no-name-in-module
from pycode128.code128_image import Code128Image


F = TypeVar('F', bound=Callable[..., Any])


CONTEXT_SETTINGS = {"help_option_names": ['-h', '--help']}


@click.command(context_settings=CONTEXT_SETTINGS)
@click.argument('data')
@click.option("-i", "--image", "image_name", type=str, required=False, help="Output file name for generated image.")
@option_group(
    "Generic Options",
    option(
        '-l',
        '--loglevel',
        'loglevel',
        type=click.Choice(["debug", "info", "warning", "error"], case_sensitive=False),
        default="info",
        show_default=True,
        help="set log level",
    ),
)
@click.version_option(__version__)
def pycode128(data: str, image_name: str, loglevel: str):
    """Code128 barcode generator library.

    DATA is the input string to be converted as Code128 label.
    \f
    Arguments:
        data: string to be converted in code128
        image_name: optional string to save generated barcode image
        loglevel: set log level.
    """
    _fnc_name = cast(types.FrameType, inspect.currentframe()).f_code.co_name
    _complete_doc = inspect.getdoc(eval(_fnc_name))  # pylint: disable=eval-used  # nosec B307
    _doc = f"{_complete_doc}".split('\n')[0]  # use only doc first row
    _str = f"{_doc[:-1]} - v{__version__}"
    click.echo(f"{_str}")
    click.echo("=" * len(_str))

    configure_logger(loglevel)

    _code128 = PyCode128(data)
    _code128.encode_raw()
    click.echo(f"Input string: {data}")
    click.echo(f"Barcode length: {_code128.length}")
    click.echo(f"Encoded data: {_code128.encoded_data}")

    if image_name:
        click.echo(f"Saving image to file: {image_name}")
        Code128Image(_code128.encoded_data).get_image().save(image_name)


if __name__ == "__main__":
    pycode128()  # pragma: no cover  # pylint: disable=no-value-for-parameter
