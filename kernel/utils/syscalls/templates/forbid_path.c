if (KJ_IN_SAFEMODE()) {
    int res = kj_user_secure_path({{ path_var }});
    if (res != 0) {
        {% if strict is defined %}if (res == -EPERM) {
            current->kj_flags |= KJ_WAS_SV;
            current->kj_violation_id = {{ id }};
            printk(KERN_WARNING "Process [%d] acquired syscall {{ name }} with restricted path\n", current->pid);
        }
        {% endif %}return res;
    }
}
