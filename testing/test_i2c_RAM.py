# -*- coding: utf-8 -*-

from i2c_mp_usb import I2C_MP_USB as SMBus
bus = SMBus()
bus.set_baudrate(100)

import time

ADDR = 0x20

STEPWISE = False

test = None

TIME_DELAY = 0.005
try:
    if STEPWISE:
        test = input("Start writing 0x34")
    bus.write_byte(ADDR, 0x34)
    time.sleep(TIME_DELAY)

    if STEPWISE:
        test = input("write nbytes")
    bus.write_byte(ADDR, 0x01)
    time.sleep(TIME_DELAY)

    if STEPWISE:
        test = input("write bytes")
    bus.write_byte(ADDR, 0x56)
    time.sleep(TIME_DELAY)

    if STEPWISE:
        test = input("switch to read")
    bus.write_byte(ADDR, 0x12)
    time.sleep(TIME_DELAY)

    if STEPWISE:
        test = input("write nbytes")
    bus.write_byte(ADDR, 0x05)
    time.sleep(TIME_DELAY)

    if STEPWISE:
        test = input("read bytes")
    #print(bus.read_byte(ADDR))
    #print(bus.read_byte(ADDR))
    print(bus.read_i2c_block_raw(ADDR, 5))

except Exception as e:
    print(e)

finally:
    bus.close()