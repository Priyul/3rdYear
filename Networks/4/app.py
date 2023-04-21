from flask import Flask, render_template, request, redirect, url_for
import os
import datetime

app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = './static/'
app.config['MAX_CONTENT_LENGTH'] = 16 * 1024 * 1024


FILE_PATH = 'appointments.txt'


def read_appointments():
    appointments = []
    with open(FILE_PATH, 'r') as f:
        for line in f:
            appointment = line.strip().split(',')
            appointments.append(appointment)
    return appointments


def add_appointment(date, time, description, filename):
    with open(FILE_PATH, 'a') as f:
        f.write(f'{date},{time},{description},{filename}\n')


def delete_appointment(index):
    appointments = read_appointments()
    appointments.pop(index)
    with open(FILE_PATH, 'w') as f:
        for appointment in appointments:
            if len(appointment) >= 3:
                f.write(f'{appointment[0]},{appointment[1]},{appointment[2]}')
                if len(appointment) == 4:
                    f.write(f',{appointment[3]}')
                f.write('\n')


@app.route('/')
def index():
    appointments = read_appointments()
    return render_template('index.html', appointments=appointments)

@app.route('/add', methods=['POST'])
def add():
    date = request.form['date']
    time = request.form['time']
    description = request.form['description']
    file = request.files['file']
    if file:
        filename = datetime.datetime.now().strftime("%Y%m%d-%H%M%S") + '_' + file.filename
        file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
    else:
        filename = ''
    add_appointment(date, time, description, filename)
    return redirect(url_for('index'))

@app.route('/delete/<int:index>', methods=['POST'])
def delete(index):
    delete_appointment(index)
    return redirect(url_for('index'))

@app.route('/search')
def search():
    query = request.args.get('query')
    appointments = read_appointments()
    search_results = []
    for appointment in appointments:
        if query in appointment[0] or query in appointment[1] or query in appointment[2]:
            search_results.append(appointment)
    if len(search_results) == 0:
        message = "No appointment found"
    else:
        message = f"{len(search_results)} appointment(s) found"
    return render_template('index.html', appointments=search_results, message=message)



if __name__ == '__main__':
    app.run(debug=True)

