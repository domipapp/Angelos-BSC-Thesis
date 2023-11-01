from flask import Flask, request, jsonify, send_from_directory
import db_handling as DB
from flask_cors import CORS

app = Flask(__name__, template_folder=ROUTE)
CORS(app)  # Enable CORS for the entire app

# Simulated data (replace with actual data retrieval logic)

cursor, connection = DB.connect_to_db(
    host=DB.HOST, user=DB.USER, databse=DB.DATABASE, password=DB.PASSWORD
)


@app.route("/api/data", methods=["GET"])
def get_data():
    id = request.args.get("id")

    if id is not None:
        # Ensure to sanitize or validate the "id" value to prevent SQL injection (not shown here).

        data = DB.send_query(
            cursor=cursor,
            connection=connection,
            query=f"SELECT temperature, time_of_measurement FROM measurements WHERE sensor_id = {id};",
        )
        formattedTempData = [
            (temperature, time.strftime("%Y-%m-%d %H:%M:%S"))
            for temperature, time in data
        ]
        data = DB.send_query(
            cursor=cursor,
            connection=connection,
            query=f"SELECT humidity, time_of_measurement FROM measurements WHERE sensor_id = {id};",
        )
        formattedHumidData = [
            (humidity, time.strftime("%Y-%m-%d %H:%M:%S")) for humidity, time in data
        ]
        return jsonify({"temp": formattedTempData, "humid": formattedHumidData})

    elif "unique_ids" in request.args:
        # Ensure to sanitize or validate the "idCount" value to prevent SQL injection (not shown here).

        data = DB.send_query(
            cursor=cursor,
            connection=connection,
            query=f"SELECT DISTINCT sensor_id FROM measurements;",
        )
        data = [id for id, in data]
        return jsonify({"sensor_id_array": data})
    return jsonify({"error": "Missing or invalid parameters"}), 400
