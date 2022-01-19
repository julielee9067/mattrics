import mysql.connector

from core.secret_config import MYSQL_SECRET


class DatabaseConnector:
    def __init__(self):
        self.connection = mysql.connector.connect(**MYSQL_SECRET)
        self.cursor = self.connection.cursor()

    def insert_therapist(self, first_name: str, last_name: str, email: str):
        query = (
            "INSERT INTO Therapist (firstname, lastname, email) " "VALUES (%s, %s, %s)"
        )
        self.cursor.execute(query, (first_name, last_name, email))
        self.connection.commit()


if __name__ == "__main__":
    db_connector = DatabaseConnector()
    db_connector.insert_therapist(
        first_name="Julie",
        last_name="Lee",
        email="julielee9067@gmail.com",
    )
