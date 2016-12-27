/* Copyright (C) 2000-2012 Peter Selinger.
   This file is part of ccrypt. It is free software and it is covered
   by the GNU general public license. See the file COPYING for details. */

/* traverse.h: functions for traversing through a list of files, optionally
   recursing through directory structure */
/* $Id: traverse.h 305 2012-10-11 12:41:22Z selinger $ */ 

#ifndef __TRAVERSE_H
#define __TRAVERSE_H

#include <sys/stat.h>
#include <dirent.h>

int traverse_toplevel(char **filelist, int count);

#endif /* __TRAVERSE_H */
