import msgpack
import socket
import array

unpacker = msgpack.Unpacker()

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('127.0.0.1', 9990))
print("started")
while True:
    ret = s.recv(1)
    if not ret:
        print("Closed")
        break
    unpacker.feed(ret)
    for value in unpacker:
        try:
            arr = array.array("d", value[b"ports"][0][b"dataRef"])
            print(arr)
        except:
            pass