#!/usr/bin/env python

# SPDX-FileCopyrightText: 2023 Gabriele Pongelli
#
# SPDX-License-Identifier: MIT

"""Tests for `pycode128` cli tool."""

import logging
import os
from unittest.mock import patch

from click.testing import CliRunner

from pycode128 import __version__
from pycode128.cli_tools import cli


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
    os.unlink('outfile.png')


def test_command_line_mutually_exclusive():
    """Test the CLI arguments mutually exclusive."""
    runner = CliRunner()

    ret = runner.invoke(cli.pycode128, ['-c', '-r', 'testlabel'])
    assert 'Error: the following parameters are mutually exclusive:' in ret.output
    assert ret.exit_code == 2


def test_command_line_optional_arguments():
    """Test the CLI optional arguments."""
    runner = CliRunner()

    carriage_return = runner.invoke(cli.pycode128, ["-c", "TEST"])
    assert carriage_return.exit_code == 0
    assert 'Input string: TEST' in carriage_return.output
    assert 'Barcode length: 110' in carriage_return.output

    label_prog = runner.invoke(cli.pycode128, ["-r", "TEST"])
    assert label_prog.exit_code == 0
    assert 'Input string: [FNC3] $TEST' in label_prog.output
    assert 'Barcode length: 132' in label_prog.output

    action_label = runner.invoke(cli.pycode128, ["-a", "TEST"])
    assert action_label.exit_code == 0
    assert 'Input string: [FNC3] TEST' in action_label.output
    assert 'Barcode length: 121' in action_label.output


@patch('logging.basicConfig')
def test_logger(patched_log):
    """Testing configure_logger.

    Arguments:
        patched_log: patched basicConfig method
    """
    cli.configure_logger()
    patched_log.assert_called_with(
        level=logging.INFO, format='%(asctime)s [%(levelname)s - %(filename)s:%(lineno)d]    %(message)s', handlers=None
    )

    cli.configure_logger('info')
    patched_log.assert_called_with(
        level=logging.INFO, format='%(asctime)s [%(levelname)s - %(filename)s:%(lineno)d]    %(message)s', handlers=None
    )

    cli.configure_logger('debug')
    patched_log.assert_called_with(
        level=logging.DEBUG,
        format='%(asctime)s [%(levelname)s - %(filename)s:%(lineno)d]    %(message)s',
        handlers=None,
    )

    cli.configure_logger('warning')
    patched_log.assert_called_with(
        level=logging.WARN, format='%(asctime)s [%(levelname)s - %(filename)s:%(lineno)d]    %(message)s', handlers=None
    )

    cli.configure_logger('error')
    patched_log.assert_called_with(
        level=logging.ERROR,
        format='%(asctime)s [%(levelname)s - %(filename)s:%(lineno)d]    %(message)s',
        handlers=None,
    )
