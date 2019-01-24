/* 
 * Copyright (C) 2019 Andrei Odintsov <forestryks1@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef _KJUDGE_H
#define _KJUDGE_H

#include <unistd.h>
#include <sys/prctl.h>

#define RLIMIT_MCPU				16

#define _W_MEMLIMIT				0x010000
#define _W_MEMLIMIT_STACK		0x020000
#define _W_MEMLIMIT_EXEC		0x040000
#define _W_TIMELIMIT			0x080000
#define _W_SECURITY_VIOLATION	0x100000
#define _W_CLEARMASK			0x0ffff

#define W_WAS_MEMLIMIT(x)			((x & _W_MEMLIMIT) != 0)
#define W_WAS_MEMLIMIT_STACK(x)		((x & _W_MEMLIMIT_STACK) != 0)
#define W_WAS_MEMLIMIT_EXEC(x)		((x & _W_MEMLIMIT_EXEC) != 0)
#define W_WAS_TIMELIMIT(x)			((x & _W_TIMELIMIT) != 0)
#define W_WAS_SECURITY_VIOLATION(x)	((x & _W_SECURITY_VIOLATION) != 0)
#define W_VIOLATION_ID(x)			(x >> 22)
#define W_CLEARBITS(x)				(x &= _W_CLEARMASK)

#define PR_ISOLATE				54
#define PR_MEMLIMITATION		0x0001
#define PR_TIMELIMITATION		0x0002
#define PR_SAFEMODE				0x0004

#define ru_maxvm				ru_isrss

#endif
