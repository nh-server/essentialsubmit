from flask import Flask, request, make_response

app = Flask(__name__)

@app.route('/submit', methods=['POST'])
def get_submission():
    print(request.form['discordhandle'])
    with open("res.exefs", "wb") as f:
        f.write(request.files['file'].read())

    return "<p>Hi</p>"

