import socket
import threading


class TCPClient:
    def __init__(self, connectedHost, connectedPort):
        self.__nickname = input("Choose a nickname: ")
        self.__client = None
        self.__host = connectedHost
        self.__port = connectedPort
        self.connectToServer()

    def __del__(self):
        self.__client.close()

    def connectToServer(self):
        self.__client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.__client.connect((self.__host, self.__port))

    def receive(self):
        while True:
            try:
                message = self.__client.recv(1024).decode('ascii')
                if message == 'NICKNAME':
                    self.__client.send(self.__nickname.encode('ascii'))
                else:
                    print(message)
            except:
                print("An error occured!")
                self.__client.close()
                break

    def write(self):
        while True:
            message = '{}: {}'.format(self.__nickname, input(''))
            self.__client.send(message.encode('ascii'))


tcpClient = TCPClient('127.0.0.1', 55555)

receive_thread = threading.Thread(target = tcpClient.receive)
receive_thread.start()

write_thread = threading.Thread(target = tcpClient.write)
write_thread.start()