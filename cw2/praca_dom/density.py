# input params
total_duration = 1000
period = 100
min_percent = 20
max_percent = 70

# helper variables
min_percent /= 100
max_percent/= 100
num_of_periods = total_duration // period
percent_range = max_percent - min_percent

def print_period(high_duration, low_duration):
    print(f"{high_duration} 1")
    print(f"{low_duration} 0")
    print()

current_percent = min_percent
for i in range(num_of_periods + 1):

    high_duration = current_percent * (period)
    low_duration = period - high_duration

    print_period(high_duration, low_duration)

    current_percent += percent_range * (1 / num_of_periods)




