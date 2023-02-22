#!/usr/bin/env python

# SPDX-FileCopyrightText: 2023 Gabriele Pongelli
#
# SPDX-License-Identifier: MIT

"""Tests for `pycode128` cli tool."""

from click.testing import CliRunner

from pycode128.cli_tools import cli
from pycode128 import __version__


def test_command_line_interface():
    """Test the CLI."""
    runner = CliRunner()
    result = runner.invoke(cli.pycode128)
    assert result.exit_code == 2
    assert 'Error: Missing argument' in result.output

    help_result = runner.invoke(cli.pycode128, ['--help'])
    assert help_result.exit_code == 0
    assert 'Show this message and exit.' in help_result.output

    version_result = runner.invoke(cli.pycode128, ['--version'])
    assert version_result.exit_code == 0
    assert __version__ in version_result.output


def test_command_line_argument():
    """Test the CLI arguments."""
    runner = CliRunner()

    barcode = runner.invoke(cli.pycode128, ['test'])
    assert barcode.exit_code == 0
    assert 'Input string: test' in barcode.output
    assert 'Barcode length: 99' in barcode.output

    out_file = runner.invoke(cli.pycode128, ['test', '-i', 'outfile.png'])
    assert out_file.exit_code == 0
    assert 'Saving image to file: outfile.png' in out_file.output