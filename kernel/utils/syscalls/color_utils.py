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
