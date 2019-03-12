from utils import Colors, find_block
import sys


def generic_forbid(source, path):
    return source


def generic_omit(source, path):
    return source


def none(source, path):
    return source


def unspecified(source, path):
    # TODO: show appropriate warning instead of doing this shit
    # print(Colors.warn("".format(path)), file=sys.stderr)
    print("Unspecified action on {0}".format(path))

    args_begin, args_end = find_block(source, 0, "()")
    code_begin, code_end = find_block(source, 0, "{}")
    args = list(map(lambda x: x.strip(), source[args_begin + 1:args_end].split(',')))
    name = args[0]

    endl = source.index("\n", code_begin + 1)
    pref = ""
    for i in range(endl + 1, len(source)):
        if source[i] == "\t" or source[i] == " ":
            pref += source[i]
        else:
            break

    source = "{0}\n{1}{2}{3}".format(
        source[:endl],
        pref,
        """if (KJ_IN_SAFEMODE()) printk("Process [%d] acquired syscall {0}\\n", current->pid);""".format(name),
        source[endl:]
    )

    return source


actions = {
    "none": none,
    "forbid": generic_forbid,
    "omit": generic_omit,
    "unspecified": unspecified
}
