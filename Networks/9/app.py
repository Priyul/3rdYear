import socket
import pexpect
import base64

# Replacement policy
newspeak_policy = {
    'very fast': 'plusfast',
    'fast': 'speedful',
    'rapid': 'speedful',
    'quick': 'speedful',
    'slow': 'unspeedful',
    'ran': 'runned',
    'stole': 'stealed',
    'better': 'gooder',
    'best': 'goodest',
    'very good': 'plusgood',
    'very bad': 'plusungood'
}

disclaimer = "\n\nPlease do not take anything in this email seriously!"

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Allow reuse of a socket address
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

# Bind the socket to the port
server_address = ('localhost', 54321)
sock.bind(server_address)

# Listen for incoming connections
sock.listen(1)

while True:
    # Wait for a connection
    print('Waiting for a connection...')
    connection, client_address = sock.accept()
    data_mode = False
    smtp_server = None

    try:
        print('Connection from', client_address)
        connection.sendall(b"250 OK\r\n")
        # Receive the data in small chunks and retransmit it
        while True:
            data = connection.recv(1024)
            if data:

                command = data.decode('utf-8').strip()

                if data_mode and command != '.':
                    if 'illuminati' in command.lower():
                        command = 'Hello world'
                    else:
                        for word, replacement in newspeak_policy.items():
                            command = command.replace(word, replacement)
                        
                    command=command+disclaimer
                    print(f'Command after replacement: {command}')

                if smtp_server is None:
                    smtp_server = pexpect.spawn("telnet localhost 25")
                    smtp_server.expect("220.*\r\n")
                    response = smtp_server.match.group().decode()
                    connection.sendall(response.encode())
                    print(f'Sent to SMTP.py: {response}')
                    continue

                print(f'Sent to SMTP server: {command}')
                smtp_server.sendline(command)

                if command.upper() == 'DATA':
                    data_mode = True
                    print('set to true')
                    smtp_server.expect("[0-9]+[-\s].*\r\n")
                    response = smtp_server.match.group().decode()
                    connection.sendall(response.encode())
                    print(f'Sent to SMTP.py: {response}')
                elif data_mode and command == '.':
                    # End of DATA command
                    data_mode = False
                    smtp_server.expect("[0-9]+[-\s].*\r\n")
                    response = smtp_server.match.group().decode()
                    connection.sendall(response.encode())
                    print(f'Sent to SMTP.py: {response}')
                elif data_mode:

                    continue
                else:
                    smtp_server.expect("[0-9]+[-\s].*\r\n")
                    response = smtp_server.match.group().decode()
                    connection.sendall(response.encode())
                    print(f'Sent to SMTP.py: {response}')
            else:
                break
        connection.sendall(b"404 Not Found\r\n")
    finally:
        # Clean up the connection
        connection.close()
        if smtp_server is not None:
            smtp_server.close()


