import network
import time
import socket

from machine import Pin

led = Pin('LED',Pin.OUT)
led.off()

#*************************************************************************** NETWORK SETUP

ssid = 'Your Wifi id'
password = 'Your Wifi Password'

#Create a WLAN object in station mode(STA_IF means client mode)
wlan = network.WLAN(network.STA_IF)
wlan.active()   #Turn on the Wifi
wlan.connect(ssid,password) #Connect to the wifi with 

#**************************************************************************** NETWORK SETUP

#**************************************************************************** Html page SETUP

#Create a basic Html page for posting
html = """<!DOCTYPE html>
<html>
    <head> <title>Pico W</title> </head>
    <body> <h1>Pico W</h1>
        <p>%s</p>
    </body>
</html>
"""

#**************************************************************************** Html page SETUP

#*************************************************************************** Connection status Checking 
maxWait = 10

while maxWait > 0:
    if(wlan.status() == 'STAT_CONNECT_FAIL' or wlan.status() == 'STAT_GOT_IP'):
        break #Exit if connection fails or succeeds
    maxWait -= 1
    print("Waiting for Connection...")
    time.sleep(1)

if(wlan.status() == 'STAT_CONNECT_FAIL' or wlan.status() == 'STAT_WRONG_PASSWORD'):
    print("Connection Failed: Check Passowrk, SSID and Retry")
else:
    print("Connected")
    status = wlan.ifconfig() #Gets IP configuration
    print("IP = " + status[0])

#*************************************************************************** Connection status Checking 

#*************************************************************************** Socket Programming Section

#Define a Socket Address: any IP, any HTTP Port
address = socket.getaddrinfo('0.0.0.0',80)[0][-1]

#Create and bind socket
soc = socket.socket()
soc.bind(address)
soc.listen(1)

print("Listening on", address)

#*************************************************************************** Socket Programming Section

#*************************************************************************** Main server loop
while True:
    try:
        cl,address = soc.accept()
        print("Client connected from: ", address)
        request = cl.recv(1024)   # Read the HTTP request
        print(request)
        request = str(request)    # Convert bytes to string

        #***************************************** CHECK THIS *******************************************
        # Check if request was to turn light on or off
        led_on = request.find('/light/on')     # Will return -1 if not found
        led_off = request.find('/light/off')

        print('led on = ' + str(led_on))
        print('led off = ' + str(led_off))

        # If '/light/on' was requested
        if led_on == 6:
            print("led on")
            led.value(1)          # Turn on LED
            stateis = "LED is ON"

        # If '/light/off' was requested
        if led_off == 6:
            print("led off")
            led.value(0)          # Turn off LED
            stateis = "LED is OFF"

        # Insert the current LED state into the HTML template
        response = html % stateis
        #***************************************** CHECK THIS *******************************************

        # Send HTTP response headers and HTML content
        cl.send('HTTP/1.0 200 OK\r\nContent-type: text/html\r\n\r\n')
        cl.send(response)
        cl.close()   # Close the connection

    except OSError as e:
        cl.close()
        print('connection closed')  # Handle any socket errors gracefully

#*************************************************************************** Main server loop