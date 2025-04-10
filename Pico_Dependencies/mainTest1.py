import network
import socket
import time

ssid = 'Phi'
password = 'RISisTheKey!MU-MIM0'

wlan = network.WLAN(network.STA_IF)
wlan.active(True)
wlan.connect(ssid, password)

# Wait for connection
while not wlan.isconnected():
    print("Connecting...")
    time.sleep(1)

print("Connected, IP:", wlan.ifconfig()[0])

# Setup socket
addr = socket.getaddrinfo('0.0.0.0', 8080)[0][-1]

s = socket.socket()
s.bind(addr)
s.listen(1)
print("Listening on", addr)

while True:
    cl, addr = s.accept()
    print("Client connected from", addr)
    cl.send('HTTP/1.0 200 OK\r\nContent-type: text/html\r\n\r\n')
    cl.send('<h1>Hello from Pico W</h1>')
    cl.close()