import socket
import threading
import datetime

HOST = "127.0.0.1"
PORT = 5000

def handle_client(conn, addr):
    print(f"[+] Connected: {addr}")

    with conn:
        while True:
            data = conn.recv(1024)
            if not data:
                break

            command = data.decode().strip().upper()
            print(f"Received from {addr}: {command}")

            if command == "PING":
                conn.sendall(b"PONG\n")
            elif command == "TIME":
                now = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                conn.sendall(f"{now}\n".encode())
            elif command == "EXIT":
                conn.sendall(b"Goodbye!\n")
                break
            else:
                conn.sendall(b"Unknown command\n")

    print(f"[-] Disconnected: {addr}")

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        s.listen()

        print(f"[SERVER] Listening on {HOST}:{PORT}")

        while True:
            conn, addr = s.accept()
            thread = threading.Thread(target=handle_client, args=(conn, addr))
            thread.start()

if __name__ == "__main__":
    main()
