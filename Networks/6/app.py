import os
import sys
import datetime

def main():
    # Check if the input file is provided
    if len(sys.argv) != 2:
        print("Usage: python send_reminders.py input_file")
        return

    input_file = sys.argv[1]

    # Read the input file
    with open(input_file, 'r') as file:
        events = file.readlines()

    # Get today's date
    today = datetime.date.today()

    # Calculate the target date
    target_date = today + datetime.timedelta(days=6)

    # Filter the events that match the target date
    matching_events = []
    for event in events:
        event_date, event_name = event.strip().split(' ', 1)
        day, month = event_date.split('/')
        event_date = datetime.date(today.year, int(month), int(day))

        if event_date == target_date:
            matching_events.append(event_name)

    # Send an email if there are matching events
    if matching_events:
        email_subject = "Upcoming events reminder"
        email_body = "Upcoming events in 6 days:\n\n" + '\n'.join(matching_events)
        send_email(email_subject, email_body, "priyul20@gmail.com")

def send_email(subject, body, recipient):
    cmd = f'echo "{body}" | mail -s "{subject}" {recipient}'
    os.system(cmd)

if __name__ == '__main__':
    main() 
