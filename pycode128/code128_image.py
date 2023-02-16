# SPDX-FileCopyrightText: 2023 Gabriele Pongelli
#
# SPDX-License-Identifier: MIT

"""Class to retrieve a Pillow Image from byte buffer."""

from itertools import repeat

from PIL import Image


class Code128Image:
    def __init__(self, byte_buffer: bytes, bar_height: int = 200, bar_width: int = 5):
        self.__bar_height = bar_height
        self.__bar_width = bar_width

        # Once you get barcode_data, each byte corresponds to whether a vertical line should be drawn.
        # If the byte is 0xff, then draw a line. If the byte is 0x00, then don't.
        self.__bytes = byte_buffer.replace(b'\x00', b'\x01').replace(b'\xff', b'\x00').replace(b'\x01', b'\xff')

    def get_image(self) -> Image:
        _cols = b''.join(map(lambda x: bytes(repeat(x, self.__bar_width)), self.__bytes))
        _image_width = len(_cols)
        _image_bytes = b''.join(repeat(_cols, self.__bar_height))
        _image = Image.frombuffer(mode="L", size=(_image_width, self.__bar_height), data=_image_bytes)
        return _image
