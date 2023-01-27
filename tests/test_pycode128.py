#!/usr/bin/env python

# SPDX-FileCopyrightText: 2023 Gabriele Pongelli
#
# SPDX-License-Identifier: MIT

"""Tests for `pycode128` package."""

import pytest
from click.testing import CliRunner

from pycode128.cli_tools import cli
from pycode128.pycode128 import PyCode128


@pytest.fixture
def response():
    """Sample pytest fixture.

    See more at: http://doc.pytest.org/en/latest/fixture.html
    """
    # import requests
    # return requests.get('https://github.com/audreyr/cookiecutter-pypackage')


def test_object_creation():
    """Test PyCode128 object creation."""
    _code128 = PyCode128('test')
    assert _code128.input_data == 'test'
    assert _code128.__doc__ == "PyCode128 object"


def test_content(response):
    """Sample pytest test function with the pytest fixture as an argument.

    Arguments:
        response: pytest feature
    """
    # from bs4 import BeautifulSoup
    # assert 'GitHub' in BeautifulSoup(response.content).title.string
    del response


def test_command_line_interface():
    """Test the CLI."""
    runner = CliRunner()
    result = runner.invoke(cli.main)
    assert result.exit_code == 0
    assert 'pycode128' in result.output
    help_result = runner.invoke(cli.main, ['--help'])
    assert help_result.exit_code == 0
    assert '--help  Show this message and exit.' in help_result.output


def test_py_version():
    """Dummy test to print python version used by pytest."""
    import sys

    print(f"in TEST: {sys.version}  -- {sys.version_info}")
    # if sys.version_info <= (3, 9, 18):
    #     # 3.9 OK
    #     assert True
    # else:
    #     # 3.10 FAIL
    #     assert False
