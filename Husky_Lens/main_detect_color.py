from huskylensPythonLibrary import HuskyLensLibrary
import time
import uos
import machine
import st7789py as st7789
from fonts import vga2_8x8 as font1
from fonts import vga1_16x32 as font2
import random
                                        # oled display height
husky = HuskyLensLibrary("I2C") ## SERIAL OR I2C 
 
#SPI(1) default pins
# spi1_sck=10
# spi1_mosi=11
# spi1_miso=8     #not use
# st7789_res = 12
# st7789_dc  = 13
# st7789_cs  = 9
disp_width = 240
disp_height = 240
CENTER_Y = int(disp_width/2)
CENTER_X = int(disp_height/2)
 
# print(uos.uname())
# spi1 = machine.SPI(1, baudrate=40000000, polarity=1)
# print(spi1)
# display = st7789.ST7789(spi1, disp_width, disp_width,
#                           reset=machine.Pin(st7789_res, machine.Pin.OUT),
#                           dc=machine.Pin(st7789_dc, machine.Pin.OUT),
#                           xstart=0, ystart=0, rotation=0)
 
while True:
    result = husky.command_request()
    value = ""
    
    # For Color Detection with id = 1
    # Error is the deviation different between Center frame and Image Center
    #print(result, len(result))
    try:
        for i in range(len(result)):
            if (result[i][4] == 1):
                xdif = result[i][0] - result[i][2]
                cx = result[i][2] + (xdif/2)
                
                error = CENTER_X - cx
                print(result[i], CENTER_X, result[i][4])
                print(cx,xdif,error)
            
    except:
        pass


