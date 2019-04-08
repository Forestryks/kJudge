from utils import Colors, find_block, render_template


def insert_begin(source, template, **kwargs):
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

    string = render_template(
        template,
        id=1,
        name=name,
        **kwargs
    )
    string = ''.join([pref + line for line in string.split("\n")])

    source = "{0}{1}{2}".format(
        source[:endl],
        string,
        source[endl:]
    )
    return source


def generic_forbid(source, path):
    return insert_begin(source, "templates/forbid.c")


def generic_omit(source, path):
    return insert_begin(source, "templates/omit.c")


def none(source, path):
    return source


def unspecified(source, path):
    args_begin, args_end = find_block(source, 0, "()")
    args = list(map(lambda x: x.strip(), source[args_begin + 1:args_end].split(',')))
    name = args[0]
    print(Colors.warn("Unspecified action on {0}:{1}".format(path, name)))
    return source


def forbid_path(source, path, path_var, mode="soft"):
    if mode == "strict":
        return insert_begin(source, "templates/forbid_path.c", path_var=path_var, strict="true")
    return insert_begin(source, "templates/forbid_path.c", path_var=path_var)


def forbid_path_at(source, path, dfd_var, path_var, mode="soft"):
    if mode == "strict":
        return insert_begin(source, "templates/forbid_path_at.c", dfd_var=dfd_var, path_var=path_var, strict="true")
    return insert_begin(source, "templates/forbid_path_at.c", dfd_var=dfd_var, path_var=path_var)


actions = {
    "none": none,
    "forbid": generic_forbid,
    "omit": generic_omit,
    "unspecified": unspecified,
    "forbid_path": forbid_path,
    "forbid_path_at": forbid_path_at
}
