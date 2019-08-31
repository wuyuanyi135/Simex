import time

import msgpack
import socket
import array
import threading

unpacker = msgpack.Unpacker()

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('127.0.0.1', 9990))
stop = False
def receiver():
    while True:
        ret = s.recv(1024)
        if not ret:
            print("Closed")
            stop = True
            break
        unpacker.feed(ret)
        for value in unpacker:
            try:
                #arr = array.array("d", value[b"ports"][0][b"dataRef"])
                print(value)
            except:
                print("error")
def sender():
    while not stop:
        buf = msgpack.packb({b"t": 4}) # info
        s.send(buf)
        time.sleep(3)
recv_thread = threading.Thread(target=receiver)
sender_thread = threading.Thread(target=sender)

recv_thread.start()
sender_thread.start()

recv_thread.join()
sender_thread.join()
