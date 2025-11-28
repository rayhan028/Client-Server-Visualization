import socket
import selectors
import types
import datetime

HOST = "127.0.0.1"
PORT = 5000

sel = selectors.DefaultSelector()

def accept_wrapper(sock):
    conn, addr = sock.accept()
    print(f"[+] Connected: {addr}")
    conn.setblocking(False)
    data = types.SimpleNamespace(addr=addr, inb=b"", outb=b"")
    sel.register(conn, selectors.EVENT_READ | selectors.EVENT_WRITE, data=data)

def service_connection(key, mask):
    sock = key.fileobj
    data = key.data

    if mask & selectors.EVENT_READ:
        try:
            recv_data = sock.recv(1024)
        except ConnectionResetError:
            recv_data = None

        if recv_data:
            data.inb += recv_data
            while b"\n" in data.inb:
                line, data.inb = data.inb.split(b"\n", 1)
                command = line.decode().strip().upper()
                print(f"[{data.addr}] Command: {command}")

                if command == "PING":
                    now = datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]
                    data.outb += f"PONG {now}\n".encode()
                elif command == "TIME":
                    now = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                    data.outb += f"{now}\n".encode()
                elif command == "EXIT":
                    data.outb += b"Goodbye!\n"
                    sel.unregister(sock)
                    sock.close()
                    print(f"[-] Disconnected: {data.addr}")
                else:
                    data.outb += b"Unknown command\n"
        else:
            sel.unregister(sock)
            sock.close()
            print(f"[-] Disconnected: {data.addr}")

    if mask & selectors.EVENT_WRITE and data.outb:
        sent = sock.send(data.outb)
        data.outb = data.outb[sent:]

def main():
    lsock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    lsock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    lsock.bind((HOST, PORT))
    lsock.listen()
    print(f"[SERVER] Listening on {HOST}:{PORT}")
    lsock.setblocking(False)
    sel.register(lsock, selectors.EVENT_READ, data=None)

    try:
        while True:
            events = sel.select(timeout=None)
            for key, mask in events:
                if key.data is None:
                    accept_wrapper(key.fileobj)
                else:
                    service_connection(key, mask)
    except KeyboardInterrupt:
        print("\n[SERVER] Shutting down...")
    finally:
        sel.close()

if __name__ == "__main__":
    main()
