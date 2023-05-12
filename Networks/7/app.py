import socket
import base64
import datetime
import time
from email.parser import BytesParser, Parser
from email.policy import default

POP3_SERVER = "localhost"
SMTP_SERVER = "localhost"
SMTP_PORT = 25
EMAIL = "priyul20@gmail.com"
PASSWORD = "tdqqrzvuhuaaizwf"
INTERVAL = 60  # Check every 60 seconds

def connect_pop3():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((POP3_SERVER, 110))
    return s

def connect_smtp():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((SMTP_SERVER, SMTP_PORT))
    return s

def send_pop3_command(s, command):
    s.sendall(command.encode() + b'\r\n')
    response = s.recv(1024)
    return response.decode()

def send_smtp_command(s, command):
    s.sendall(command.encode() + b'\r\n')
    response = s.recv(1024)
    return response.decode()

def check_mail():
    s = connect_pop3()

    # Log in to the POP3 server
    send_pop3_command(s, f"USER {EMAIL}")
    send_pop3_command(s, f"PASS {PASSWORD}")

    # Get the number of messages
    response = send_pop3_command(s, "STAT")
    num_messages = int(response.split()[1])

    # Retrieve messages with subject 'prac7'
    prac7_messages = []
    for i in range(1, num_messages + 1):
        response = send_pop3_command(s, f"TOP {i} 0")
        message_bytes = b'\r\n'.join(response.split(b'\r\n')[1:])
        message = BytesParser(policy=default).parsebytes(message_bytes)
        if message['Subject'] == 'prac7':
            prac7_messages.append((i, message['From']))

    # Log out and close the connection
    send_pop3_command(s, "QUIT")
    s.close()

    return prac7_messages

def send_vacation_reply(smtp_socket, recipient_email):
    send_smtp_command(smtp_socket, "HELO mytestserver.local")
    send_smtp_command(smtp_socket, f"MAIL FROM: {EMAIL}")
    send_smtp_command(smtp_socket, f"RCPT TO: {recipient_email}")
    send_smtp_command(smtp_socket, "DATA")
    send_smtp_command(smtp_socket, f"Subject: Vacation Auto-Reply\r\n\r\nDear Sender,\r\n\r\nI am currently on vacation and will not be able to respond to your email. I will get back to you as soon as possible.\r\n\r\nBest regards,\r\n{EMAIL}\r\n.")
    send_smtp_command(smtp_socket, "QUIT")

def main():
    notified_senders = set()

    while True:
        prac7_messages = check_mail()

        smtp_socket = connect_smtp()
        for index, sender in prac7_messages:
            if sender not in notified_senders:
                send_vacation_reply(smtp_socket, sender)
                notified_senders.add(sender)
        smtp_socket.close()

        time.sleep(INTERVAL)

if __name__ == "__main__":
    main()
