from flask import Flask, request, jsonify, send_from_directory
import common.db_handling as DB
from flask_cors import CORS
import os
import threading

ROOT_DIR = os.path.dirname(os.path.abspath(__file__))
ROUTE_DIR = os.path.join(ROOT_DIR, "../../Frontend/angelos/build/")

app = Flask(__name__, static_folder=ROUTE_DIR)
CORS(app)  # Enable CORS for the entire app

db_semaphore = threading.Semaphore()
# Simulated data (replace with actual data retrieval logic)

cursor, connection = DB.connect_to_db(
    host=DB.HOST, user=DB.USER, databse=DB.DATABASE, password=DB.PASSWORD
)


@app.route("/api/data", methods=["GET"])
def get_data():
    id = request.args.get("id")

    if id is not None:
        # Ensure to sanitize or validate the "id" value to prevent SQL injection (not shown here).
        db_semaphore.acquire()
        data = DB.send_query(
            cursor=cursor,
            connection=connection,
            query=f"SELECT humidity, temperature, time_of_measurement FROM measurements WHERE sensor_id = {id};",
        )

        dataMin = DB.send_query(
            cursor=cursor,
            connection=connection,
            query=f"SELECT MIN(humidity) AS min_humidity, MIN(temperature) AS min_temperature FROM measurements WHERE sensor_id = {id};",
        )
        db_semaphore.release()

        humidityMin, temperatureMin = dataMin[0]

        formattedTempData = [
            {"date": time.strftime("%Y-%m-%d %H:%M:%S"), "value": temperature}
            for _, temperature, time in data
        ]

        formattedHumidData = [
            {"date": time.strftime("%Y-%m-%d %H:%M:%S"), "value": humidity}
            for humidity, _, time in data
        ]

        return jsonify(
            {
                "temp": formattedTempData,
                "humid": formattedHumidData,
                "min_humidity": humidityMin,
                "min_temperature": temperatureMin,
            }
        )

    elif "unique_ids" in request.args:
        db_semaphore.acquire()
        data = DB.send_query(
            cursor=cursor,
            connection=connection,
            query=f"SELECT DISTINCT sensor_id FROM measurements;",
        )
        db_semaphore.release()
        data = [id for id, in data]
        return jsonify({"sensor_id_array": data})
    return jsonify({"error": "Missing or invalid parameters"}), 400


@app.route("/manifest.json")
def serve_manifest():
    return app.send_static_file("manifest.json")


@app.route("/static/<path:filename>")
def serve_static(filename):
    return send_from_directory(ROUTE_DIR + "/static", filename)


# Add a catch-all route to serve the main HTML file for any route
@app.route("/", defaults={"path": ""})
@app.route("/<path:path>")
def catch_all(path):
    return app.send_static_file("index.html")
