from machine import Pin
from time import sleep

led = Pin('LED',Pin.OUT)
def blink():
    led.on()
    sleep(1)
    led.off()
    sleep(1)

while True:
    blink()
