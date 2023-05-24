from time import sleep
import periphery


controller_out = periphery.GPIO("/dev/gpiochip0", 27, "out")

controller_in = periphery.GPIO("/dev/gpiochip0", 18, "in")



while True:
    controller_out.write(False)
    if controller_in.read() == 1:
        controller_out.write(True)
        sleep(2)


