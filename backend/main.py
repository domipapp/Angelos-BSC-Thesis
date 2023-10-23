import my_port
import my_socket
import data_handling
import db_handling
import socket
import select
import threading
import sys
import copy
import time
import api

MAX_PORT = 10
PORT_START = 9000
PORT_END = 9999
MAX_RETRYS = 1
TIMEOUT_SEC = 40
CLOSING_STRING = "close"

def process_socket_handle(clientSocket: socket.socket):
    binaryData = b""
    data = ""
    while True:
        # Wait for data to be received or for the timeout to expire
        ready_sockets, _, _ = select.select([clientSocket], [], [], TIMEOUT_SEC)

        if len(ready_sockets) == 0:
            # Timeout expired, assume client disconnected
            # Main handles closing sockets. Returning from process indicates disconnection
            # The thread stops and it is now detectable that it has stopped
            return 
        # Always update binaryData so bytes are not lost
        data, dataDate, binaryData = data_handling.excract_data(clientSocket=clientSocket, binaryData=binaryData)
        
        if data != None:
            # Explicitly asked to close the socket
            if CLOSING_STRING in data:
                return
            # Delete bytes, so it doesnt leak into next message
            binaryData = b""
            if not data_handling.is_data_valid(data):
                print("Invalid data format")
                continue
            id, temp, humidity = data_handling.extract_floats(data)
            query = db_handling.make_insert_query(id=id, temp=temp, humidity=humidity, dataDate=dataDate)
            dbSemaphore.acquire()
            db_handling.send_query(cursor=cursor, connection=connection, query=query )
            dbSemaphore.release()



# If a port has closed, update Ports object
def process_update_ports():
    while True:
        processesSemaphore.acquire()
        for serverSocket, clientSocket, thread in processes:
            if not thread.is_alive():
                _, port = serverSocket.getsockname()
                my_socket.close_socket(clientSocket=clientSocket, serverSocket=serverSocket)
                portsSemaphore.acquire()
                Ports.set_port(port=port, used=False, error=False)
                portsSemaphore.release()
                processes.remove([serverSocket, clientSocket, thread])
        processesSemaphore.release()

# Sometimes a port might be unavailable and is flagged used 
def process_release_past_used_ports():
    while True:
        portsSemaphore.acquire()
        ports = copy.deepcopy(Ports.get_port_list())
        portsSemaphore.release()
        for portNum, used, error in ports:
            if error:
                time.sleep(30) # Wait a little for port to be freed by OS
                portsSemaphore.acquire()
                Ports.set_port(port=portNum, used=False, error=False)
                portsSemaphore.release()



def main():
    thread = threading.Thread(target=process_update_ports)
    thread.start()
    thread = threading.Thread(target=process_release_past_used_ports)
    thread.start()
    while True:
        try:
            portsSemaphore.acquire()
            port = Ports.get_next_port()
            portsSemaphore.release()
        except:
            portsSemaphore.release()
            continue # Wait until new port frees up
        serverSocket = socket.socket()
        try:
            serverSocket = my_socket.set_up_socket(maxRetrys=MAX_RETRYS, port=port)
            clientSocket, serverSocket = my_socket.connect_client(serverSocket)
            thread = threading.Thread(target=process_socket_handle, args=(clientSocket,))
            thread.start()
            processesSemaphore.acquire()
            processes.append([serverSocket, clientSocket, thread])
            processesSemaphore.release()
        except (ValueError, RuntimeError):
            processesSemaphore.release()
            portsSemaphore.acquire()
            Ports.set_port(port=port, used=False, error=True)
            portsSemaphore.release()



        

if __name__ == "__main__":
    api.app.run(debug=True)
    # Global variables
    processes = []
    processesSemaphore = threading.Semaphore(1)
    try:
        Ports = my_port.MyPort(portStart=PORT_START, portEnd=PORT_END, portMax=MAX_PORT)
    except:
        print("Check constants! Incorrect confiugartion.")
        sys.exit()
    portsSemaphore = threading.Semaphore(1)
    cursor, connection = db_handling.connect_to_db(host=db_handling.HOST, user=db_handling.USER, password=db_handling.PASSWORD, databse=db_handling.DATABASE)
    if not cursor or not connection:
        print("Unable to connect to databae. Check configuration!")
        sys.exit()
    dbSemaphore = threading.Semaphore(1)    
    # Run program
    main()







    