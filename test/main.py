import socket

HOST = "127.0.0.1"  # The server's hostname or IP address
PORT = 9001  # The port used by the server

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    command = input('Message : ')
    while command != "exit":
        s.sendall(bytes(command, 'utf-8'))
        print("Message sended.")
        command = input('Message : ')
