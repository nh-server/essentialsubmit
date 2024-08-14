from flask import Flask, request, make_response
from flask_httpauth import HTTPBasicAuth
from configparser import ConfigParser
import os

if not "config.ini" in os.listdir():
    print("Configuration doesn't exist")
    exit() 
config = ConfigParser()
config.read("config.ini")
config_username = config["Access_Credentials"]["username"]
config_password = config["Access_Credentials"]["password"]
if config_password == "<password here>":
    print("Password not set (too dangerous)")
    exit()

app = Flask(__name__)
auth = HTTPBasicAuth()

@auth.verify_password
def verify_password(username, password):
    if config_password == password and config_username == username:
        return username

@app.route('/submit', methods=['POST'])
def get_submission():
    print(request.form['discordhandle'])
    with open(f"essential_{request.form['discordhandle']}.exefs", "wb") as f:
        f.write(request.files['file'].read())

    return "<p>Hi</p>"

@app.route('/retrieve', methods=['GET'])
@auth.login_required
def retrieve():
    name = request.args['username']
    print(f"{username} is being retrieved")
    with open(f"essential_{username}.exefs", "rb") as f:
        essential = f.read()
    make_response(essential)
    response = make_response(movable)
    response.headers.set('Content-Type', 'application/octet-stream')
    response.headers.set('Content-Disposition', 'attachment', filename=f'essential_{name}.exefs')
    return response
