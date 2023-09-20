import socket
import time

# Returns a valid socket
def _set_up_socket(port):
    # Define the host and port to listen on
    host = socket.gethostbyname(socket.gethostname())
    # Create a TCP socket object
    serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    serversocket.setblocking(True)
    # Bind the socket to the host and port
    try:
        serversocket.bind((host, port))
    except Exception as fail:
        print(str(fail))
        print(f"NOT ABLE TO BIND PORT: {port}")
        raise RuntimeError
    return serversocket

# Returns a valid socket
def set_up_socket(maxRetrys, port):
    for i in range(maxRetrys + 1):
        try:
            serverSocket = _set_up_socket(port)
            return serverSocket
        except:
            if i != maxRetrys:
                time.sleep(10) # Retry, maybe OS freed up port
            else:
                raise RuntimeError
    

# Connects client to socket
# Returns sockets
def connect_client(serverSocket: socket.socket):
    host, port = serverSocket.getsockname()
    # Start listening for incoming connections
    serverSocket.listen()  #one connection enabled
    print(f"Listening on {host}:{port}...")

    # Wait for a client to connect
    clientsocket, addr = serverSocket.accept()
    print(f"Got a connection from {addr} on port: {port}")
    return clientsocket, serverSocket

# Closes sockets
def close_socket(clientSocket: socket.socket, serverSocket: socket.socket):
    host, port = serverSocket.getsockname()
    print(f"Closing {host}:{port}")
    clientSocket.close()
    serverSocket.close()
