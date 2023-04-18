from flask import Flask, render_template_string, request
from datetime import datetime
import pytz

app = Flask(__name__)

CITIES = {
    'South Africa': 'Africa/Johannesburg',
    'New York': 'America/New_York',
    'London': 'Europe/London',
    'Tokyo': 'Asia/Tokyo',
    'Sydney': 'Australia/Sydney',
}

TEMPLATE = """
<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="refresh" content="1">
    <title>World Clock</title>
</head>
<body>
    <h1>World Clock</h1>
    <h2>South Africa: {{ local_time }}</h2>
    <h3>{{ selected_city }}: {{ selected_time }}</h3>
    <ul>
    {% for city in cities %}
        <li><a href="/?city={{ city }}">{{ city }}</a></li>
    {% endfor %}
    </ul>
</body>
</html>
"""

@app.route('/')
def world_clock():
    city = request.args.get('city')
    local_timezone = pytz.timezone(CITIES['South Africa'])
    local_time = datetime.now(local_timezone).strftime('%Y-%m-%d %H:%M:%S')

    if city and city in CITIES:
        selected_timezone = pytz.timezone(CITIES[city])
        selected_time = datetime.now(selected_timezone).strftime('%Y-%m-%d %H:%M:%S')
    else:
        city = 'Select a city'
        selected_time = ''

    return render_template_string(TEMPLATE, local_time=local_time, selected_city=city, selected_time=selected_time, cities=CITIES.keys())

if __name__ == '__main__':
    app.run(port=54321)