#!/opt/k11bot/env/bin/python -u
#coding: utf8

import os
from filehandling import (filehandling)
from flask import Flask, render_template, send_file


template_dir = os.path.abspath('/home/pi/webserver/root')
app = Flask(__name__,template_folder=template_dir)
fh = filehandling()

@app.route('/')
def index():
    return render_template('index.htm')

@app.route('/audio/<int:id>')
def show_post(id):
    # show the post with the given id, the id is an integer
    #return fh.getButtonFile(id)
    if not id in range(0,8):
        return "ERROR"

    return send_file(fh.getButtonFile(id));

if __name__ == '__main__':
    app.run(debug=False, host='0.0.0.0')



