import datetime
import pexpect

def read_events(file_name):
    with open(file_name, 'r') as f:
        lines = f.readlines()
    events = [line.strip().split(' ', 1) for line in lines]
    return events

def check_event_in_six_days(events):
    today = datetime.date.today()
    target_date = today + datetime.timedelta(days=6)
    target_date_str = target_date.strftime('%d/%m')

    for date, event in events:
        if date == target_date_str:
            return event
    return None

def send_email(event):
    sender_email = "priyul20@gmail.com"
    recipient_email = "priyul20@gmail.com"

    # Start telnet connection
    telnet = pexpect.spawn("telnet localhost 25")

    # Send SMTP commands
    telnet.expect("Connected")
    telnet.sendline("HELO mytestserver.local")
    telnet.expect("250")
    telnet.sendline(f"MAIL FROM: {sender_email}")
    telnet.expect("250")
    telnet.sendline(f"RCPT TO: {recipient_email}")
    telnet.expect("250")
    telnet.sendline("DATA")
    telnet.expect("354")
    telnet.sendline(f"Subject: Reminder - {event} in 6 days\r\n\r\nDear user,\r\n\r\nThis is a reminder that {event} is occurring in 6 days.\r\nBest regards,\r\nYour Reminder Bot")
    telnet.sendline(".")
    telnet.expect("250")
    telnet.sendline("QUIT")
    telnet.expect("221")
    telnet.close()

def main():
    events = read_events('events.txt')
    event = check_event_in_six_days(events)
    
    if event:
        send_email(event)

if __name__ == "__main__":
    main()
