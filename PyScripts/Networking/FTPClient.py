from ftplib import FTP

class FTPClient:
    def __init__(self, hostName, userName, passwordName):
        self.__host = hostName
        self.__user = userName
        self.__password = passwordName
        self.__file = './text.txt'
        self.__ftpInit()

    def __ftpInit(self):
        with FTP(self.__host) as ftp:
            try:
                ftp.login(user=self.__user, passwd=self.__password)
                print(ftp.getwelcome())
                self.__downloadFile(ftp)
                self.__uploadFile(ftp)
            except:
                print("Login failed")

    def __downloadFile(self, ftp):
        ftp.cwd("mainFolder")
        with open(self.__file, 'wb') as file:
            ftp.retrbinary("RETR " + "mytest.txt", file.write, 1024)
        ftp.quit()         

    def __uploadFile(self, ftp):
        with open(self.__file, 'rb') as file:
            ftp.storbinary("STOR " + "upload.txt", file)
        ftp.quit()


ftpClnt = FTPClient("127.0.0.1", "ilya", "1234")
