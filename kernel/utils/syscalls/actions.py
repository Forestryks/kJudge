from color_utils import Colors
import sys


def generic_forbid(source, path):
    return source


# def generic_forbid(source, path):
#     return source


def generic_omit(source, path):
    return source


def none(source, path):
    return source


def unspecified(source, path):
    # print(Colors.warn("".format(path)), file=sys.stderr)
    return source


actions = {
    "none": none,
    "forbid": generic_forbid,
    "omit": generic_omit,
    "unspecified": unspecified
}
