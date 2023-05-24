# input params
# start_freq = 2
# end_freq = 10
start_freq = 10
end_freq = 2


def print_period(high_duration, low_duration):
    print(f"{high_duration} 1")
    print(f"{low_duration} 0")
    print()

step = 1 if end_freq > start_freq else -1

for current_freq in range(start_freq, end_freq + 1, step):

    period = 1 / current_freq
    print_period(period/2, period/2)