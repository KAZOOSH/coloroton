#!/opt/k11bot/env/bin/python -u
#coding: utf8

import os
import alsaaudio
from flask import Flask, render_template, send_file

mypath = os.path.dirname(os.path.realpath(__file__))
template_dir = os.path.realpath(mypath + '/template')
static_folder = os.path.realpath(mypath + '/resroot')
app = Flask(__name__,template_folder=template_dir,static_url_path='/res',static_folder=static_folder)

@app.route('/')
def index():
	return send_file('template/index.htm');
	#return render_template('index.htm')

@app.route('/audiolevel/<int:id>')
def sound(id):
	m = alsaaudio.Mixer()
	m.setvolume(id)
    	return 'OK' 

@app.after_request
def add_header(r):
    """
    Add headers to both force latest IE rendering engine or Chrome Frame,
    and also to cache the rendered page for 10 minutes.
    """
    r.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    r.headers["Pragma"] = "no-cache"
    r.headers["Expires"] = "0"
    r.headers['Cache-Control'] = 'public, max-age=0'
    return r



if __name__ == '__main__':
    app.run(debug=False, host='0.0.0.0')



