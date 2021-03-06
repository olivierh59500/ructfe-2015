#!/usr/bin/env python3
# coding=utf-8
from random import randrange, choice, shuffle, randint, sample
from sys import argv, stderr
from os import path
from websocket import create_connection
from websocket._exceptions import WebSocketException
from socket import error as net_error
from json import loads, dumps
from csv import reader

__author__ = 'm_messiah'

OK, GET_ERROR, CORRUPT, FAIL, INTERNAL_ERROR = 101, 102, 103, 104, 110


def close(code, public="", private=""):
    if public:
        print(public)
    if private:
        print(private, file=stderr)
    exit(code)


class Client(object):
    def __init__(self, sock, username, password):
        self.username = username
        self.password = password
        self.ws = sock

    def register(self):
        """Registration"""
        self.ws.send(dumps(
            {'action': 'register',
             'params': {'username': self.username, 'password': self.password}}))
        answer = loads(self.ws.recv())
        if "successful" not in answer["text"]:
            close(CORRUPT, "Registration", "registration failed: %s" % answer)
        else:
            return answer['text'].split('uid=')[-1].split(')')[0]
        return answer

    def auth(self):
        """Authentication"""
        self.ws.send(dumps(
            {'action': 'auth',
             'params': {'username': self.username, 'password': self.password}}))
        answer = loads(self.ws.recv())
        if "text" not in answer or "Welcome" not in answer["text"]:
            if "id" not in answer:
                close(CORRUPT,
                      "Authentication", "authentication failed: %s" % answer)
            else:
                _ = self.ws.recv()

        if "rows" not in answer:
            answer = loads(self.ws.recv())
        # Show Profiles"""
        if ("data" not in answer["rows"][0] or
                len(answer["rows"][0]["data"]) < 5):
            close(CORRUPT, "Profiles", "showProfile failed: %s" % answer)
        return answer

    def crimes(self):
        """Last crimes"""
        self.ws.send(dumps({'action': 'show_crimes',
                            'params': {'offset': 0}}))
        answer = loads(self.ws.recv())
        while 'text' in answer and 'New crime' in answer['text']:
            answer = loads(self.ws.recv())
        
        if ("rows" not in answer or
                "data" not in answer["rows"][0] or
                len(answer["rows"][0]["data"]) < 2 or
                "crimeid" not in answer["rows"][0]["data"][0]):
            close(CORRUPT, "Crimes", "List crimes failed: %s" % answer)

        crimes = list(filter(lambda cr: cr['public'] == "fa-lock",
                              answer["rows"][0]["data"]))
        if crimes:
            self.ws.send(dumps({'action': 'show_crime',
                                'params': {
                                    'crimeid': choice(crimes)["crimeid"]
                                }}))
            answer = loads(self.ws.recv())
            while 'text' in answer and 'New crime' in answer['text']:
                answer = loads(self.ws.recv())

            if ("type" in answer and answer['type'] == "error" and
                    "text" in answer):
                if len(answer["text"]) > 26:
                    if "&nbsp;" not in answer["text"]:
                        close(CORRUPT, "Crimes",
                              "List auth crimes failed: %s" % answer)

        return answer

    def assign_profile(self):
        """Assign Profile"""
        self.ws.send(dumps(
            {'action': 'its_me',
             'params': {'profileid': "c452b3012f534f51a7c12981b82821ac",
                        'info': "1991-07-27"}}))
        answer = loads(self.ws.recv())
        while 'text' in answer and 'New crime' in answer['text']:
            answer = loads(self.ws.recv())
        if "successful" not in answer["text"]:
            close(CORRUPT, "Assignment", "assignment failed: %s" % answer)
        return answer

    def my_profile(self):
        """My profile"""
        self.assign_profile()
        self.ws.send(dumps({'action': 'show_my_profile'}))
        answer = loads(self.ws.recv())
        while 'text' in answer and 'New crime' in answer['text']:
            answer = loads(self.ws.recv())
        if ("rows" not in answer or
                "cols" not in answer["rows"][0] or
                'rows' not in answer['rows'][0]['cols'][0] or
                'hidden' not in answer['rows'][0]['cols'][0]['rows'][0] or
                not answer['rows'][0]['cols'][0]['rows'][0]['hidden']):
            close(CORRUPT, "My profile", "My profile failed: %s" % answer)
        return answer

    def report(self, flag_id=None, flag=None):
        """Report a crime
        :param flag:
        :param flag_id:
        """
        crimes = list(reader(open(
            path.join(path.dirname(path.realpath(__file__)), "crimes.csv"))))
        uids = list(open(path.join(path.dirname(path.realpath(__file__)),
                                   "uids.list")).readlines())
        tags, crimes = crimes[0], crimes[1:]
        crime = dict(zip(tags, choice(crimes)))
        if flag_id:
            crime['name'] = flag_id

        crime['private'] = True if flag_id else False

        if flag:
            crime['description'] += " " + str(flag)
        crime['closed'] = crime['closed'] == "true"
        crime['participants'] = ",".join(map(lambda u: u.strip(),
                                             sample(uids, randint(1, 5))))
        self.ws.send(dumps({'action': 'report', 'params': crime}))
        answer = loads(self.ws.recv())
        while 'text' in answer and 'New crime' in answer['text']:
            answer = loads(self.ws.recv())
        if "text" not in answer or "submited" not in answer["text"]:
            close(CORRUPT, "Report", "flag put failed: %s" % answer)
        return answer

    def show_report(self, flag_id, flag):
        """Show crime and check
        :param flag:
        :param flag_id:
        """
        self.ws.send(dumps({'action': 'search', 'params': {'text': flag_id}}))
        answer = loads(self.ws.recv())
        while 'text' in answer and 'New crime' in answer['text']:
            answer = loads(self.ws.recv())
        if ("rows" not in answer or
                len(answer['rows']) < 2 or
                'data' not in answer['rows'][1]):
            close(CORRUPT, "Search", "search failed: %s" % answer)

        if len(answer['rows'][1]['data']) < 1:
            close(GET_ERROR, "Search", "crime %s not searchable: %s" % (flag_id, answer))

        for crime in answer['rows'][1]['data']:
            self.ws.send(dumps({'action': 'show_crime',
                                'params': {'crimeid': crime['crimeid']}}))

            answer = loads(self.ws.recv())
            while 'text' in answer and 'New crime' in answer['text']:
                answer = loads(self.ws.recv())
            if ("rows" in answer and len(answer['rows']) and
                    'data' in answer['rows'][0] and
                    'description' in answer['rows'][0]['data'] and
                    'name' in answer['rows'][0]['data'] and
                    answer['rows'][0]['data']['name'] == flag_id and
                    flag ==
                    answer['rows'][0]['data']['description'].split()[-1]):
                close(OK)

        close(GET_ERROR)

    def search_uid(self, flag_id, flag):
        self.ws.send(dumps({'action': 'search', 'params': {'text': flag_id}}))
        answer = loads(self.ws.recv())
        while 'text' in answer and 'New crime' in answer['text']:
            answer = loads(self.ws.recv())
        if ("rows" not in answer or
                len(answer['rows']) < 3 or
                'data' not in answer['rows'][2]):
            close(CORRUPT, "Search", "search failed: %s" % answer)
        if len(answer['rows'][2]['data']) < 1:
            close(GET_ERROR, "Search", "user not searchable: %s" % answer)
        for user in answer['rows'][2]['data']:
            if user['answer'] == flag:
                close(OK)
        close(GET_ERROR)


