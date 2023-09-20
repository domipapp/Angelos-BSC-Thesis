import mysql.connector
from mysql.connector.cursor import MySQLCursor

def connect_to_db(host, user, password, database):
    db_config = {
        "host": host,
        "user": user,
        "password": password,
        "database": database,
    }

    try:
        # Establish a connection to the MySQL server
        connection = mysql.connector.connect(**db_config)
        # Create a cursor object to interact with the database
        cursor = connection.cursor()
        return cursor, connection  # Return both cursor and connection
    except mysql.connector.Error as error:
        return None, None  # Return None for both cursor and connection

def make_insert_query(temp, humidity, dataDate):
    return f"INSERT INTO measurements (value1, value2, measurement_time) VALUES ({temp}, {humidity}, '{dataDate}');"

def send_query(cursor: MySQLCursor, query):
    cursor.execute(query)
    # No need to fetch results for an INSERT query
    return None

# Establish the database connection and cursor
cursor, connection = connect_to_db("localhost", "pythonUser", "pythonUser", "weather")

if cursor and connection:
    query = make_insert_query(1.19283, 38.991, "2023-01-01 00:00:00")
    send_query(cursor, query)
    connection.commit()  # Commit the transaction
    cursor.close()  # Close the cursor
    connection.close()  # Close the connection
else:
    print("Failed to connect to the database.")
