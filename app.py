from flask import Flask, request, make_response, render_template
from flask_httpauth import HTTPBasicAuth
from configparser import ConfigParser
import os
import sys

def get_essential_list():
    essential_discord_names = []
    for file in os.listdir("essentials"):
        essential_discord_names.append(file[10:].split('.exefs')[0])
    return essential_discord_names   

if not "config.ini" in os.listdir():
    print("Configuration doesn't exist")
    sys.exit(4) 

config = ConfigParser()
config.read("config.ini")
config_username = config["Access_Credentials"]["username"]
config_password = config["Access_Credentials"]["password"]
if config_password == "<password here>":
    print("Password not set (too dangerous)")
    sys.exit(4)

if not "essentials" in os.listdir():
    print("Create an \"essentials\" folder")
    sys.exit(4)

app = Flask(__name__)
auth = HTTPBasicAuth()

@auth.verify_password
def verify_password(username, password):
    if config_password == password and config_username == username:
        return username

@app.route('/submit', methods=['POST'])
def get_submission():
    print(f"essential.exefs submitted by {request.form['discordhandle']}")
    with open(f"essentials/essential_{request.form['discordhandle']}.exefs", "wb") as f:
        f.write(request.files['file'].read())

    return "<p>Hi</p>"

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/admin')
@auth.login_required
def admin():
    return render_template('admin.html', entries=get_essential_list())

@app.route('/retrieve', methods=['GET'])
@auth.login_required
def retrieve():
    name = request.args['username']
    print(f"{name} retrieved by {request.remote_addr}")
    with open(f"essentials/essential_{name}.exefs", "rb") as f:
        essential = f.read()
    response = make_response(essential)
    response.headers.set('Content-Type', 'application/octet-stream')
    response.headers.set('Content-Disposition', 'attachment', filename=f'essential_{name}.exefs')
    return response

@app.route('/delete')
@auth.login_required
def delete():
    name = request.args['username']
    print(f"{name} deleted by {request.remote_addr}")
    os.remove(f"essentials/essential_{name}.exefs")
    return "Deleted successfully. <a href=\"/admin\">Return to Admin Panel</a>"
