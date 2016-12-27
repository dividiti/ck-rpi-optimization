/* Copyright (C) 2000-2012 Peter Selinger.
   This file is part of ccrypt. It is free software and it is covered
   by the GNU general public license. See the file COPYING for details. */

/* readkey.h */
/* $Id: readkey.h 314 2012-10-15 16:35:56Z selinger $ */

#ifndef __READKEY_H
#define __READKEY_H

char *readkey(const char *prompt, const char *promptcont, const char *myname, int echo);

#endif /* __READKEY_H */
