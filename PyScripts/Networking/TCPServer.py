import threading
import socket

class TCPServer:
    def __init__(self, hostName, portName):
        self.__host = hostName
        self.__port = portName
        self.__server = None
        self.__clients = []
        self.__nicknames = []
        self.defineProtocol()
    
    def __del__(self):
        self.__server.close()

    def defineProtocol(self):
        self.__server = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # TCP
        self.__server.bind((self.__host, self.__port))
        self.__server.listen()

    def broadcast(self, message):
        for client in self.__clients:
            client.send(message)

    def handle(self, client):
        while True:
            try:
                message = client.recv(1024)
                self.broadcast(message)
            except:
                index = self.__clients.index(client)
                self.__clients.remove(client)
                client.close()
                nickname = self.__nicknames[index]
                self.broadcast(f'{nickname} left the chat!'.encode('ascii'))
                self.__nicknames.remove(nickname)
                break

    def receive(self):
        while True:
            client, address = self.__server.accept()
            print(f"Connected with {str(address)}")
            client.send('NICK'.encode('ascii'))
            nickname = client.recv(1024).decode('ascii')
            self.__nicknames.append(nickname)
            self.__clients.append(client)
            print(f'Nickname of the client is {nickname}!')
            self.broadcast(f'{nickname} joined the chat'.encode('ascii'))
            client.send('Connected to the server!'.encode('ascii'))
            thread = threading.Thread(target=self.handle, args=(client,))
            thread.start()


srvr = TCPServer('127.0.0.1', 55555)
print("Server is running...")
srvr.receive()

