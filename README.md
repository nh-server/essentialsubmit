# buildin

to build this goddamn thingie, run make SERVER_ADDRESS=https://... \
e.g. make SERVER_ADDRESS=https://3ds.web.page/submit (this is not an actual valid url i hope)

app.py is a super small flask app that listens for post request on /submit, saves essential as essential_\<discordname\>.exefs and prints the discord username to stdout. \
use gunicorn, e.g. gunicorn -w 1 app:app. I use venv to set stuff up: \
python3 -m venv .venv \
. .venv/bin/activate \
pip install flask gunicorn \
gunicorn -w 1 app:app