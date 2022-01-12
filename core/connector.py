import mysql.connector

from core.secret_config import MYSQL_SECRET


class DatabaseConnector:
    def __init__(self):
        self.connection = mysql.connector.connect(**MYSQL_SECRET)
        self.cursor = self.connection.cursor()
