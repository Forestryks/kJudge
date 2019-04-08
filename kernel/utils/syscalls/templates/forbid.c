if (KJ_IN_SAFEMODE()) {
    current->kj_flags |= KJ_WAS_SV;
    current->kj_violation_id = {{ id }};
    printk(KERN_WARNING "Process [%d] acquired restricted syscall {{ name }}\n", current->pid);
    return -EPERM;
}
