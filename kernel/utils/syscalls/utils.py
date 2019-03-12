import sys

def find_block(text, pos, parentheses):
    opened = False
    balance = 0

    begin = pos
    for i in range(pos, len(text)):
        if text[i] == parentheses[0]:
            balance += 1
            if not opened:
                opened = True
                begin = i
        elif text[i] == parentheses[1]:
            balance -= 1
        if opened and balance == 0:
            return begin, i
        if balance < 0:
            raise ValueError
    raise ValueError


class Colors:
    @staticmethod
    def warn(text):
        return "\x1b[3;33m" + text + "\x1b[0m"

    @staticmethod
    def fail(text):
        return "\x1b[1;31m" + text + "\x1b[0m"

    @staticmethod
    def good(text):
        return "\x1b[1;32m" + text + "\x1b[0m"

    @staticmethod
    def italic(text):
        return "\x1b[3m" + text + "\x1b[0m"


def ask_yes_no(message, default="yes"):
    valid = {
        "yes": True,
        "ye": True,
        "y": True,
        "no": False,
        "n": False
    }

    if default is None:
        suffix = " [y/n]"
    elif default in valid and valid[default] == True:
        suffix = " [Y/n]"
    elif default in valid and valid[default] == False:
        suffix = " [N/y]"
    else:
        raise ValueError("invalid default answer: {0}".format(default))

    while True:
        sys.stdout.write(message + suffix)
        answer = input().lower()
        if answer == "" and default is not None:
            return valid[default]
        elif answer in valid:
            return valid[answer]
        else:
            pass
