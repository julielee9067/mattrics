import statistics
from typing import List

import mysql.connector

from core.constants import NUM_CYCLE, PATIENT_ID, TOTAL_NUM_NODES
from core.secrets.secret_config import MYSQL_SECRET
from utils import logger


class DatabaseConnector:
    def __init__(self):
        self.connection = mysql.connector.connect(**MYSQL_SECRET)
        self.cursor = self.connection.cursor()

    def get_raw_data(self) -> List[List]:
        query = f"SELECT body FROM Test ORDER BY created_date DESC LIMIT {NUM_CYCLE}"
        self.cursor.execute(
            query,
        )
        result = self.cursor.fetchall()

        res = []
        for record in result:
            for mat_data in record:
                converted_data = [int(val) for val in mat_data.split(",") if val != ""]
                converted_data[0] = int(statistics.mean(converted_data))
                while len(converted_data) < TOTAL_NUM_NODES:
                    converted_data.insert(0, int(statistics.mean(converted_data)))
                print(len(converted_data))
                print(converted_data)
                # if len(converted_data) == TOTAL_NUM_NODES:
                res.append(converted_data)

        logger.info(f"Successfully found {len(res)} data: {res}")
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
    raw_data = db_connector.get_raw_data()
    print(raw_data)
