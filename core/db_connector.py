from datetime import datetime, timedelta

import mysql.connector

from core.constants import PATIENT_ID
from core.secrets.secret_config import MYSQL_SECRET
from utils import logger


class DatabaseConnector:
    def __init__(self):
        self.connection = mysql.connector.connect(**MYSQL_SECRET)
        self.cursor = self.connection.cursor()

    def get_raw_data(self, recorded_date: datetime = datetime.now()) -> str:
        start_timestamp = (recorded_date - timedelta(days=1)).replace(hour=12)
        end_timestamp = recorded_date.replace(hour=12)

        logger.info(
            f"Searching for data between: {start_timestamp} and {end_timestamp}"
        )
        query = "SELECT body FROM RawData WHERE patientId = %s AND recorded_date BETWEEN %s AND %s"
        data = (PATIENT_ID, start_timestamp, end_timestamp)
        self.cursor.execute(query, data)
        result = self.cursor.fetchall()

        res = ""
        for item in result:
            for i in item:
                res += i

        return res

    def insert_therapist(self, first_name: str, last_name: str, email: str):
        query = (
            "INSERT INTO Therapist (_id, firstname, lastname, email) "
            "VALUES (%s, %s, %s, %s)"
        )
        self.cursor.execute(query, (first_name, last_name, email))
        self.connection.commit()

    def insert_pressure_data(self, path: str) -> int:
        id_query = "SELECT pressureId FROM Pressure ORDER BY pressureId DESC LIMIT 1;"
        self.cursor.execute(id_query)
        row = self.cursor.fetchone()
        _id = row[0] + 1

        path = str(path)
        query = "INSERT INTO Pressure (_id, path) VALUES (%s, %s)"
        self.cursor.execute(
            query,
            (
                _id,
                path,
            ),
        )
        self.connection.commit()
        logger.info(f"Successfully inserted pressure data")
        return self.cursor.lastrowid

    def insert_breathing_data(self, average_rate: float, path: str) -> int:
        id_query = (
            "SELECT breathingId "
            "FROM Breathing "
            "ORDER BY breathingId DESC "
            "LIMIT 1;"
        )
        self.cursor.execute(id_query)
        row = self.cursor.fetchone()
        _id = row[0] + 1

        path = str(path)
        query = "INSERT INTO Breathing (_id, averageRate, path) VALUES (%s, %s, %s)"
        self.cursor.execute(query, (_id, average_rate, path))
        self.connection.commit()
        logger.info(f"Successfully inserted breathing data")
        return self.cursor.lastrowid

    def insert_daily_data(self, pressure_id: int, breathing_id: int) -> int:
        id_query = "SELECT id FROM DailyData ORDER BY id DESC LIMIT 1;"
        self.cursor.execute(id_query)
        row = self.cursor.fetchone()
        _id = row[0] + 1

        query = (
            "INSERT INTO DailyData (_id, patientId, pressureId, breathingId) "
            "VALUES (%s, %s, %s, %s)"
        )
        self.cursor.execute(query, (_id, PATIENT_ID, pressure_id, breathing_id))
        self.connection.commit()
        logger.info(f"Successfully inserted daily data")
        return self.cursor.lastrowid

    def insert_pressure_daily_data(self, pressure_id: int) -> int:
        id_query = "SELECT id FROM DailyData ORDER BY id DESC LIMIT 1;"
        self.cursor.execute(id_query)
        row = self.cursor.fetchone()
        _id = row[0] + 1

        query = (
            "INSERT INTO DailyData (_id, patientId, pressureId) " "VALUES (%s, %s, %s)"
        )
        self.cursor.execute(query, (_id, PATIENT_ID, pressure_id))
        self.connection.commit()
        logger.info(f"Successfully inserted daily data")
        return self.cursor.lastrowid


if __name__ == "__main__":
    db_connector = DatabaseConnector()
    raw_data = db_connector.get_raw_data(
        recorded_date=(datetime(year=2022, month=3, day=11))
    )
    print(raw_data)
