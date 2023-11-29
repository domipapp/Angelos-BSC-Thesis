import TCP_server.TCP_server as TCP_server
import common.my_socket as my_socket
import backend.web_backend as web_backend
import subprocess
import os


if __name__ == "__main__":
    try:
        # web_backend.app.run(debug=True)
        # Start the Gunicorn server as a subprocess
        gunicorn_command = [
            "gunicorn",
            "-w",
            "4",
            "-b",
            my_socket.get_local_ip() + ":5000",
            "--log-level",
            "debug",
            "backend.web_backend:app",
        ]

        print("Starting Gunicorn server: %s" % " ".join(gunicorn_command))
        subprocess_webserver = subprocess.Popen(gunicorn_command)

        print("Starting TCP server")
        subprocess_TCP_server = subprocess.Popen(
            ["python3", "-m", "TCP_server.TCP_server"],
            cwd=os.path.abspath(os.path.join(os.path.dirname(__file__))),
        )

        subprocess_webserver.wait()
        subprocess_TCP_server.wait()

    except KeyboardInterrupt:
        # Handle keyboard interrupt (Ctrl+C)
        print("Keyboard interrupt detected. Stopping subprocesses.")

        # Send a signal to terminate the subprocesses
        subprocess_webserver.terminate()
        subprocess_TCP_server.terminate()

        # Wait for subprocesses to finish
        subprocess_webserver.wait()
        subprocess_TCP_server.wait()

        print("Subprocesses stopped.")
