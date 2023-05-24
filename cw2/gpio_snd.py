from time import sleep
import periphery

start_freq = 262
end_freq = 272

controller = periphery.GPIO("/dev/gpiochip0", 3, "out")
freq_list = [196, 207, 220, 233, 246, 261, 277, 293]


def print_period(high_duration, low_duration):
    controller.write(True)
    sleep(high_duration)
    controller.write(False)
    sleep(low_duration)
    

step = 1 if end_freq > start_freq else -1

for current_freq in freq_list:

    period = 1.0 / current_freq
    for i in range(1000):
        print_period(period/2, period/2)
    

controller.close()
