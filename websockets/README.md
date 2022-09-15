This is a bit rough and ready right now.

It uses images over websockets because mjpeg (as far as I can tell) 
doesn't give you much control over the appearance.

I think it's also probably possible to send more debug info via the websockets too, though haven't tried yet, again not possible with mjpeg.

# Running it

1. Run the sockets server

     `sudo python3 socket_server.py`


(it needed to be port 80 on my laptop, I think my laptop was blocking higher ports)


2. Load up the code onto the esp32 cam

Currently the laptop's IP is hardcoded (search for '192') and you need to add your network credentials.


3. Run a webserver in this directory

e.g. 

     python3 -m http.server 8000

and go to 

http://localhost:8000/

