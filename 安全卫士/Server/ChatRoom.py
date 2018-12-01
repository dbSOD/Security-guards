#coding:utf-8
author = "GuiShou"
import socket
import threading
import enum
import struct
import minsql
import time

class EnumMessageType(enum.Enum):
    Md5Message=0       #MD5消息
    CommitMessage=1    #提交样本消息

#添加聊天室服务器类
class ChatRoomServer(object):
    # 保存了当前登录的所有用户
    dictlist = {}

    # 初始化
    def __init__(self):
        # 创建socket
        self.server=socket.socket()
        # 绑定套接字
        self.server.bind(("127.0.0.1",1234))
        self.server.listen(5)
        print("``服务端启动了 吼吼吼！！！``")
        #初始化mysql连接
        self.sql=minsql.Mysql()

    # 自定义等待连接 每收到一个请求就给scoket开一个线程
    def OwnerAccept(self):
        accThread=threading.Thread(target=self.__acceptProc__)
        accThread.start()

    #等待连接
    def __acceptProc__(self):
        while True:
            socketClient,addrClient=self.server.accept()
            sockThread=threading.Thread(target=self.__recvProc__,args=(socketClient,))
            sockThread.start()


    #处理Md5消息
    def Md5Message(self,Message,m_socket):
        len, result = self.sql.getresult("SELECT * FROM virustable")
        m_socket.send(struct.pack('i',len))
        time.sleep(0.1)
        # 循环向客户端发送消息
        for index in range(len):
            print(result[index][0])
            m_socket.send(struct.pack('36s',result[index][0].encode('UTF8')))
            time.sleep(0.1)


    #处理提交样本消息
    def CommitMessage(self, Message, m_socket):
        #对收到的MD5解包
        MD5,= struct.unpack("36s", Message[4:40])
        MD5 = MD5.decode('GBK')
        self.sql.insert("insert into virustable value ('%s')" % (MD5))

        m_socket.send(struct.pack('i', 1))

    # 处理收到所有的消息
    def __recvProc__(self,s):
        while True:
            try:
                #接收客户端发送的消息 打
                message=s.recv(2060)
                #对收到的消息类型进行解包
                msgType, = struct.unpack("i", message[:4])

                #根据消息类型执行对应的函数
                self.dictfun[msgType](self,message,s)

            except Exception as msg:
                print(msg)
                print(s.getsockname(), '退出了服务器')
                s.close()
                break

    # 一个字典 包含的是对应的消息处理函数
    dictfun = {
        0: Md5Message,
        1:CommitMessage
    }
if __name__ == '__main__':
    MyScoket=ChatRoomServer()
    MyScoket.OwnerAccept()

