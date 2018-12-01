# coding: utf-8
import pymysql


class Mysql(object):

    def __init__(self):
        try:
            # 主机名 用户名 密码 端口号 数据库名
            self.mysql = pymysql.connect(host='127.0.0.1', db='virusdatabase',
                user='root', password='873217126', port=3306)

            # 使用游标进行数据库的操作
            self.cur = self.mysql.cursor()
        except Exception as msg:
            # 项目中，使用异常处理一定要输出异常信息
            # 如果不使用异常处理，会导致异常位置难以发现
            print(msg)


    def insert(self, mysql):
        try:
            self.cur.execute(mysql)
            # 执行的操作没有同步到数据库
            # 所有修改数据库内容的语句都需要commit
            self.mysql.commit()
            return True
        except Exception as msg:
            # 防止数据被损坏
            self.mysql.rollback()
            print(msg)
            return False

    def select(self, mysql):
        try:
            self.cur.execute(mysql)
            s = self.cur.fetchall()
            return len(s)

        except Exception as msg:
            # 防止数据被损坏
            self.mysql.rollback()
            print(msg)

    def getresult(self, mysql):
        try:
            self.cur.execute(mysql)
            s = self.cur.fetchall()
            return len(s), s

        except Exception as msg:
            # 防止数据被损坏
            self.mysql.rollback()
            print(msg)

