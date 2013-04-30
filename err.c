/*****************************************************************************
 *  Copyright (C) 2010-11 Lawrence Livermore National Security, LLC.
 *  Written by Jim Garlick <garlick@llnl.gov> LLNL-CODE-423279
 *  All Rights Reserved.
 *
 *  This file is part of the Distributed I/O Daemon (diod).
 *  For details, see <http://code.google.com/p/diod/>.
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License (as published by the
 *  Free Software Foundation) version 2, dated June 1991.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the IMPLIED WARRANTY OF MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the terms and conditions of the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software Foundation,
 *  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA or see
 *  <http://www.gnu.org/licenses/>.
 *****************************************************************************/

#if HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "err.h"

extern char *prog;

void
_verr (int errnum, const char *fmt, va_list ap)
{
    char buf[128];
    char errbuf[64];
    char *s = strerror (errnum);

    vsnprintf (buf, sizeof (buf), fmt, ap);  /* ignore overflow */
    fprintf (stderr, "%s: %s: %s\n", prog, buf, s);
}

void
_vmsg (const char *fmt, va_list ap)
{
    char buf[1024];

    vsnprintf (buf, sizeof (buf), fmt, ap);  /* ignore overflow */
    fprintf (stderr, "%s: %s\n", prog, buf);
}

void
err_exit (const char *fmt, ...)
{
    va_list ap;

    va_start (ap, fmt);
    _verr (errno, fmt, ap);
    va_end (ap);
    exit (1);
}

void
err (const char *fmt, ...)
{
    va_list ap;

    va_start (ap, fmt);
    _verr (errno, fmt, ap);
    va_end (ap);
}

void
errn_exit (int errnum, const char *fmt, ...)
{
    va_list ap;

    va_start (ap, fmt);
    _verr (errnum, fmt, ap);
    va_end (ap);
    exit (1);
}

void
errn (int errnum, const char *fmt, ...)
{
    va_list ap;

    va_start (ap, fmt);
    _verr (errnum, fmt, ap);
    va_end (ap);
}

void
msg_exit (const char *fmt, ...)
{
    va_list ap;

    va_start (ap, fmt);
    _vmsg (fmt, ap);
    va_end (ap);
    exit (1);
}

void
msg (const char *fmt, ...)
{
    va_list ap;

    va_start (ap, fmt);
    _vmsg (fmt, ap);
    va_end (ap);
}

/*
 * vi:tabstop=4 shiftwidth=4 expandtab
 */

