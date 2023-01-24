# SPDX-FileCopyrightText: 2023 Gabriele Pongelli
#
# SPDX-License-Identifier: MIT

"""Console script for pycode128."""

import click

from pycode128 import __version__


@click.command()
def main():
    """Main entrypoint."""
    _str = f"pycode128 v{__version__}"
    click.echo(_str)
    click.echo("=" * len(_str))
    click.echo("Python extension for Code128 barcode generator library")


if __name__ == "__main__":
    main()  # pragma: no cover
