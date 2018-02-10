#!/opt/k11bot/env/bin/python -u
#coding: utf8

import os
import alsaaudio
from flask import Flask, render_template, send_file

mypath = os.path.dirname(os.path.realpath(__file__))
template_dir = os.path.realpath(mypath + '/wwwroot')
static_folder = os.path.realpath(mypath + '/wwwroot')
app = Flask(__name__,template_folder=template_dir,static_url_path='/res',static_folder=static_folder)

@app.route('/')
def index():
    return render_template('index.htm')

@app.route('/audio/<int:id>')
def sound(id):
	m = alsaaudio.Mixer()
	m.setvolume(id)
    	return 'OK' 

@app.route('/res/<path:path>')
def send_js(path):
    return send_from_directory('js', path)


if __name__ == '__main__':
    app.run(debug=False, host='0.0.0.0')



