import network
import socket
import time

# Replace these with your actual Wi-Fi credentials
ssid = 'Phi'
password = 'RISisTheKey!MU-MIM0'

# Connect to Wi-Fi
wlan = network.WLAN(network.STA_IF)
wlan.active(True)
wlan.connect(ssid, password)

# Wait for connection
print("Connecting to Wi-Fi...")
while not wlan.isconnected():
    time.sleep(1)
print("Connected! IP address:", wlan.ifconfig()[0])

# Create simple HTML page
html = """<!DOCTYPE html>
<html>
    <head>
        <title>Home Sentinel</title>
    </head>
    <body>
        <h1>Welcome to Home Sentinel</h1>
    </body>
</html>
"""

# Setup socket
addr = socket.getaddrinfo('0.0.0.0', 80)[0][-1]
s = socket.socket()
s.bind(addr)
s.listen(1)
print("Listening on http://%s" % wlan.ifconfig()[0])

# Serve webpage
while True:
    try:
        cl, addr = s.accept()
        print('Client connected from', addr)
        cl.recv(1024)  # Read request (but we don't need to do anything with it)
        cl.send('HTTP/1.0 200 OK\r\nContent-type: text/html\r\n\r\n')
        cl.send(html)
        cl.close()
    except OSError:
        cl.close()
        print("Connection closed")