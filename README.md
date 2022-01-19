# log.c2

A simple logging library.

Log.c2 is based on [rxi/log.c](https://github.com/rxi/log.c) with [MIT LICENSE](https://github.com/rxi/log.c/blob/master/LICENSE) which is inactive now. Log.c has a very flexible and scalable architecture. There are some enhancements added in log.c2.

Enhancements:

- Print logs to stdout/stderr.
- Backup log file by file line number.
- Add more log levels.
- Enhance time accuracy.
- ...

## Usage
**[log.c](log.c?raw=1)** and **[log.h](log.h?raw=1)** should be dropped
into an existing project and compiled along with it. The library provides
function-like macros for logging:

```c
log_debug(const char *fmt, ...);
log_info(const char *fmt, ...);
log_notice(const char *fmt, ...);
log_warn(const char *fmt, ...);
log_error(const char *fmt, ...);
log_critical(const char *fmt, ...);
log_alert(const char *fmt, ...);
log_emergency(const char *fmt, ...);
```

Each function takes a printf format string followed by additional arguments:

```c
log_debug("Hello %s", "world")
```

Resulting in a line with the given format printed to stdout/stderr/file:

```
2020-08-11_16:05:03_877 [EMERGE] [demo_stdout.c:5  ]: this is a log.
```
#### log_stdout_verbose(int level)
Print log to stdout. Any messages above the given `level` are ignored.

#### log_stderr_verbose(int level)
Print log to stderr. Any messages above the given `level` are ignored.

#### log_add_fp(FILE *fp, int level)
One or more file pointers where the log will be written can be provided to the
library by using the `log_add_fp()` function. The data written to the file
output is of the following format:

```
2020-08-11_16:05:03_877 [EMERGE] [demo_stdout.c:5  ]: this is a log.
```

Any messages above the given `level` are ignored. If the library failed to add a
file pointer a value less-than-zero is returned.

#### log_add_file_backup(char *filename, size_t n, int level)
File where the log will be written can be backuped by file line number. The backup will be renamed with a timestamp tail. A new file will be opened to continue logging.

```
demo.log  demo.log_2021-08-10_16:31:18_857
```

#### log_add_callback(log_LogFn fn, void *udata, int level)
One or more callback functions which are called with the log data can be
provided to the library by using the `log_add_callback()` function. A callback
function is passed a `log_Event` structure containing the `line` number,
`filename`, `fmt` string, `va` printf va\_list, `level` and the given `udata`.

#### log_set_lock(log_LockFn fn, void *udata)
If the log will be written to from multiple threads a lock function can be set.
The function is passed the boolean `true` if the lock should be acquired or
`false` if the lock should be released and the given `udata` value.

#### const char* log_level_string(int level)
Returns the name of the given log level as a string.

#### LOG_USE_COLOR
If the library is compiled with `-DLOG_USE_COLOR` ANSI color escape codes will
be used when printing.

## Help

There are demos in `demo/`.
```shell
❯ make
❯ ./demo
===========================================================
  Usages: demo [demoname]
Demoname: stdout
Demoname: file
Demoname: file_backup
❯ ./demo stdout
...

```

## License
This library is free software; you can redistribute it and/or modify it under
the terms of the MIT license. See [LICENSE](LICENSE) for details.