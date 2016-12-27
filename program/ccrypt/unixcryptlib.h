/* Copyright (C) 2000-2012 Peter Selinger.
   This file is part of ccrypt. It is free software and it is covered
   by the GNU general public license. See the file COPYING for details. */

/* unixcryptlib.c: library to simulate old "unix crypt" program */
/* $Id: unixcryptlib.h 305 2012-10-11 12:41:22Z selinger $ */

/* WARNING: do not use this software for encryption! The encryption
   provided by this program has been broken and is not secure. Only
   use this software to decrypt existing data. */

#ifndef _UNIXCRYPTLIB_H
#define _UNIXCRYPTLIB_H

#include "ccryptlib.h"

int unixcrypt_init(ccrypt_stream_t *b, const char *key);
int unixcrypt(ccrypt_stream_t *b);
int unixcrypt_end(ccrypt_stream_t *b);

#endif /* _UNIXCRYPTLIB_H */
