import socket
import select
import threading
import sys
import time
import sys
from common import db_handling
from common import my_socket
from . import data_handling
from . import my_port


MAX_PORT = 10
PORT_START = 9000
PORT_END = 9999
MAX_RETRYS = 1
TIMEOUT_SEC = 50
CLOSING_STRING = "close"

threads = []

threadsSemaphore = threading.Semaphore(1)
portsSemaphore = threading.Semaphore(1)
dbSemaphore = threading.Semaphore(1)

cursor, connection = db_handling.connect_to_db(
    host=db_handling.HOST,
    user=db_handling.USER,
    password=db_handling.PASSWORD,
    databse=db_handling.DATABASE,
)
if not cursor or not connection:
    print("Unable to connect to database. Check configuration!")
    sys.exit()

try:
    Ports = my_port.MyPort(portStart=PORT_START, portEnd=PORT_END, portMax=MAX_PORT)
except:
    print("Check constants! Incorrect confiugartion.")
    sys.exit()


def thread_socket_handle(clientSocket: socket.socket):
    binaryData = b""
    _, port = clientSocket.getsockname()
    while True:
        # Wait for data to be received or for the timeout to expire

        ready_sockets, _, _ = select.select([clientSocket], [], [], TIMEOUT_SEC)

        if len(ready_sockets) == 0:
            # Timeout expired, assume client disconnected
            # Main handles closing sockets. Returning from thread indicates disconnection
            # The thread stops and it is now detectable that it has stopped
            return
        # Always update binaryData so bytes are not lost
        chunk = clientSocket.recv(1024)

        if chunk == b"":  # Client disconnected
            return

        binaryData += chunk
        data, dataDate, binaryData = data_handling.extract_data(binaryData=binaryData)

        if data != None:
            print(f"Received message on port {port}: {data}")
            # Explicitly asked to close the socket
            if CLOSING_STRING in data:
                return
            # Delete bytes, so it doesnt leak into next message
            binaryData = b""

            try:  # Extract data with format validation
                id, temp, humidity = data_handling.extract_floats(data)
            except ValueError:
                print("Invalid data format")
                continue

            query = db_handling.make_insert_query(
                id=id, temp=temp, humidity=humidity, dataDate=dataDate
            )
            dbSemaphore.acquire()
            db_handling.send_query(cursor=cursor, connection=connection, query=query)
            dbSemaphore.release()


# If a port has closed, update Ports object
def thread_update_ports():
    while True:
        threadsSemaphore.acquire()
        for serverSocket, clientSocket, thread in threads:
            if not thread.is_alive():
                _, port = serverSocket.getsockname()
                my_socket.close_socket(
                    clientSocket=clientSocket, serverSocket=serverSocket
                )
                portsSemaphore.acquire()
                Ports.set_port(port=port, used=False, error=False)
                portsSemaphore.release()
                threads.remove([serverSocket, clientSocket, thread])
        threadsSemaphore.release()


# Sometimes a port might be unavailable and is flagged used
def thread_release_bindError_ports():
    while True:
        portsSemaphore.acquire()
        ports = Ports.get_ports_list()
        portsSemaphore.release()
        for portNum, used, error in ports:
            if error:
                time.sleep(30)  # Wait a little for port to be freed by OS
                if not my_socket.test_port(portNum=portNum):  # still in use
                    portsSemaphore.acquire()
                    Ports.get_new_port([portNum, used, error])
                    portsSemaphore.release()
                    continue
                portsSemaphore.acquire()
                Ports.set_port(port=portNum, used=False, error=False)
                portsSemaphore.release()


def main():
    thread = threading.Thread(target=thread_update_ports)
    thread.start()
    thread = threading.Thread(target=thread_release_bindError_ports)
    thread.start()
    while True:
        try:
            portsSemaphore.acquire()
            port = Ports.get_next_port()
            portsSemaphore.release()
        except:
            portsSemaphore.release()
            continue  # Wait until new port frees up

        try:
            serverSocket = my_socket.set_up_socket(maxRetrys=MAX_RETRYS, port=port)
            clientSocket = my_socket.connect_client(serverSocket)
            thread = threading.Thread(target=thread_socket_handle, args=(clientSocket,))
            thread.start()
            threadsSemaphore.acquire()
            threads.append([serverSocket, clientSocket, thread])
            threadsSemaphore.release()
        except (ValueError, RuntimeError):
            threadsSemaphore.release()
            portsSemaphore.acquire()
            Ports.set_port(port=port, used=False, error=True)
            portsSemaphore.release()


if __name__ == "__main__":
    main()
