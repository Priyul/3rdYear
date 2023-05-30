import datetime
import pexpect
from base64 import b64decode, b64encode

# def modify_payload(part: EmailMessage):
#     content_transfer_encoding = part.get('Content-Transfer-Encoding', '').lower()
#     payload = part.get_payload()

#     if content_transfer_encoding == 'base64':
#         decoded_payload = b64decode(payload).decode('utf-8')
#     elif content_transfer_encoding == 'quoted-printable':
#         decoded_payload = quopri.decodestring(payload).decode('utf-8')
#     else:
#         decoded_payload = payload

#     # Apply your modifications to the decoded payload here...

#     if content_transfer_encoding == 'base64':
#         part.set_payload(b64encode(decoded_payload.encode()).decode())
#     elif content_transfer_encoding == 'quoted-printable':
#         part.set_payload(quopri.encodestring(decoded_payload.encode()).decode())

def send_email(data):
    sender_email = "priyul20@gmail.com"
    recipient_email = "priyul20@gmail.com"

    telnet = pexpect.spawn("telnet localhost 54321")

    telnet.expect("Connected")
    telnet.sendline("HELO mytestserver.local")
    telnet.expect("220")
    print('recieved 220')

    telnet.sendline(f"MAIL FROM: {sender_email}")
    telnet.expect("250")
    print('recieved 250 again ')

    telnet.sendline(f"RCPT TO: {recipient_email}")
    telnet.expect("250")
    print('recieved 250 2nd time')

    telnet.sendline("DATA")
    telnet.expect("354")
    print('recieved 354')

    telnet.sendline(f"{data}")
    telnet.sendline(".")
    telnet.expect("250")
    print('recieved 250 3rd time')

    telnet.sendline("QUIT")
    telnet.expect("221")
    print('recieved 221')
    telnet.close()


def main():
    #data = "fast rapid quick test slow ran stole test better best test very good very fast very bad tes test "
    data = "fast rapid quick test5 slow ran stole test6 better illuminati best test7 very good very fast very bad test8 test9 "
    send_email(data)

if __name__ == "__main__":
    main()
