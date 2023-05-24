import gpio4
from time import sleep
gpio27 = gpio4.SysfsGPIO(27)
gpio27.export = True
# use the pin
gpio27.direction = 'out'
# set direction to output

for i in range(10):
    sleep(1)
    gpio27.value = 0
    sleep(1)
    # set value to low
    gpio27.value = 1
    # set value to high


gpio27.export = False

