import TCP_server.TCP_server as TCP_server
import common.my_socket as my_socket
import backend.web_backend as web_backend
import subprocess


if __name__ == "__main__":
    # web_backend.app.run(debug=True)
    # Start the Gunicorn server as a subprocess
    gunicorn_command = [
        "gunicorn",
        "-w",
        "4",
        "-b",
        my_socket.get_local_ip() + ":5000",
        "web_backend:app",
    ]
    print("Starting Gunicorn server: %s" % " ".join(gunicorn_command))
    subprocess.Popen(gunicorn_command)

    # TCP server program
    TCP_server.main()
