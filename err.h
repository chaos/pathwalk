void err_exit (const char *fmt, ...)
        __attribute__ ((format (printf, 1, 2), noreturn));
void err (const char *fmt, ...)
        __attribute__ ((format (printf, 1, 2)));
void errn_exit (int errnum, const char *fmt, ...)
        __attribute__ ((format (printf, 2, 3), noreturn));
void errn (int errnum, const char *fmt, ...)
        __attribute__ ((format (printf, 2, 3)));
void msg (const char *fmt, ...)
        __attribute__ ((format (printf, 1, 2)));
void msg_exit (const char *fmt, ...)
        __attribute__ ((format (printf, 1, 2), noreturn));
