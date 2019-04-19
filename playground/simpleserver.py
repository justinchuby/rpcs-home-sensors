from flask import Flask, request
import sys

app = Flask(__name__)


@app.route('/hs', methods=('POST', 'GET'))
def main():
    if request.method == 'GET':
        return 'You GOT this'
    print(request.data)
    print(request.method, request.get_json(), file=sys.stdout)
    return ''


if __name__ == '__main__':
    app.run(
        host='0.0.0.0',
        debug=True
    )
