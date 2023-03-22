from time import sleep
import periphery

controller = periphery.GPIO("/dev/gpiochip0", "GPIO27", "out")



# input params
total_duration = 10
period = 0.01
min_percent = 0
max_percent = 100

# helper variables
min_percent /= 100
max_percent/= 100
num_of_periods = total_duration // period
percent_range = max_percent - min_percent


def print_period(high_duration, low_duration):
    # print(f"{high_duration} 1")
    controller.write(True)
    sleep(high_duration)
    controller.write(False)
    sleep(low_duration)
    


current_percent = min_percent
for i in range(int(num_of_periods) + 1):

    high_duration = current_percent * (period)
    low_duration = period - high_duration

    print_period(high_duration, low_duration)

    current_percent += percent_range * (1 / num_of_periods)
