import mysql.connector
from mysql.connector.cursor import MySQLCursor

HOST = "localhost"
USER = "pythonUser"
PASSWORD = "pythonUser"
DATABASE = "weather"

def connect_to_db(host, user, password, databse):
    db_config = {
        "host": host,
        "user": user,
        "password": password,
        "database": databse,
    }

    try:
        # Establish a connection to the MySQL server
        connection = mysql.connector.connect(**db_config)
        # Create a cursor object to interact with the database
        cursor = connection.cursor()
    except mysql.connector.Error as error:
        return None, None
    return cursor, connection


def make_insert_query(id, temp, humidity, dataDate):
    return f"INSERT INTO measurements (sensor_id, temperature, humidity, time_of_measurement) VALUES ({id}, {temp}, {humidity}, '{dataDate}');"

def send_query(cursor: MySQLCursor, connection: mysql.connector.MySQLConnection, query):
    cursor.execute(query)
    connection.commit()  # Commit the transaction
    result = cursor.fetchall()
    return result

def disconnect_db(cursor: MySQLCursor, connection: mysql.connector.MySQLConnection):
    cursor.close()  # Close the cursor
    connection.close()  # Close the connection


