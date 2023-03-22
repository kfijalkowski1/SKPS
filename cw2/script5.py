import periphery
from time import sleep

PREDKOSC_DZWIEKU = 340

def measure_distance(controller_trigger, controller_echo):
    controller_trigger.write(True)
    sleep(1e-5)
    controller_trigger.write(False)

    start = controller_echo.read_event().timestamp
    end = controller_echo.read_event().timestamp

    return PREDKOSC_DZWIEKU * (end - start) / 1e9


controller_echo = periphery.GPIO("/dev/gpiochip0", 9, "in", edge="both")
controller_trigger = periphery.GPIO("/dev/gpiochip0", 10, "out")


while True:
    print(f'Odległość: {measure_distance(controller_trigger, controller_echo)}')
    sleep(0.1)

