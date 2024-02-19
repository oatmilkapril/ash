# Intro

ash is a simple shell program. It's an abbreviation of "April's shell".

You can run cash using the following steps:

```sh
$ mkdir build
$ cd build/
$ cmake ..
$ make
$ ./ash
```

You can exit cash by typing "exit"

# Functionality

- When the user types ctrl+c, a SIGINT is forwarded to all fg processes
- When the user types ctrl+z, a SIGSTP is forwarded to all fg processes
- When a process terminates due to an uncaught signal, the shell should reap each child proces.
- Jobs ending with a & should be run in the bg
- Support for the following cmd_type commands:
    - exit
    - jobs 
    - bg 
    - fg 
    - alias
    - help
    - export
    - echo
- Boolean expressions (&&, ||)
- Pipes to files (>)
- Pipes for stdin/stdout/stderr (|)

Note that many of these features aren't implemented yet.

# Notes

Remember to block signals when accessing global stores in async-signal-safe handlers. Also be careful around
the fork calls

Signals are sent to the entire foreground process group (to cover spawned children). When we fork a process,
the pgid needs to be updated. Otherwise it'd be the same as the shell's pgid and we'd kill the shell

We should restore signal handlers to the default after forking

