#include "syscall_names.h"
#include <sys/syscall.h>

const char *syscall_name(long sysno) {
    switch (sysno) {
#ifdef SYS_FAST_atomic_update
    case SYS_FAST_atomic_update: return "FAST_atomic_update";
#endif

#ifdef SYS_FAST_cmpxchg
    case SYS_FAST_cmpxchg: return "FAST_cmpxchg";
#endif

#ifdef SYS_FAST_cmpxchg64
    case SYS_FAST_cmpxchg64: return "FAST_cmpxchg64";
#endif

#ifdef SYS__llseek
    case SYS__llseek: return "_llseek";
#endif

#ifdef SYS__newselect
    case SYS__newselect: return "_newselect";
#endif

#ifdef SYS__sysctl
    case SYS__sysctl: return "_sysctl";
#endif

#ifdef SYS_accept
    case SYS_accept: return "accept";
#endif

#ifdef SYS_accept4
    case SYS_accept4: return "accept4";
#endif

#ifdef SYS_access
    case SYS_access: return "access";
#endif

#ifdef SYS_acct
    case SYS_acct: return "acct";
#endif

#ifdef SYS_acl_get
    case SYS_acl_get: return "acl_get";
#endif

#ifdef SYS_acl_set
    case SYS_acl_set: return "acl_set";
#endif

#ifdef SYS_add_key
    case SYS_add_key: return "add_key";
#endif

#ifdef SYS_adjtimex
    case SYS_adjtimex: return "adjtimex";
#endif

#ifdef SYS_afs_syscall
    case SYS_afs_syscall: return "afs_syscall";
#endif

#ifdef SYS_alarm
    case SYS_alarm: return "alarm";
#endif

#ifdef SYS_alloc_hugepages
    case SYS_alloc_hugepages: return "alloc_hugepages";
#endif

#ifdef SYS_arch_prctl
    case SYS_arch_prctl: return "arch_prctl";
#endif

#ifdef SYS_arm_fadvise64_64
    case SYS_arm_fadvise64_64: return "arm_fadvise64_64";
#endif

#ifdef SYS_arm_sync_file_range
    case SYS_arm_sync_file_range: return "arm_sync_file_range";
#endif

#ifdef SYS_atomic_barrier
    case SYS_atomic_barrier: return "atomic_barrier";
#endif

#ifdef SYS_atomic_cmpxchg_32
    case SYS_atomic_cmpxchg_32: return "atomic_cmpxchg_32";
#endif

#ifdef SYS_attrctl
    case SYS_attrctl: return "attrctl";
#endif

#ifdef SYS_bdflush
    case SYS_bdflush: return "bdflush";
#endif

#ifdef SYS_bind
    case SYS_bind: return "bind";
#endif

#ifdef SYS_bpf
    case SYS_bpf: return "bpf";
#endif

#ifdef SYS_break
    case SYS_break: return "break";
#endif

#ifdef SYS_breakpoint
    case SYS_breakpoint: return "breakpoint";
#endif

#ifdef SYS_brk
    case SYS_brk: return "brk";
#endif

#ifdef SYS_cachectl
    case SYS_cachectl: return "cachectl";
#endif

#ifdef SYS_cacheflush
    case SYS_cacheflush: return "cacheflush";
#endif

#ifdef SYS_capget
    case SYS_capget: return "capget";
#endif

#ifdef SYS_capset
    case SYS_capset: return "capset";
#endif

#ifdef SYS_chdir
    case SYS_chdir: return "chdir";
#endif

#ifdef SYS_chmod
    case SYS_chmod: return "chmod";
#endif

#ifdef SYS_chown
    case SYS_chown: return "chown";
#endif

#ifdef SYS_chown32
    case SYS_chown32: return "chown32";
#endif

#ifdef SYS_chroot
    case SYS_chroot: return "chroot";
#endif

#ifdef SYS_clock_adjtime
    case SYS_clock_adjtime: return "clock_adjtime";
#endif

#ifdef SYS_clock_adjtime64
    case SYS_clock_adjtime64: return "clock_adjtime64";
#endif

#ifdef SYS_clock_getres
    case SYS_clock_getres: return "clock_getres";
#endif

#ifdef SYS_clock_getres_time64
    case SYS_clock_getres_time64: return "clock_getres_time64";
#endif

#ifdef SYS_clock_gettime
    case SYS_clock_gettime: return "clock_gettime";
#endif

#ifdef SYS_clock_gettime64
    case SYS_clock_gettime64: return "clock_gettime64";
#endif

#ifdef SYS_clock_nanosleep
    case SYS_clock_nanosleep: return "clock_nanosleep";
#endif

#ifdef SYS_clock_nanosleep_time64
    case SYS_clock_nanosleep_time64: return "clock_nanosleep_time64";
#endif

#ifdef SYS_clock_settime
    case SYS_clock_settime: return "clock_settime";
#endif

#ifdef SYS_clock_settime64
    case SYS_clock_settime64: return "clock_settime64";
#endif

#ifdef SYS_clone
    case SYS_clone: return "clone";
#endif

#ifdef SYS_clone2
    case SYS_clone2: return "clone2";
#endif

#ifdef SYS_clone3
    case SYS_clone3: return "clone3";
#endif

#ifdef SYS_close
    case SYS_close: return "close";
#endif

#ifdef SYS_cmpxchg_badaddr
    case SYS_cmpxchg_badaddr: return "cmpxchg_badaddr";
#endif

#ifdef SYS_connect
    case SYS_connect: return "connect";
#endif

#ifdef SYS_copy_file_range
    case SYS_copy_file_range: return "copy_file_range";
#endif

#ifdef SYS_creat
    case SYS_creat: return "creat";
#endif

#ifdef SYS_create_module
    case SYS_create_module: return "create_module";
#endif

#ifdef SYS_delete_module
    case SYS_delete_module: return "delete_module";
#endif

#ifdef SYS_dipc
    case SYS_dipc: return "dipc";
#endif

#ifdef SYS_dup
    case SYS_dup: return "dup";
#endif

#ifdef SYS_dup2
    case SYS_dup2: return "dup2";
#endif

#ifdef SYS_dup3
    case SYS_dup3: return "dup3";
#endif

#ifdef SYS_epoll_create
    case SYS_epoll_create: return "epoll_create";
#endif

#ifdef SYS_epoll_create1
    case SYS_epoll_create1: return "epoll_create1";
#endif

#ifdef SYS_epoll_ctl
    case SYS_epoll_ctl: return "epoll_ctl";
#endif

#ifdef SYS_epoll_ctl_old
    case SYS_epoll_ctl_old: return "epoll_ctl_old";
#endif

#ifdef SYS_epoll_pwait
    case SYS_epoll_pwait: return "epoll_pwait";
#endif

#ifdef SYS_epoll_wait
    case SYS_epoll_wait: return "epoll_wait";
#endif

#ifdef SYS_epoll_wait_old
    case SYS_epoll_wait_old: return "epoll_wait_old";
#endif

#ifdef SYS_eventfd
    case SYS_eventfd: return "eventfd";
#endif

#ifdef SYS_eventfd2
    case SYS_eventfd2: return "eventfd2";
#endif

#ifdef SYS_exec_with_loader
    case SYS_exec_with_loader: return "exec_with_loader";
#endif

#ifdef SYS_execv
    case SYS_execv: return "execv";
#endif

#ifdef SYS_execve
    case SYS_execve: return "execve";
#endif

#ifdef SYS_execveat
    case SYS_execveat: return "execveat";
#endif

#ifdef SYS_exit
    case SYS_exit: return "exit";
#endif

#ifdef SYS_exit_group
    case SYS_exit_group: return "exit_group";
#endif

#ifdef SYS_faccessat
    case SYS_faccessat: return "faccessat";
#endif

#ifdef SYS_fadvise64
    case SYS_fadvise64: return "fadvise64";
#endif

#ifdef SYS_fadvise64_64
    case SYS_fadvise64_64: return "fadvise64_64";
#endif

#ifdef SYS_fallocate
    case SYS_fallocate: return "fallocate";
#endif

#ifdef SYS_fanotify_init
    case SYS_fanotify_init: return "fanotify_init";
#endif

#ifdef SYS_fanotify_mark
    case SYS_fanotify_mark: return "fanotify_mark";
#endif

#ifdef SYS_fchdir
    case SYS_fchdir: return "fchdir";
#endif

#ifdef SYS_fchmod
    case SYS_fchmod: return "fchmod";
#endif

#ifdef SYS_fchmodat
    case SYS_fchmodat: return "fchmodat";
#endif

#ifdef SYS_fchown
    case SYS_fchown: return "fchown";
#endif

#ifdef SYS_fchown32
    case SYS_fchown32: return "fchown32";
#endif

#ifdef SYS_fchownat
    case SYS_fchownat: return "fchownat";
#endif

#ifdef SYS_fcntl
    case SYS_fcntl: return "fcntl";
#endif

#ifdef SYS_fcntl64
    case SYS_fcntl64: return "fcntl64";
#endif

#ifdef SYS_fdatasync
    case SYS_fdatasync: return "fdatasync";
#endif

#ifdef SYS_fgetxattr
    case SYS_fgetxattr: return "fgetxattr";
#endif

#ifdef SYS_finit_module
    case SYS_finit_module: return "finit_module";
#endif

#ifdef SYS_flistxattr
    case SYS_flistxattr: return "flistxattr";
#endif

#ifdef SYS_flock
    case SYS_flock: return "flock";
#endif

#ifdef SYS_fork
    case SYS_fork: return "fork";
#endif

#ifdef SYS_fp_udfiex_crtl
    case SYS_fp_udfiex_crtl: return "fp_udfiex_crtl";
#endif

#ifdef SYS_free_hugepages
    case SYS_free_hugepages: return "free_hugepages";
#endif

#ifdef SYS_fremovexattr
    case SYS_fremovexattr: return "fremovexattr";
#endif

#ifdef SYS_fsconfig
    case SYS_fsconfig: return "fsconfig";
#endif

#ifdef SYS_fsetxattr
    case SYS_fsetxattr: return "fsetxattr";
#endif

#ifdef SYS_fsmount
    case SYS_fsmount: return "fsmount";
#endif

#ifdef SYS_fsopen
    case SYS_fsopen: return "fsopen";
#endif

#ifdef SYS_fspick
    case SYS_fspick: return "fspick";
#endif

#ifdef SYS_fstat
    case SYS_fstat: return "fstat";
#endif

#ifdef SYS_fstat64
    case SYS_fstat64: return "fstat64";
#endif

#ifdef SYS_fstatat64
    case SYS_fstatat64: return "fstatat64";
#endif

#ifdef SYS_fstatfs
    case SYS_fstatfs: return "fstatfs";
#endif

#ifdef SYS_fstatfs64
    case SYS_fstatfs64: return "fstatfs64";
#endif

#ifdef SYS_fsync
    case SYS_fsync: return "fsync";
#endif

#ifdef SYS_ftime
    case SYS_ftime: return "ftime";
#endif

#ifdef SYS_ftruncate
    case SYS_ftruncate: return "ftruncate";
#endif

#ifdef SYS_ftruncate64
    case SYS_ftruncate64: return "ftruncate64";
#endif

#ifdef SYS_futex
    case SYS_futex: return "futex";
#endif

#ifdef SYS_futex_time64
    case SYS_futex_time64: return "futex_time64";
#endif

#ifdef SYS_futimesat
    case SYS_futimesat: return "futimesat";
#endif

#ifdef SYS_get_kernel_syms
    case SYS_get_kernel_syms: return "get_kernel_syms";
#endif

#ifdef SYS_get_mempolicy
    case SYS_get_mempolicy: return "get_mempolicy";
#endif

#ifdef SYS_get_robust_list
    case SYS_get_robust_list: return "get_robust_list";
#endif

#ifdef SYS_get_thread_area
    case SYS_get_thread_area: return "get_thread_area";
#endif

#ifdef SYS_get_tls
    case SYS_get_tls: return "get_tls";
#endif

#ifdef SYS_getcpu
    case SYS_getcpu: return "getcpu";
#endif

#ifdef SYS_getcwd
    case SYS_getcwd: return "getcwd";
#endif

#ifdef SYS_getdents
    case SYS_getdents: return "getdents";
#endif

#ifdef SYS_getdents64
    case SYS_getdents64: return "getdents64";
#endif

#ifdef SYS_getdomainname
    case SYS_getdomainname: return "getdomainname";
#endif

#ifdef SYS_getdtablesize
    case SYS_getdtablesize: return "getdtablesize";
#endif

#ifdef SYS_getegid
    case SYS_getegid: return "getegid";
#endif

#ifdef SYS_getegid32
    case SYS_getegid32: return "getegid32";
#endif

#ifdef SYS_geteuid
    case SYS_geteuid: return "geteuid";
#endif

#ifdef SYS_geteuid32
    case SYS_geteuid32: return "geteuid32";
#endif

#ifdef SYS_getgid
    case SYS_getgid: return "getgid";
#endif

#ifdef SYS_getgid32
    case SYS_getgid32: return "getgid32";
#endif

#ifdef SYS_getgroups
    case SYS_getgroups: return "getgroups";
#endif

#ifdef SYS_getgroups32
    case SYS_getgroups32: return "getgroups32";
#endif

#ifdef SYS_gethostname
    case SYS_gethostname: return "gethostname";
#endif

#ifdef SYS_getitimer
    case SYS_getitimer: return "getitimer";
#endif

#ifdef SYS_getpagesize
    case SYS_getpagesize: return "getpagesize";
#endif

#ifdef SYS_getpeername
    case SYS_getpeername: return "getpeername";
#endif

#ifdef SYS_getpgid
    case SYS_getpgid: return "getpgid";
#endif

#ifdef SYS_getpgrp
    case SYS_getpgrp: return "getpgrp";
#endif

#ifdef SYS_getpid
    case SYS_getpid: return "getpid";
#endif

#ifdef SYS_getpmsg
    case SYS_getpmsg: return "getpmsg";
#endif

#ifdef SYS_getppid
    case SYS_getppid: return "getppid";
#endif

#ifdef SYS_getpriority
    case SYS_getpriority: return "getpriority";
#endif

#ifdef SYS_getrandom
    case SYS_getrandom: return "getrandom";
#endif

#ifdef SYS_getresgid
    case SYS_getresgid: return "getresgid";
#endif

#ifdef SYS_getresgid32
    case SYS_getresgid32: return "getresgid32";
#endif

#ifdef SYS_getresuid
    case SYS_getresuid: return "getresuid";
#endif

#ifdef SYS_getresuid32
    case SYS_getresuid32: return "getresuid32";
#endif

#ifdef SYS_getrlimit
    case SYS_getrlimit: return "getrlimit";
#endif

#ifdef SYS_getrusage
    case SYS_getrusage: return "getrusage";
#endif

#ifdef SYS_getsid
    case SYS_getsid: return "getsid";
#endif

#ifdef SYS_getsockname
    case SYS_getsockname: return "getsockname";
#endif

#ifdef SYS_getsockopt
    case SYS_getsockopt: return "getsockopt";
#endif

#ifdef SYS_gettid
    case SYS_gettid: return "gettid";
#endif

#ifdef SYS_gettimeofday
    case SYS_gettimeofday: return "gettimeofday";
#endif

#ifdef SYS_getuid
    case SYS_getuid: return "getuid";
#endif

#ifdef SYS_getuid32
    case SYS_getuid32: return "getuid32";
#endif

#ifdef SYS_getunwind
    case SYS_getunwind: return "getunwind";
#endif

#ifdef SYS_getxattr
    case SYS_getxattr: return "getxattr";
#endif

#ifdef SYS_getxgid
    case SYS_getxgid: return "getxgid";
#endif

#ifdef SYS_getxpid
    case SYS_getxpid: return "getxpid";
#endif

#ifdef SYS_getxuid
    case SYS_getxuid: return "getxuid";
#endif

#ifdef SYS_gtty
    case SYS_gtty: return "gtty";
#endif

#ifdef SYS_idle
    case SYS_idle: return "idle";
#endif

#ifdef SYS_init_module
    case SYS_init_module: return "init_module";
#endif

#ifdef SYS_inotify_add_watch
    case SYS_inotify_add_watch: return "inotify_add_watch";
#endif

#ifdef SYS_inotify_init
    case SYS_inotify_init: return "inotify_init";
#endif

#ifdef SYS_inotify_init1
    case SYS_inotify_init1: return "inotify_init1";
#endif

#ifdef SYS_inotify_rm_watch
    case SYS_inotify_rm_watch: return "inotify_rm_watch";
#endif

#ifdef SYS_io_cancel
    case SYS_io_cancel: return "io_cancel";
#endif

#ifdef SYS_io_destroy
    case SYS_io_destroy: return "io_destroy";
#endif

#ifdef SYS_io_getevents
    case SYS_io_getevents: return "io_getevents";
#endif

#ifdef SYS_io_pgetevents
    case SYS_io_pgetevents: return "io_pgetevents";
#endif

#ifdef SYS_io_pgetevents_time64
    case SYS_io_pgetevents_time64: return "io_pgetevents_time64";
#endif

#ifdef SYS_io_setup
    case SYS_io_setup: return "io_setup";
#endif

#ifdef SYS_io_submit
    case SYS_io_submit: return "io_submit";
#endif

#ifdef SYS_io_uring_enter
    case SYS_io_uring_enter: return "io_uring_enter";
#endif

#ifdef SYS_io_uring_register
    case SYS_io_uring_register: return "io_uring_register";
#endif

#ifdef SYS_io_uring_setup
    case SYS_io_uring_setup: return "io_uring_setup";
#endif

#ifdef SYS_ioctl
    case SYS_ioctl: return "ioctl";
#endif

#ifdef SYS_ioperm
    case SYS_ioperm: return "ioperm";
#endif

#ifdef SYS_iopl
    case SYS_iopl: return "iopl";
#endif

#ifdef SYS_ioprio_get
    case SYS_ioprio_get: return "ioprio_get";
#endif

#ifdef SYS_ioprio_set
    case SYS_ioprio_set: return "ioprio_set";
#endif

#ifdef SYS_ipc
    case SYS_ipc: return "ipc";
#endif

#ifdef SYS_kcmp
    case SYS_kcmp: return "kcmp";
#endif

#ifdef SYS_kern_features
    case SYS_kern_features: return "kern_features";
#endif

#ifdef SYS_kexec_file_load
    case SYS_kexec_file_load: return "kexec_file_load";
#endif

#ifdef SYS_kexec_load
    case SYS_kexec_load: return "kexec_load";
#endif

#ifdef SYS_keyctl
    case SYS_keyctl: return "keyctl";
#endif

#ifdef SYS_kill
    case SYS_kill: return "kill";
#endif

#ifdef SYS_lchown
    case SYS_lchown: return "lchown";
#endif

#ifdef SYS_lchown32
    case SYS_lchown32: return "lchown32";
#endif

#ifdef SYS_lgetxattr
    case SYS_lgetxattr: return "lgetxattr";
#endif

#ifdef SYS_link
    case SYS_link: return "link";
#endif

#ifdef SYS_linkat
    case SYS_linkat: return "linkat";
#endif

#ifdef SYS_listen
    case SYS_listen: return "listen";
#endif

#ifdef SYS_listxattr
    case SYS_listxattr: return "listxattr";
#endif

#ifdef SYS_llistxattr
    case SYS_llistxattr: return "llistxattr";
#endif

#ifdef SYS_llseek
    case SYS_llseek: return "llseek";
#endif

#ifdef SYS_lock
    case SYS_lock: return "lock";
#endif

#ifdef SYS_lookup_dcookie
    case SYS_lookup_dcookie: return "lookup_dcookie";
#endif

#ifdef SYS_lremovexattr
    case SYS_lremovexattr: return "lremovexattr";
#endif

#ifdef SYS_lseek
    case SYS_lseek: return "lseek";
#endif

#ifdef SYS_lsetxattr
    case SYS_lsetxattr: return "lsetxattr";
#endif

#ifdef SYS_lstat
    case SYS_lstat: return "lstat";
#endif

#ifdef SYS_lstat64
    case SYS_lstat64: return "lstat64";
#endif

#ifdef SYS_madvise
    case SYS_madvise: return "madvise";
#endif

#ifdef SYS_mbind
    case SYS_mbind: return "mbind";
#endif

#ifdef SYS_membarrier
    case SYS_membarrier: return "membarrier";
#endif

#ifdef SYS_memfd_create
    case SYS_memfd_create: return "memfd_create";
#endif

#ifdef SYS_memory_ordering
    case SYS_memory_ordering: return "memory_ordering";
#endif

#ifdef SYS_migrate_pages
    case SYS_migrate_pages: return "migrate_pages";
#endif

#ifdef SYS_mincore
    case SYS_mincore: return "mincore";
#endif

#ifdef SYS_mkdir
    case SYS_mkdir: return "mkdir";
#endif

#ifdef SYS_mkdirat
    case SYS_mkdirat: return "mkdirat";
#endif

#ifdef SYS_mknod
    case SYS_mknod: return "mknod";
#endif

#ifdef SYS_mknodat
    case SYS_mknodat: return "mknodat";
#endif

#ifdef SYS_mlock
    case SYS_mlock: return "mlock";
#endif

#ifdef SYS_mlock2
    case SYS_mlock2: return "mlock2";
#endif

#ifdef SYS_mlockall
    case SYS_mlockall: return "mlockall";
#endif

#ifdef SYS_mmap
    case SYS_mmap: return "mmap";
#endif

#ifdef SYS_mmap2
    case SYS_mmap2: return "mmap2";
#endif

#ifdef SYS_modify_ldt
    case SYS_modify_ldt: return "modify_ldt";
#endif

#ifdef SYS_mount
    case SYS_mount: return "mount";
#endif

#ifdef SYS_move_mount
    case SYS_move_mount: return "move_mount";
#endif

#ifdef SYS_move_pages
    case SYS_move_pages: return "move_pages";
#endif

#ifdef SYS_mprotect
    case SYS_mprotect: return "mprotect";
#endif

#ifdef SYS_mpx
    case SYS_mpx: return "mpx";
#endif

#ifdef SYS_mq_getsetattr
    case SYS_mq_getsetattr: return "mq_getsetattr";
#endif

#ifdef SYS_mq_notify
    case SYS_mq_notify: return "mq_notify";
#endif

#ifdef SYS_mq_open
    case SYS_mq_open: return "mq_open";
#endif

#ifdef SYS_mq_timedreceive
    case SYS_mq_timedreceive: return "mq_timedreceive";
#endif

#ifdef SYS_mq_timedreceive_time64
    case SYS_mq_timedreceive_time64: return "mq_timedreceive_time64";
#endif

#ifdef SYS_mq_timedsend
    case SYS_mq_timedsend: return "mq_timedsend";
#endif

#ifdef SYS_mq_timedsend_time64
    case SYS_mq_timedsend_time64: return "mq_timedsend_time64";
#endif

#ifdef SYS_mq_unlink
    case SYS_mq_unlink: return "mq_unlink";
#endif

#ifdef SYS_mremap
    case SYS_mremap: return "mremap";
#endif

#ifdef SYS_msgctl
    case SYS_msgctl: return "msgctl";
#endif

#ifdef SYS_msgget
    case SYS_msgget: return "msgget";
#endif

#ifdef SYS_msgrcv
    case SYS_msgrcv: return "msgrcv";
#endif

#ifdef SYS_msgsnd
    case SYS_msgsnd: return "msgsnd";
#endif

#ifdef SYS_msync
    case SYS_msync: return "msync";
#endif

#ifdef SYS_multiplexer
    case SYS_multiplexer: return "multiplexer";
#endif

#ifdef SYS_munlock
    case SYS_munlock: return "munlock";
#endif

#ifdef SYS_munlockall
    case SYS_munlockall: return "munlockall";
#endif

#ifdef SYS_munmap
    case SYS_munmap: return "munmap";
#endif

#ifdef SYS_name_to_handle_at
    case SYS_name_to_handle_at: return "name_to_handle_at";
#endif

#ifdef SYS_nanosleep
    case SYS_nanosleep: return "nanosleep";
#endif

#ifdef SYS_newfstatat
    case SYS_newfstatat: return "newfstatat";
#endif

#ifdef SYS_nfsservctl
    case SYS_nfsservctl: return "nfsservctl";
#endif

#ifdef SYS_ni_syscall
    case SYS_ni_syscall: return "ni_syscall";
#endif

#ifdef SYS_nice
    case SYS_nice: return "nice";
#endif

#ifdef SYS_old_adjtimex
    case SYS_old_adjtimex: return "old_adjtimex";
#endif

#ifdef SYS_old_getpagesize
    case SYS_old_getpagesize: return "old_getpagesize";
#endif

#ifdef SYS_oldfstat
    case SYS_oldfstat: return "oldfstat";
#endif

#ifdef SYS_oldlstat
    case SYS_oldlstat: return "oldlstat";
#endif

#ifdef SYS_oldolduname
    case SYS_oldolduname: return "oldolduname";
#endif

#ifdef SYS_oldstat
    case SYS_oldstat: return "oldstat";
#endif

#ifdef SYS_oldumount
    case SYS_oldumount: return "oldumount";
#endif

#ifdef SYS_olduname
    case SYS_olduname: return "olduname";
#endif

#ifdef SYS_open
    case SYS_open: return "open";
#endif

#ifdef SYS_open_by_handle_at
    case SYS_open_by_handle_at: return "open_by_handle_at";
#endif

#ifdef SYS_open_tree
    case SYS_open_tree: return "open_tree";
#endif

#ifdef SYS_openat
    case SYS_openat: return "openat";
#endif

#ifdef SYS_osf_adjtime
    case SYS_osf_adjtime: return "osf_adjtime";
#endif

#ifdef SYS_osf_afs_syscall
    case SYS_osf_afs_syscall: return "osf_afs_syscall";
#endif

#ifdef SYS_osf_alt_plock
    case SYS_osf_alt_plock: return "osf_alt_plock";
#endif

#ifdef SYS_osf_alt_setsid
    case SYS_osf_alt_setsid: return "osf_alt_setsid";
#endif

#ifdef SYS_osf_alt_sigpending
    case SYS_osf_alt_sigpending: return "osf_alt_sigpending";
#endif

#ifdef SYS_osf_asynch_daemon
    case SYS_osf_asynch_daemon: return "osf_asynch_daemon";
#endif

#ifdef SYS_osf_audcntl
    case SYS_osf_audcntl: return "osf_audcntl";
#endif

#ifdef SYS_osf_audgen
    case SYS_osf_audgen: return "osf_audgen";
#endif

#ifdef SYS_osf_chflags
    case SYS_osf_chflags: return "osf_chflags";
#endif

#ifdef SYS_osf_execve
    case SYS_osf_execve: return "osf_execve";
#endif

#ifdef SYS_osf_exportfs
    case SYS_osf_exportfs: return "osf_exportfs";
#endif

#ifdef SYS_osf_fchflags
    case SYS_osf_fchflags: return "osf_fchflags";
#endif

#ifdef SYS_osf_fdatasync
    case SYS_osf_fdatasync: return "osf_fdatasync";
#endif

#ifdef SYS_osf_fpathconf
    case SYS_osf_fpathconf: return "osf_fpathconf";
#endif

#ifdef SYS_osf_fstat
    case SYS_osf_fstat: return "osf_fstat";
#endif

#ifdef SYS_osf_fstatfs
    case SYS_osf_fstatfs: return "osf_fstatfs";
#endif

#ifdef SYS_osf_fstatfs64
    case SYS_osf_fstatfs64: return "osf_fstatfs64";
#endif

#ifdef SYS_osf_fuser
    case SYS_osf_fuser: return "osf_fuser";
#endif

#ifdef SYS_osf_getaddressconf
    case SYS_osf_getaddressconf: return "osf_getaddressconf";
#endif

#ifdef SYS_osf_getdirentries
    case SYS_osf_getdirentries: return "osf_getdirentries";
#endif

#ifdef SYS_osf_getdomainname
    case SYS_osf_getdomainname: return "osf_getdomainname";
#endif

#ifdef SYS_osf_getfh
    case SYS_osf_getfh: return "osf_getfh";
#endif

#ifdef SYS_osf_getfsstat
    case SYS_osf_getfsstat: return "osf_getfsstat";
#endif

#ifdef SYS_osf_gethostid
    case SYS_osf_gethostid: return "osf_gethostid";
#endif

#ifdef SYS_osf_getitimer
    case SYS_osf_getitimer: return "osf_getitimer";
#endif

#ifdef SYS_osf_getlogin
    case SYS_osf_getlogin: return "osf_getlogin";
#endif

#ifdef SYS_osf_getmnt
    case SYS_osf_getmnt: return "osf_getmnt";
#endif

#ifdef SYS_osf_getrusage
    case SYS_osf_getrusage: return "osf_getrusage";
#endif

#ifdef SYS_osf_getsysinfo
    case SYS_osf_getsysinfo: return "osf_getsysinfo";
#endif

#ifdef SYS_osf_gettimeofday
    case SYS_osf_gettimeofday: return "osf_gettimeofday";
#endif

#ifdef SYS_osf_kloadcall
    case SYS_osf_kloadcall: return "osf_kloadcall";
#endif

#ifdef SYS_osf_kmodcall
    case SYS_osf_kmodcall: return "osf_kmodcall";
#endif

#ifdef SYS_osf_lstat
    case SYS_osf_lstat: return "osf_lstat";
#endif

#ifdef SYS_osf_memcntl
    case SYS_osf_memcntl: return "osf_memcntl";
#endif

#ifdef SYS_osf_mincore
    case SYS_osf_mincore: return "osf_mincore";
#endif

#ifdef SYS_osf_mount
    case SYS_osf_mount: return "osf_mount";
#endif

#ifdef SYS_osf_mremap
    case SYS_osf_mremap: return "osf_mremap";
#endif

#ifdef SYS_osf_msfs_syscall
    case SYS_osf_msfs_syscall: return "osf_msfs_syscall";
#endif

#ifdef SYS_osf_msleep
    case SYS_osf_msleep: return "osf_msleep";
#endif

#ifdef SYS_osf_mvalid
    case SYS_osf_mvalid: return "osf_mvalid";
#endif

#ifdef SYS_osf_mwakeup
    case SYS_osf_mwakeup: return "osf_mwakeup";
#endif

#ifdef SYS_osf_naccept
    case SYS_osf_naccept: return "osf_naccept";
#endif

#ifdef SYS_osf_nfssvc
    case SYS_osf_nfssvc: return "osf_nfssvc";
#endif

#ifdef SYS_osf_ngetpeername
    case SYS_osf_ngetpeername: return "osf_ngetpeername";
#endif

#ifdef SYS_osf_ngetsockname
    case SYS_osf_ngetsockname: return "osf_ngetsockname";
#endif

#ifdef SYS_osf_nrecvfrom
    case SYS_osf_nrecvfrom: return "osf_nrecvfrom";
#endif

#ifdef SYS_osf_nrecvmsg
    case SYS_osf_nrecvmsg: return "osf_nrecvmsg";
#endif

#ifdef SYS_osf_nsendmsg
    case SYS_osf_nsendmsg: return "osf_nsendmsg";
#endif

#ifdef SYS_osf_ntp_adjtime
    case SYS_osf_ntp_adjtime: return "osf_ntp_adjtime";
#endif

#ifdef SYS_osf_ntp_gettime
    case SYS_osf_ntp_gettime: return "osf_ntp_gettime";
#endif

#ifdef SYS_osf_old_creat
    case SYS_osf_old_creat: return "osf_old_creat";
#endif

#ifdef SYS_osf_old_fstat
    case SYS_osf_old_fstat: return "osf_old_fstat";
#endif

#ifdef SYS_osf_old_getpgrp
    case SYS_osf_old_getpgrp: return "osf_old_getpgrp";
#endif

#ifdef SYS_osf_old_killpg
    case SYS_osf_old_killpg: return "osf_old_killpg";
#endif

#ifdef SYS_osf_old_lstat
    case SYS_osf_old_lstat: return "osf_old_lstat";
#endif

#ifdef SYS_osf_old_open
    case SYS_osf_old_open: return "osf_old_open";
#endif

#ifdef SYS_osf_old_sigaction
    case SYS_osf_old_sigaction: return "osf_old_sigaction";
#endif

#ifdef SYS_osf_old_sigblock
    case SYS_osf_old_sigblock: return "osf_old_sigblock";
#endif

#ifdef SYS_osf_old_sigreturn
    case SYS_osf_old_sigreturn: return "osf_old_sigreturn";
#endif

#ifdef SYS_osf_old_sigsetmask
    case SYS_osf_old_sigsetmask: return "osf_old_sigsetmask";
#endif

#ifdef SYS_osf_old_sigvec
    case SYS_osf_old_sigvec: return "osf_old_sigvec";
#endif

#ifdef SYS_osf_old_stat
    case SYS_osf_old_stat: return "osf_old_stat";
#endif

#ifdef SYS_osf_old_vadvise
    case SYS_osf_old_vadvise: return "osf_old_vadvise";
#endif

#ifdef SYS_osf_old_vtrace
    case SYS_osf_old_vtrace: return "osf_old_vtrace";
#endif

#ifdef SYS_osf_old_wait
    case SYS_osf_old_wait: return "osf_old_wait";
#endif

#ifdef SYS_osf_oldquota
    case SYS_osf_oldquota: return "osf_oldquota";
#endif

#ifdef SYS_osf_pathconf
    case SYS_osf_pathconf: return "osf_pathconf";
#endif

#ifdef SYS_osf_pid_block
    case SYS_osf_pid_block: return "osf_pid_block";
#endif

#ifdef SYS_osf_pid_unblock
    case SYS_osf_pid_unblock: return "osf_pid_unblock";
#endif

#ifdef SYS_osf_plock
    case SYS_osf_plock: return "osf_plock";
#endif

#ifdef SYS_osf_priocntlset
    case SYS_osf_priocntlset: return "osf_priocntlset";
#endif

#ifdef SYS_osf_profil
    case SYS_osf_profil: return "osf_profil";
#endif

#ifdef SYS_osf_proplist_syscall
    case SYS_osf_proplist_syscall: return "osf_proplist_syscall";
#endif

#ifdef SYS_osf_reboot
    case SYS_osf_reboot: return "osf_reboot";
#endif

#ifdef SYS_osf_revoke
    case SYS_osf_revoke: return "osf_revoke";
#endif

#ifdef SYS_osf_sbrk
    case SYS_osf_sbrk: return "osf_sbrk";
#endif

#ifdef SYS_osf_security
    case SYS_osf_security: return "osf_security";
#endif

#ifdef SYS_osf_select
    case SYS_osf_select: return "osf_select";
#endif

#ifdef SYS_osf_set_program_attributes
    case SYS_osf_set_program_attributes: return "osf_set_program_attributes";
#endif

#ifdef SYS_osf_set_speculative
    case SYS_osf_set_speculative: return "osf_set_speculative";
#endif

#ifdef SYS_osf_sethostid
    case SYS_osf_sethostid: return "osf_sethostid";
#endif

#ifdef SYS_osf_setitimer
    case SYS_osf_setitimer: return "osf_setitimer";
#endif

#ifdef SYS_osf_setlogin
    case SYS_osf_setlogin: return "osf_setlogin";
#endif

#ifdef SYS_osf_setsysinfo
    case SYS_osf_setsysinfo: return "osf_setsysinfo";
#endif

#ifdef SYS_osf_settimeofday
    case SYS_osf_settimeofday: return "osf_settimeofday";
#endif

#ifdef SYS_osf_shmat
    case SYS_osf_shmat: return "osf_shmat";
#endif

#ifdef SYS_osf_signal
    case SYS_osf_signal: return "osf_signal";
#endif

#ifdef SYS_osf_sigprocmask
    case SYS_osf_sigprocmask: return "osf_sigprocmask";
#endif

#ifdef SYS_osf_sigsendset
    case SYS_osf_sigsendset: return "osf_sigsendset";
#endif

#ifdef SYS_osf_sigstack
    case SYS_osf_sigstack: return "osf_sigstack";
#endif

#ifdef SYS_osf_sigwaitprim
    case SYS_osf_sigwaitprim: return "osf_sigwaitprim";
#endif

#ifdef SYS_osf_sstk
    case SYS_osf_sstk: return "osf_sstk";
#endif

#ifdef SYS_osf_stat
    case SYS_osf_stat: return "osf_stat";
#endif

#ifdef SYS_osf_statfs
    case SYS_osf_statfs: return "osf_statfs";
#endif

#ifdef SYS_osf_statfs64
    case SYS_osf_statfs64: return "osf_statfs64";
#endif

#ifdef SYS_osf_subsys_info
    case SYS_osf_subsys_info: return "osf_subsys_info";
#endif

#ifdef SYS_osf_swapctl
    case SYS_osf_swapctl: return "osf_swapctl";
#endif

#ifdef SYS_osf_swapon
    case SYS_osf_swapon: return "osf_swapon";
#endif

#ifdef SYS_osf_syscall
    case SYS_osf_syscall: return "osf_syscall";
#endif

#ifdef SYS_osf_sysinfo
    case SYS_osf_sysinfo: return "osf_sysinfo";
#endif

#ifdef SYS_osf_table
    case SYS_osf_table: return "osf_table";
#endif

#ifdef SYS_osf_uadmin
    case SYS_osf_uadmin: return "osf_uadmin";
#endif

#ifdef SYS_osf_usleep_thread
    case SYS_osf_usleep_thread: return "osf_usleep_thread";
#endif

#ifdef SYS_osf_uswitch
    case SYS_osf_uswitch: return "osf_uswitch";
#endif

#ifdef SYS_osf_utc_adjtime
    case SYS_osf_utc_adjtime: return "osf_utc_adjtime";
#endif

#ifdef SYS_osf_utc_gettime
    case SYS_osf_utc_gettime: return "osf_utc_gettime";
#endif

#ifdef SYS_osf_utimes
    case SYS_osf_utimes: return "osf_utimes";
#endif

#ifdef SYS_osf_utsname
    case SYS_osf_utsname: return "osf_utsname";
#endif

#ifdef SYS_osf_wait4
    case SYS_osf_wait4: return "osf_wait4";
#endif

#ifdef SYS_osf_waitid
    case SYS_osf_waitid: return "osf_waitid";
#endif

#ifdef SYS_pause
    case SYS_pause: return "pause";
#endif

#ifdef SYS_pciconfig_iobase
    case SYS_pciconfig_iobase: return "pciconfig_iobase";
#endif

#ifdef SYS_pciconfig_read
    case SYS_pciconfig_read: return "pciconfig_read";
#endif

#ifdef SYS_pciconfig_write
    case SYS_pciconfig_write: return "pciconfig_write";
#endif

#ifdef SYS_perf_event_open
    case SYS_perf_event_open: return "perf_event_open";
#endif

#ifdef SYS_perfctr
    case SYS_perfctr: return "perfctr";
#endif

#ifdef SYS_perfmonctl
    case SYS_perfmonctl: return "perfmonctl";
#endif

#ifdef SYS_personality
    case SYS_personality: return "personality";
#endif

#ifdef SYS_pidfd_open
    case SYS_pidfd_open: return "pidfd_open";
#endif

#ifdef SYS_pidfd_send_signal
    case SYS_pidfd_send_signal: return "pidfd_send_signal";
#endif

#ifdef SYS_pipe
    case SYS_pipe: return "pipe";
#endif

#ifdef SYS_pipe2
    case SYS_pipe2: return "pipe2";
#endif

#ifdef SYS_pivot_root
    case SYS_pivot_root: return "pivot_root";
#endif

#ifdef SYS_pkey_alloc
    case SYS_pkey_alloc: return "pkey_alloc";
#endif

#ifdef SYS_pkey_free
    case SYS_pkey_free: return "pkey_free";
#endif

#ifdef SYS_pkey_mprotect
    case SYS_pkey_mprotect: return "pkey_mprotect";
#endif

#ifdef SYS_poll
    case SYS_poll: return "poll";
#endif

#ifdef SYS_ppoll
    case SYS_ppoll: return "ppoll";
#endif

#ifdef SYS_ppoll_time64
    case SYS_ppoll_time64: return "ppoll_time64";
#endif

#ifdef SYS_prctl
    case SYS_prctl: return "prctl";
#endif

#ifdef SYS_pread64
    case SYS_pread64: return "pread64";
#endif

#ifdef SYS_preadv
    case SYS_preadv: return "preadv";
#endif

#ifdef SYS_preadv2
    case SYS_preadv2: return "preadv2";
#endif

#ifdef SYS_prlimit64
    case SYS_prlimit64: return "prlimit64";
#endif

#ifdef SYS_process_vm_readv
    case SYS_process_vm_readv: return "process_vm_readv";
#endif

#ifdef SYS_process_vm_writev
    case SYS_process_vm_writev: return "process_vm_writev";
#endif

#ifdef SYS_prof
    case SYS_prof: return "prof";
#endif

#ifdef SYS_profil
    case SYS_profil: return "profil";
#endif

#ifdef SYS_pselect6
    case SYS_pselect6: return "pselect6";
#endif

#ifdef SYS_pselect6_time64
    case SYS_pselect6_time64: return "pselect6_time64";
#endif

#ifdef SYS_ptrace
    case SYS_ptrace: return "ptrace";
#endif

#ifdef SYS_putpmsg
    case SYS_putpmsg: return "putpmsg";
#endif

#ifdef SYS_pwrite64
    case SYS_pwrite64: return "pwrite64";
#endif

#ifdef SYS_pwritev
    case SYS_pwritev: return "pwritev";
#endif

#ifdef SYS_pwritev2
    case SYS_pwritev2: return "pwritev2";
#endif

#ifdef SYS_query_module
    case SYS_query_module: return "query_module";
#endif

#ifdef SYS_quotactl
    case SYS_quotactl: return "quotactl";
#endif

#ifdef SYS_read
    case SYS_read: return "read";
#endif

#ifdef SYS_readahead
    case SYS_readahead: return "readahead";
#endif

#ifdef SYS_readdir
    case SYS_readdir: return "readdir";
#endif

#ifdef SYS_readlink
    case SYS_readlink: return "readlink";
#endif

#ifdef SYS_readlinkat
    case SYS_readlinkat: return "readlinkat";
#endif

#ifdef SYS_readv
    case SYS_readv: return "readv";
#endif

#ifdef SYS_reboot
    case SYS_reboot: return "reboot";
#endif

#ifdef SYS_recv
    case SYS_recv: return "recv";
#endif

#ifdef SYS_recvfrom
    case SYS_recvfrom: return "recvfrom";
#endif

#ifdef SYS_recvmmsg
    case SYS_recvmmsg: return "recvmmsg";
#endif

#ifdef SYS_recvmmsg_time64
    case SYS_recvmmsg_time64: return "recvmmsg_time64";
#endif

#ifdef SYS_recvmsg
    case SYS_recvmsg: return "recvmsg";
#endif

#ifdef SYS_remap_file_pages
    case SYS_remap_file_pages: return "remap_file_pages";
#endif

#ifdef SYS_removexattr
    case SYS_removexattr: return "removexattr";
#endif

#ifdef SYS_rename
    case SYS_rename: return "rename";
#endif

#ifdef SYS_renameat
    case SYS_renameat: return "renameat";
#endif

#ifdef SYS_renameat2
    case SYS_renameat2: return "renameat2";
#endif

#ifdef SYS_request_key
    case SYS_request_key: return "request_key";
#endif

#ifdef SYS_restart_syscall
    case SYS_restart_syscall: return "restart_syscall";
#endif

#ifdef SYS_riscv_flush_icache
    case SYS_riscv_flush_icache: return "riscv_flush_icache";
#endif

#ifdef SYS_rmdir
    case SYS_rmdir: return "rmdir";
#endif

#ifdef SYS_rseq
    case SYS_rseq: return "rseq";
#endif

#ifdef SYS_rt_sigaction
    case SYS_rt_sigaction: return "rt_sigaction";
#endif

#ifdef SYS_rt_sigpending
    case SYS_rt_sigpending: return "rt_sigpending";
#endif

#ifdef SYS_rt_sigprocmask
    case SYS_rt_sigprocmask: return "rt_sigprocmask";
#endif

#ifdef SYS_rt_sigqueueinfo
    case SYS_rt_sigqueueinfo: return "rt_sigqueueinfo";
#endif

#ifdef SYS_rt_sigreturn
    case SYS_rt_sigreturn: return "rt_sigreturn";
#endif

#ifdef SYS_rt_sigsuspend
    case SYS_rt_sigsuspend: return "rt_sigsuspend";
#endif

#ifdef SYS_rt_sigtimedwait
    case SYS_rt_sigtimedwait: return "rt_sigtimedwait";
#endif

#ifdef SYS_rt_sigtimedwait_time64
    case SYS_rt_sigtimedwait_time64: return "rt_sigtimedwait_time64";
#endif

#ifdef SYS_rt_tgsigqueueinfo
    case SYS_rt_tgsigqueueinfo: return "rt_tgsigqueueinfo";
#endif

#ifdef SYS_rtas
    case SYS_rtas: return "rtas";
#endif

#ifdef SYS_s390_guarded_storage
    case SYS_s390_guarded_storage: return "s390_guarded_storage";
#endif

#ifdef SYS_s390_pci_mmio_read
    case SYS_s390_pci_mmio_read: return "s390_pci_mmio_read";
#endif

#ifdef SYS_s390_pci_mmio_write
    case SYS_s390_pci_mmio_write: return "s390_pci_mmio_write";
#endif

#ifdef SYS_s390_runtime_instr
    case SYS_s390_runtime_instr: return "s390_runtime_instr";
#endif

#ifdef SYS_s390_sthyi
    case SYS_s390_sthyi: return "s390_sthyi";
#endif

#ifdef SYS_sched_get_affinity
    case SYS_sched_get_affinity: return "sched_get_affinity";
#endif

#ifdef SYS_sched_get_priority_max
    case SYS_sched_get_priority_max: return "sched_get_priority_max";
#endif

#ifdef SYS_sched_get_priority_min
    case SYS_sched_get_priority_min: return "sched_get_priority_min";
#endif

#ifdef SYS_sched_getaffinity
    case SYS_sched_getaffinity: return "sched_getaffinity";
#endif

#ifdef SYS_sched_getattr
    case SYS_sched_getattr: return "sched_getattr";
#endif

#ifdef SYS_sched_getparam
    case SYS_sched_getparam: return "sched_getparam";
#endif

#ifdef SYS_sched_getscheduler
    case SYS_sched_getscheduler: return "sched_getscheduler";
#endif

#ifdef SYS_sched_rr_get_interval
    case SYS_sched_rr_get_interval: return "sched_rr_get_interval";
#endif

#ifdef SYS_sched_rr_get_interval_time64
    case SYS_sched_rr_get_interval_time64: return "sched_rr_get_interval_time64";
#endif

#ifdef SYS_sched_set_affinity
    case SYS_sched_set_affinity: return "sched_set_affinity";
#endif

#ifdef SYS_sched_setaffinity
    case SYS_sched_setaffinity: return "sched_setaffinity";
#endif

#ifdef SYS_sched_setattr
    case SYS_sched_setattr: return "sched_setattr";
#endif

#ifdef SYS_sched_setparam
    case SYS_sched_setparam: return "sched_setparam";
#endif

#ifdef SYS_sched_setscheduler
    case SYS_sched_setscheduler: return "sched_setscheduler";
#endif

#ifdef SYS_sched_yield
    case SYS_sched_yield: return "sched_yield";
#endif

#ifdef SYS_seccomp
    case SYS_seccomp: return "seccomp";
#endif

#ifdef SYS_security
    case SYS_security: return "security";
#endif

#ifdef SYS_select
    case SYS_select: return "select";
#endif

#ifdef SYS_semctl
    case SYS_semctl: return "semctl";
#endif

#ifdef SYS_semget
    case SYS_semget: return "semget";
#endif

#ifdef SYS_semop
    case SYS_semop: return "semop";
#endif

#ifdef SYS_semtimedop
    case SYS_semtimedop: return "semtimedop";
#endif

#ifdef SYS_semtimedop_time64
    case SYS_semtimedop_time64: return "semtimedop_time64";
#endif

#ifdef SYS_send
    case SYS_send: return "send";
#endif

#ifdef SYS_sendfile
    case SYS_sendfile: return "sendfile";
#endif

#ifdef SYS_sendfile64
    case SYS_sendfile64: return "sendfile64";
#endif

#ifdef SYS_sendmmsg
    case SYS_sendmmsg: return "sendmmsg";
#endif

#ifdef SYS_sendmsg
    case SYS_sendmsg: return "sendmsg";
#endif

#ifdef SYS_sendto
    case SYS_sendto: return "sendto";
#endif

#ifdef SYS_set_mempolicy
    case SYS_set_mempolicy: return "set_mempolicy";
#endif

#ifdef SYS_set_robust_list
    case SYS_set_robust_list: return "set_robust_list";
#endif

#ifdef SYS_set_thread_area
    case SYS_set_thread_area: return "set_thread_area";
#endif

#ifdef SYS_set_tid_address
    case SYS_set_tid_address: return "set_tid_address";
#endif

#ifdef SYS_set_tls
    case SYS_set_tls: return "set_tls";
#endif

#ifdef SYS_setdomainname
    case SYS_setdomainname: return "setdomainname";
#endif

#ifdef SYS_setfsgid
    case SYS_setfsgid: return "setfsgid";
#endif

#ifdef SYS_setfsgid32
    case SYS_setfsgid32: return "setfsgid32";
#endif

#ifdef SYS_setfsuid
    case SYS_setfsuid: return "setfsuid";
#endif

#ifdef SYS_setfsuid32
    case SYS_setfsuid32: return "setfsuid32";
#endif

#ifdef SYS_setgid
    case SYS_setgid: return "setgid";
#endif

#ifdef SYS_setgid32
    case SYS_setgid32: return "setgid32";
#endif

#ifdef SYS_setgroups
    case SYS_setgroups: return "setgroups";
#endif

#ifdef SYS_setgroups32
    case SYS_setgroups32: return "setgroups32";
#endif

#ifdef SYS_sethae
    case SYS_sethae: return "sethae";
#endif

#ifdef SYS_sethostname
    case SYS_sethostname: return "sethostname";
#endif

#ifdef SYS_setitimer
    case SYS_setitimer: return "setitimer";
#endif

#ifdef SYS_setns
    case SYS_setns: return "setns";
#endif

#ifdef SYS_setpgid
    case SYS_setpgid: return "setpgid";
#endif

#ifdef SYS_setpgrp
    case SYS_setpgrp: return "setpgrp";
#endif

#ifdef SYS_setpriority
    case SYS_setpriority: return "setpriority";
#endif

#ifdef SYS_setregid
    case SYS_setregid: return "setregid";
#endif

#ifdef SYS_setregid32
    case SYS_setregid32: return "setregid32";
#endif

#ifdef SYS_setresgid
    case SYS_setresgid: return "setresgid";
#endif

#ifdef SYS_setresgid32
    case SYS_setresgid32: return "setresgid32";
#endif

#ifdef SYS_setresuid
    case SYS_setresuid: return "setresuid";
#endif

#ifdef SYS_setresuid32
    case SYS_setresuid32: return "setresuid32";
#endif

#ifdef SYS_setreuid
    case SYS_setreuid: return "setreuid";
#endif

#ifdef SYS_setreuid32
    case SYS_setreuid32: return "setreuid32";
#endif

#ifdef SYS_setrlimit
    case SYS_setrlimit: return "setrlimit";
#endif

#ifdef SYS_setsid
    case SYS_setsid: return "setsid";
#endif

#ifdef SYS_setsockopt
    case SYS_setsockopt: return "setsockopt";
#endif

#ifdef SYS_settimeofday
    case SYS_settimeofday: return "settimeofday";
#endif

#ifdef SYS_setuid
    case SYS_setuid: return "setuid";
#endif

#ifdef SYS_setuid32
    case SYS_setuid32: return "setuid32";
#endif

#ifdef SYS_setxattr
    case SYS_setxattr: return "setxattr";
#endif

#ifdef SYS_sgetmask
    case SYS_sgetmask: return "sgetmask";
#endif

#ifdef SYS_shmat
    case SYS_shmat: return "shmat";
#endif

#ifdef SYS_shmctl
    case SYS_shmctl: return "shmctl";
#endif

#ifdef SYS_shmdt
    case SYS_shmdt: return "shmdt";
#endif

#ifdef SYS_shmget
    case SYS_shmget: return "shmget";
#endif

#ifdef SYS_shutdown
    case SYS_shutdown: return "shutdown";
#endif

#ifdef SYS_sigaction
    case SYS_sigaction: return "sigaction";
#endif

#ifdef SYS_sigaltstack
    case SYS_sigaltstack: return "sigaltstack";
#endif

#ifdef SYS_signal
    case SYS_signal: return "signal";
#endif

#ifdef SYS_signalfd
    case SYS_signalfd: return "signalfd";
#endif

#ifdef SYS_signalfd4
    case SYS_signalfd4: return "signalfd4";
#endif

#ifdef SYS_sigpending
    case SYS_sigpending: return "sigpending";
#endif

#ifdef SYS_sigprocmask
    case SYS_sigprocmask: return "sigprocmask";
#endif

#ifdef SYS_sigreturn
    case SYS_sigreturn: return "sigreturn";
#endif

#ifdef SYS_sigsuspend
    case SYS_sigsuspend: return "sigsuspend";
#endif

#ifdef SYS_socket
    case SYS_socket: return "socket";
#endif

#ifdef SYS_socketcall
    case SYS_socketcall: return "socketcall";
#endif

#ifdef SYS_socketpair
    case SYS_socketpair: return "socketpair";
#endif

#ifdef SYS_splice
    case SYS_splice: return "splice";
#endif

#ifdef SYS_spu_create
    case SYS_spu_create: return "spu_create";
#endif

#ifdef SYS_spu_run
    case SYS_spu_run: return "spu_run";
#endif

#ifdef SYS_ssetmask
    case SYS_ssetmask: return "ssetmask";
#endif

#ifdef SYS_stat
    case SYS_stat: return "stat";
#endif

#ifdef SYS_stat64
    case SYS_stat64: return "stat64";
#endif

#ifdef SYS_statfs
    case SYS_statfs: return "statfs";
#endif

#ifdef SYS_statfs64
    case SYS_statfs64: return "statfs64";
#endif

#ifdef SYS_statx
    case SYS_statx: return "statx";
#endif

#ifdef SYS_stime
    case SYS_stime: return "stime";
#endif

#ifdef SYS_stty
    case SYS_stty: return "stty";
#endif

#ifdef SYS_subpage_prot
    case SYS_subpage_prot: return "subpage_prot";
#endif

#ifdef SYS_swapcontext
    case SYS_swapcontext: return "swapcontext";
#endif

#ifdef SYS_swapoff
    case SYS_swapoff: return "swapoff";
#endif

#ifdef SYS_swapon
    case SYS_swapon: return "swapon";
#endif

#ifdef SYS_switch_endian
    case SYS_switch_endian: return "switch_endian";
#endif

#ifdef SYS_symlink
    case SYS_symlink: return "symlink";
#endif

#ifdef SYS_symlinkat
    case SYS_symlinkat: return "symlinkat";
#endif

#ifdef SYS_sync
    case SYS_sync: return "sync";
#endif

#ifdef SYS_sync_file_range
    case SYS_sync_file_range: return "sync_file_range";
#endif

#ifdef SYS_sync_file_range2
    case SYS_sync_file_range2: return "sync_file_range2";
#endif

#ifdef SYS_syncfs
    case SYS_syncfs: return "syncfs";
#endif

#ifdef SYS_sys_debug_setcontext
    case SYS_sys_debug_setcontext: return "sys_debug_setcontext";
#endif

#ifdef SYS_sys_epoll_create
    case SYS_sys_epoll_create: return "sys_epoll_create";
#endif

#ifdef SYS_sys_epoll_ctl
    case SYS_sys_epoll_ctl: return "sys_epoll_ctl";
#endif

#ifdef SYS_sys_epoll_wait
    case SYS_sys_epoll_wait: return "sys_epoll_wait";
#endif

#ifdef SYS_syscall
    case SYS_syscall: return "syscall";
#endif

#ifdef SYS_sysfs
    case SYS_sysfs: return "sysfs";
#endif

#ifdef SYS_sysinfo
    case SYS_sysinfo: return "sysinfo";
#endif

#ifdef SYS_syslog
    case SYS_syslog: return "syslog";
#endif

#ifdef SYS_sysmips
    case SYS_sysmips: return "sysmips";
#endif

#ifdef SYS_tee
    case SYS_tee: return "tee";
#endif

#ifdef SYS_tgkill
    case SYS_tgkill: return "tgkill";
#endif

#ifdef SYS_time
    case SYS_time: return "time";
#endif

#ifdef SYS_timer_create
    case SYS_timer_create: return "timer_create";
#endif

#ifdef SYS_timer_delete
    case SYS_timer_delete: return "timer_delete";
#endif

#ifdef SYS_timer_getoverrun
    case SYS_timer_getoverrun: return "timer_getoverrun";
#endif

#ifdef SYS_timer_gettime
    case SYS_timer_gettime: return "timer_gettime";
#endif

#ifdef SYS_timer_gettime64
    case SYS_timer_gettime64: return "timer_gettime64";
#endif

#ifdef SYS_timer_settime
    case SYS_timer_settime: return "timer_settime";
#endif

#ifdef SYS_timer_settime64
    case SYS_timer_settime64: return "timer_settime64";
#endif

#ifdef SYS_timerfd
    case SYS_timerfd: return "timerfd";
#endif

#ifdef SYS_timerfd_create
    case SYS_timerfd_create: return "timerfd_create";
#endif

#ifdef SYS_timerfd_gettime
    case SYS_timerfd_gettime: return "timerfd_gettime";
#endif

#ifdef SYS_timerfd_gettime64
    case SYS_timerfd_gettime64: return "timerfd_gettime64";
#endif

#ifdef SYS_timerfd_settime
    case SYS_timerfd_settime: return "timerfd_settime";
#endif

#ifdef SYS_timerfd_settime64
    case SYS_timerfd_settime64: return "timerfd_settime64";
#endif

#ifdef SYS_times
    case SYS_times: return "times";
#endif

#ifdef SYS_tkill
    case SYS_tkill: return "tkill";
#endif

#ifdef SYS_truncate
    case SYS_truncate: return "truncate";
#endif

#ifdef SYS_truncate64
    case SYS_truncate64: return "truncate64";
#endif

#ifdef SYS_tuxcall
    case SYS_tuxcall: return "tuxcall";
#endif

#ifdef SYS_udftrap
    case SYS_udftrap: return "udftrap";
#endif

#ifdef SYS_ugetrlimit
    case SYS_ugetrlimit: return "ugetrlimit";
#endif

#ifdef SYS_ulimit
    case SYS_ulimit: return "ulimit";
#endif

#ifdef SYS_umask
    case SYS_umask: return "umask";
#endif

#ifdef SYS_umount
    case SYS_umount: return "umount";
#endif

#ifdef SYS_umount2
    case SYS_umount2: return "umount2";
#endif

#ifdef SYS_uname
    case SYS_uname: return "uname";
#endif

#ifdef SYS_unlink
    case SYS_unlink: return "unlink";
#endif

#ifdef SYS_unlinkat
    case SYS_unlinkat: return "unlinkat";
#endif

#ifdef SYS_unshare
    case SYS_unshare: return "unshare";
#endif

#ifdef SYS_uselib
    case SYS_uselib: return "uselib";
#endif

#ifdef SYS_userfaultfd
    case SYS_userfaultfd: return "userfaultfd";
#endif

#ifdef SYS_usr26
    case SYS_usr26: return "usr26";
#endif

#ifdef SYS_usr32
    case SYS_usr32: return "usr32";
#endif

#ifdef SYS_ustat
    case SYS_ustat: return "ustat";
#endif

#ifdef SYS_utime
    case SYS_utime: return "utime";
#endif

#ifdef SYS_utimensat
    case SYS_utimensat: return "utimensat";
#endif

#ifdef SYS_utimensat_time64
    case SYS_utimensat_time64: return "utimensat_time64";
#endif

#ifdef SYS_utimes
    case SYS_utimes: return "utimes";
#endif

#ifdef SYS_utrap_install
    case SYS_utrap_install: return "utrap_install";
#endif

#ifdef SYS_vfork
    case SYS_vfork: return "vfork";
#endif

#ifdef SYS_vhangup
    case SYS_vhangup: return "vhangup";
#endif

#ifdef SYS_vm86
    case SYS_vm86: return "vm86";
#endif

#ifdef SYS_vm86old
    case SYS_vm86old: return "vm86old";
#endif

#ifdef SYS_vmsplice
    case SYS_vmsplice: return "vmsplice";
#endif

#ifdef SYS_vserver
    case SYS_vserver: return "vserver";
#endif

#ifdef SYS_wait4
    case SYS_wait4: return "wait4";
#endif

#ifdef SYS_waitid
    case SYS_waitid: return "waitid";
#endif

#ifdef SYS_waitpid
    case SYS_waitpid: return "waitpid";
#endif

#ifdef SYS_write
    case SYS_write: return "write";
#endif

#ifdef SYS_writev
    case SYS_writev: return "writev";
#endif

        default: return "???";
    }
}