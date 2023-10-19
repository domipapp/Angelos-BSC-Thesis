import re
from datetime import datetime
import socket

EOM_CHAR = b"q" # EndOfMessage character

# ^ indicates that the pattern should start matching from the beginning of the input string
# \d+ represents one or more digits
# \. represents a dot
# \s+ represents one or more whitespace characters (spaces or tabs)
# $ indicates that the pattern should finish matching at the end of the input string
DATA_PATTERN = pattern = r'^\d+\s+\d+\.\d+\s+\d+\.\d+$'

def is_data_valid(data):
    if re.match(pattern, data):
        return True
    return False

def extract_floats(data: str):
    values = data.split()
    return int(values[0]), float(values[1]), float(values[2])


# Extracts binary data as string from socket, if EOM_CHAR has arrived
# Returns the old and new byte characters, and message string if valid
def excract_data(clientSocket: socket.socket, binaryData : bytes):
        message = ""
        _, port = clientSocket.getsockname()
        chunk = clientSocket.recv(1024)
        binaryData += chunk
        if EOM_CHAR in binaryData:
            message = binaryData.decode()
            message = message.replace('q', '')
            print(f"Received message on port {port}: {message}")
            # Get current date and time
            current_datetime = datetime.now()
            # Format the date and time as a string
            formatted_datetime = current_datetime.strftime("%Y-%m-%d %H:%M:%S")
            # Append Celsius symbol to the temperature without newline
            return message, formatted_datetime, binaryData
        else:
            return None, None, binaryData