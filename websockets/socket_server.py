#!/usr/bin/python

import subprocess
import sys
import time
import os
import traceback
import math

from SimpleWebSocketServer import SimpleWebSocketServer, WebSocket

clients = []
class SimpleEcho(WebSocket):
    def handleMessage(self):
        command = self.data
        try:
          for client in clients:
            if client != self:
               client.sendMessage(command)
        except Exception as e:
          track = traceback.format_exc()
          print(e)
          print(track)

    def handleConnected(self):
        print(self.address, 'connected')
        clients.append(self)

    def handleClose(self):
        print(self.address, 'closed')
        clients.remove(self)

server = SimpleWebSocketServer('', 80, SimpleEcho)
server.serveforever()
