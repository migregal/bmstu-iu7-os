#pragma once

#ifndef __CONST_HEADER__
#define __CONST_HEADER__
    const char *WITH_DESCR[] = {
        "(1) pid                    %20s (The process ID).\n",
        "(2) comm                   %20s (The filename of the executable, in parentheses).\n",
        "(3) state                  %20s (one of the characters indicating process state).\n",
        "(4) ppid                   %20s (The PID of the parent of this process).\n",
        "(5) pgrp                   %20s (The process group ID of the process).\n",
        "(6) session                %20s (The session ID of the process).\n",
        "(7) tty_nr                 %20s (The controlling terminal of the process).\n",
        "(8) tpgid                  %20s (The ID of the foreground process group of the controlling terminal of the process).\n",
        "(9) flags                  %20s (The kernel flags word of the process).\n",
        "(10) minflt                %20s (The number of minor faults the process has made which have not required loading a memory page from disk).\n",
        "(11) cminflt               %20s (The number of minor faults that the process's waited-for children have made).\n",
        "(12) majflt                %20s (The number of major faults the process has made which have required  loading  a  memory  page from disk).\n",
        "(13) cmajflt               %20s (The number of major faults that the process's waited-for children have made).\n",
        "(14) utime                 %20s (Amount of time that this process has been scheduled in user mode, measured in clock ticks).\n",
        "(15) stime                 %20s (Amount of time that this process has been scheduled in kernel mode, measured in  clock  ticks).\n",
        "(16) cutime                %20s (Amount of time that this process's waited-for children have been scheduled in user mode, measured in clock ticks).\n",
        "(17) cstime                %20s (Amount of time that this process's waited-for children have been scheduled in kernel mode, measured in clock ticks).\n",
        "(18) priority              %20s (For processes running a real-time scheduling policy).\n",
        "(19) nice                  %20s (The nice value (see setpriority(2)), a value in the range 19 (low priority) to -20 (high priority)).\n",
        "(20) num_threads           %20s (Number of threads in this process (since Linux 2.6). Before kernel 2.6, this field was hardcoded to 0 as a placeholder for an earlier removed field.\n",
        "(21) itrealvalue           %20s (The time in jiffies before the next SIGALRM is sent to the process due to an interval timer).\n",
        "(22) starttime             %20s (The time the process started after system boot).\n",
        "(23) vsize                 %20s (Virtual memory size in bytes).\n",
        "(24) rss                   %20s (Resident Set Size: number of pages the process has in real memory).\n",
        "(25) rsslim                %20s (Current soft limit in bytes on the rss of the process).\n",
        "(26) startcode             %20s (The address above which program text can run).\n",
        "(27) endcode               %20s (The address below which program text can run).\n",
        "(28) startstack            %20s (The address of the start (i).e., bottom) of the stack.\n",
        "(29) kstkesp               %20s (The current value of ESP (stack pointer), as found in the kernel stack page for the process).\n",
        "(30) kstkeip               %20s (The current EIP (instruction pointer)).\n",
        "(31) signal                %20s (The  bitmap of pending signals, displayed as a decimal number).\n",
        "(32) blocked               %20s (The bitmap of blocked signals, displayed as a decimal number).\n",
        "(33) sigignore             %20s (The  bitmap of ignored signals, displayed as a decimal number).\n",
        "(34) sigcatch              %20s (The bitmap of caught signals, displayed as a decimal number).\n",
        "(35) wchan                 %20s (This is the \"channel\" in which the process is waiting).\n",
        "(36) nswap                 %20s (Number of pages swapped (not maintained)).\n",
        "(37) cnswap                %20s (Cumulative nswap for child processes (not maintained)).\n",
        "(38) exit_signal           %20s (since Linux 2.1.22) (Signal to be sent to parent when we die).\n",
        "(39) processor             %20s (since Linux 2.2.8) (CPU number last executed on).\n",
        "(40) rt_priority           %20s (since Linux 2.5.19) (Real-time scheduling priority, a number in the range 1 to 99 for processes scheduled under a real-time policy, or 0, for non-real-time processes).\n",
        "(41) policy                %20s (since Linux 2.5.19) (Scheduling policy).\n",
        "(42) delayacct_blkio_ticks %20s (since Linux 2.6.18) (Aggregated block I/O delays, measured in clock ticks (centiseconds)).\n",
        "(43) guest_time            %20s (since Linux 2.6.24) (Guest time of the process (time spent running a virtual CPU for a guest operating system), measured in clock ticks).\n",
        "(44) cguest_time           %20s (since Linux 2.6.24) (Guest time of the process's children, measured in clock ticks).\n",
        "(45) start_data            %20s (since Linux 3.3) (Address above which program initialized and uninitialized (BSS) data are placed).\n",
        "(46) end_data              %20s (since Linux 3.3) (Address below which program initialized and uninitialized (BSS) data are placed).\n",
        "(47) start_brk             %20s (since Linux 3.3) (Address above which program heap can be expanded with brk(2)).\n",
        "(48) arg_start             %20s (since Linux 3.5) (Address above which program command-line arguments (argv) are placed).\n",
        "(49) arg_end               %20s (since Linux 3.5) (Address below program command-line arguments (argv) are placed).\n",
        "(50) env_start             %20s (since Linux 3.5) (Address above which program environment is placed).\n",
        "(51) env_end               %20s (since Linux 3.5) (Address below which program environment is placed).\n",
        "(52) exit_code             %20s (since Linux 3.5) (The thread's exit status in the form reported by waitpid(2)).\n"};

    const char *NO_DESCR[] = {
        "(1) pid                    %20s\n",
        "(2) comm                   %20s\n",
        "(3) state                  %20s\n",
        "(4) ppid                   %20s\n",
        "(5) pgrp                   %20s\n",
        "(6) session                %20s\n",
        "(7) tty_nr                 %20s\n",
        "(8) tpgid                  %20s\n",
        "(9) flags                  %20s\n",
        "(10) minflt                %20s\n",
        "(11) cminflt               %20s\n",
        "(12) majflt                %20s\n",
        "(13) cmajflt               %20s\n",
        "(14) utime                 %20s\n",
        "(15) stime                 %20s\n",
        "(16) cutime                %20s\n",
        "(17) cstime                %20s\n",
        "(18) priority              %20s\n",
        "(19) nice                  %20s\n",
        "(20) num_threads           %20s\n",
        "(21) itrealvalue           %20s\n",
        "(22) starttime             %20s\n",
        "(23) vsize                 %20s\n",
        "(24) rss                   %20s\n",
        "(25) rsslim                %20s\n",
        "(26) startcode             %20s\n",
        "(27) endcode               %20s\n",
        "(28) startstack            %20s\n",
        "(29) kstkesp               %20s\n",
        "(30) kstkeip               %20s\n",
        "(31) signal                %20s\n",
        "(32) blocked               %20s\n",
        "(33) sigignore             %20s\n",
        "(34) sigcatch              %20s\n",
        "(35) wchan                 %20s\n",
        "(36) nswap                 %20s\n",
        "(37) cnswap                %20s\n",
        "(38) exit_signal           %20s\n",
        "(39) processor             %20s\n",
        "(40) rt_priority           %20s\n",
        "(41) policy                %20s\n",
        "(42) delayacct_blkio_ticks %20s\n",
        "(43) guest_time            %20s\n",
        "(44) cguest_time           %20s\n",
        "(45) start_data            %20s\n",
        "(46) end_data              %20s\n",
        "(47) start_brk             %20s\n",
        "(48) arg_start             %20s\n",
        "(49) arg_end               %20s\n",
        "(50) env_start             %20s\n",
        "(51) env_end               %20s\n",
        "(52) exit_code             %20s\n"};

    const char *STATM_PATTERNS[] = {
        "size     %8s  (1)\n",
        "resident %8s  (2)\n",
        "shared   %8s  (3)\n",
        "text     %8s  (4)\n",
        "lib      %8s  (5)\n",
        "data     %8s  (6)\n",
        "dt       %8s  (7)\n"};
#endif