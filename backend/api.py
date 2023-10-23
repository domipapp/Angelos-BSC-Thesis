from flask import Flask, request, jsonify, send_from_directory
import db_handling as DB
from flask_cors import CORS
ROUTE = "test_website"
app = Flask(__name__, template_folder=ROUTE)
# CORS(app)  # Enable CORS for the entire app

# Simulated data (replace with actual data retrieval logic)

cursor, connection = DB.connect_to_db(host=DB.HOST, user=DB.USER, databse=DB.DATABASE, password=DB.PASSWORD)

@app.route('/api/data', methods=['GET'])
def get_data():
    id = request.args.get('id')

    if id is None:
        return jsonify({'error': 'Missing or invalid "id" parameter'}), 400

    # Ensure to sanitize or validate the "id" value to prevent SQL injection (not shown here).
    
    data = DB.send_query(cursor=cursor, connection=connection, query=f"SELECT temperature, time_of_measurement FROM measurements WHERE sensor_id = {id};")
    formatted_data = [(temperature, time.strftime("%Y-%m-%d %H:%M:%S")) for temperature, time in data]

    return jsonify({'id': formatted_data})


# Define a route that accepts a dynamic URL parameter
@app.route('/<path:file_path>')
def serve_html(file_path):
    try:
        file_path = file_path.replace(ROUTE, "")

        # Specify the MIME type for the file
        if file_path.endswith('.css'):
            mimetype = 'text/css'
        elif file_path.endswith('.js'):
            mimetype = 'application/javascript'
        else:
            mimetype = 'text/html'

        # Use send_from_directory to serve the file with the specified MIME type
        return send_from_directory(ROUTE , file_path, mimetype=mimetype)
    except FileNotFoundError:
        # Handle cases where the file doesn't exist
        return "Page not found", 404
