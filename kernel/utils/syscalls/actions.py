from utils import Colors, find_block


def insert_begin(source, lines):
    code_begin, code_end = find_block(source, 0, "{}")
    args_begin, args_end = find_block(source, 0, "()")
    args = list(map(lambda x: x.strip(), source[args_begin + 1:args_end].split(',')))
    name = args[0]
    endl = source.index("\n", code_begin + 1)
    pref = "\n"
    for i in range(endl + 1, len(source)):
        if source[i] == "\t" or source[i] == " ":
            pref += source[i]
        else:
            break

    string = ''.join([pref + line.format(
        id=str(1),
        name=name
    ) for line in lines.split("\n")])

    source = "{0}{1}{2}".format(
        source[:endl],
        string,
        source[endl:]
    )
    return source


def generic_forbid(source, path):
    lines = """if (KJ_IN_SAFEMODE()) {{
    current->kj_flags |= KJ_WAS_SV;
    current->kj_violation_id = {id};
    printk(KERN_ALERT "Process [%d] acquired restricted syscall {name}", current->pid);
    return -EPERM;
}}"""
    return insert_begin(source, lines)


def generic_omit(source, path):
    lines = """if (KJ_IN_SAFEMODE()) {{
    return -EPERM;
}}"""
    return insert_begin(source, lines)


def none(source, path):
    return source


def unspecified(source, path):
    args_begin, args_end = find_block(source, 0, "()")
    args = list(map(lambda x: x.strip(), source[args_begin + 1:args_end].split(',')))
    name = args[0]
    print(Colors.warn("Unspecified action on {0}:{1}".format(path, name)))
    return source


def unknown(source, path):
    return source


def testing(source, path):
    lines = """if (KJ_IN_SAFEMODE()) {{
    if (!kj_user_secure_path(path)) {{
        printk(KERN_ALERT "Process [%d] acquired syscall {name} with restricted path", current->pid);
        return -EPERM;
    }}
    printk(KERN_ALERT "Process [%d] acquired syscall {name} with allowed path", current->pid);
}}"""
    return insert_begin(source, lines)


actions = {
    "none": none,
    "forbid": generic_forbid,
    "omit": generic_omit,
    "unspecified": unspecified,
    "unknown": unknown,
    "testing": testing
}
