# buildin

to build this goddamn thingie, run make SERVER_ADDRESS=https://... \
e.g. make SERVER_ADDRESS=https://3ds.web.page/submit (this is not an actual valid url i hope)

# hostin

app.py is a super small flask app that listens for post request on /submit, saves essential as essential_\<discordname\>.exefs and prints the discord username to stdout. \
to retrieve an essential, you can use the retrieve api, which requires a password \
you can set that password in config.ini \
an example is provided, but needs to be renamed to config.ini obv \
use gunicorn, e.g. gunicorn -w 1 app:app. I use venv to set stuff up: \
python3 -m venv .venv \
. .venv/bin/activate \
pip install flask gunicorn flask_httpauth \
gunicorn -w 1 app:app

# credit

gruetzig made this, eip forked it and is providing access to it via nintendohomebrew resources
