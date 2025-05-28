/* Get the Linux TID from a pthread_t handle.
   Copyright (C) 2025 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 2.1 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If
   not, see <https://www.gnu.org/licenses/>.  */

#include <errno.h>
#include <pthreadP.h>

pid_t
attribute_hidden
pthread_gettid_np (pthread_t threadid)
{
  clockid_t clock;
  if (pthread_getcpuclockid (threadid, &clock) != 0)
    return -1;
  /* Reverse the clock ID encoding to obtain the TID.  This is part of
     the kernel/userspace interface, so it is stable ABI.  */
  return ~(clock >> 3);
}