def check(*args):
    addr = args[0]
    answer = ""
    if not addr:
        close(INTERNAL_ERROR, None, "Check without ADDR")
    try:
        ws = create_connection("ws://%s/websocket" % addr)
        ws.send(dumps({"action": "hello"}))
        answer = loads(ws.recv())
        if answer['rows'][0]['view'] != "form":
            raise KeyError
        c = Client(ws, '%x' % randrange(16**15), '%x' % randrange(16**15))
        c.register()
        c.auth()
        checkers = [c.crimes, c.my_profile, c.report]
        shuffle(checkers)
        for checker in checkers:
            checker()
        close(OK)
    except (net_error, WebSocketException):
        close(FAIL, "No connection to %s" % addr)
    except (KeyError, IndexError):
        close(CORRUPT, "JSON structure", "Bad answer in %s" % answer)
    finally:
        try:
            ws.close()
        except UnboundLocalError:
            pass


def put(*args):
    addr = args[0]
    flag_id = args[1]
    flag = args[2]
    vuln = int(args[3]) if len(args) > 3 else 1
    answer = ""
    if not addr or not flag_id or not flag:
        close(INTERNAL_ERROR, None, "Incorrect parameters")
    try:
        ws = create_connection("ws://%s/websocket" % addr)
        ws.send(dumps({"action": "hello"}))
        answer = loads(ws.recv())
        if answer['rows'][0]['view'] != "form":
            raise KeyError
        username, password = '%x' % randrange(16**15), '%x' % randrange(16**15)
        if vuln == 2:
            username = flag
        c = Client(ws, username, password)
        uid = c.register()
        c.auth()
        if vuln == 2:
            c.report()
            close(OK, "%s" % uid)
        else:
            flag_id = flag_id.replace("-", "").upper()
            c.report(flag_id, flag)
            close(OK, "%s:%s:%s" % (c.username, c.password, flag_id))
    except (net_error, WebSocketException):
        close(FAIL, "No connection to %s" % addr)
    except (KeyError, IndexError):
        close(CORRUPT, "JSON structure", "Bad answer in %s" % answer)
    finally:
        try:
            ws.close()
        except UnboundLocalError:
            pass


def get(*args):
    addr = args[0]
    checker_flag_id = args[1]
    flag = args[2]
    vuln = int(args[3]) if len(args) > 3 else 1
    answer = ""
    if not addr or not checker_flag_id or not flag:
        close(INTERNAL_ERROR, None, "Incorrect parameters")
    try:
        ws = create_connection("ws://%s/websocket" % addr)
        ws.send(dumps({"action": "hello"}))
        answer = loads(ws.recv())
        if answer['rows'][0]['view'] != "form":
            raise KeyError
        if vuln == 1:
            username, password, flag_id = checker_flag_id.split(":")
            c = Client(ws, username, password)
            c.auth()
            c.show_report(flag_id, flag)
        else:
            c = Client(ws, '%x' % randrange(16**15), '%x' % randrange(16**15))
            c.register()
            c.auth()
            c.search_uid(checker_flag_id, flag)

        close(OK)
    except (net_error, WebSocketException):
        close(FAIL, "No connection to %s" % addr)
    except (KeyError, IndexError):
        close(CORRUPT, "JSON structure", "Bad answer in %s" % answer)
    except ValueError:
        close(GET_ERROR, private="Incorrect vuln")
    finally:
        try:
            ws.close()
        except UnboundLocalError:
            pass


def info(*args):
    close(OK, "vulns: 3:1")


COMMANDS = {'check': check, 'put': put, 'get': get, 'info': info}


def not_found(*args):
    print("Unsupported command %s" % argv[1], file=stderr)
    return INTERNAL_ERROR


if __name__ == '__main__':
    try:
        COMMANDS.get(argv[1], not_found)(*argv[2:])
    except Exception as e:
        close(INTERNAL_ERROR, "Bad-ass checker", "INTERNAL ERROR: %s" % e)
