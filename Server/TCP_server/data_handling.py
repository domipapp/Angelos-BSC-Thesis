import re
from datetime import datetime

EOM_CHAR = b"q"  # EndOfMessage character

# ^ indicates that the pattern should start matching from the beginning of the input string
# \d+ represents one or more digits
# \. represents a dot
# \s represents one whitespace character (space or tab)
# $ indicates that the pattern should finish matching at the end of the input string
DATA_PATTERN = r"^\d+\s\d+\.\d+\s\d+\.\d+$"


def is_data_valid(data):
    if re.match(DATA_PATTERN, data):
        return True
    return False


def extract_floats(data: str):
    if not is_data_valid(data):
        raise ValueError
    values = data.split()
    return int(values[0]), float(values[1]), float(values[2])


# Extracts binary data as string from socket, if EOM_CHAR has arrived
# Returns the old and new byte characters, and message string if valid
def extract_data(binaryData: bytes):
    if EOM_CHAR in binaryData:
        message = binaryData.decode()
        message = message.replace(EOM_CHAR.decode(), "")
        # Get current date and time
        current_datetime = datetime.now()
        # Format the date and time as a string
        formatted_datetime = current_datetime.strftime("%Y-%m-%d %H:%M:%S")

        return message, formatted_datetime, binaryData
    else:
        return None, None, binaryData
