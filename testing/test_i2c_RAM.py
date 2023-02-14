# -*- coding: utf-8 -*-

from i2c_mp_usb import I2C_MP_USB as SMBus
bus = SMBus()
bus.set_baudrate(100)

ADDR = 0x20

try:
    bus.write_i2c_block_raw(ADDR, [0, 10, 20])
    print(bus.read_i2c_block_raw(ADDR, 2) )
except Exception as e:
    print(e)
finally:
    bus.close()