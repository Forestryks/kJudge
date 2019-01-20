/* Copyright (C) 2019 Andrei Odintsov <forestryks1@gmail.com>
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
#include <sys/syscall.h>

#define RLIMIT_MCPU				16
#define __NR_kj_isolate			333
#define SYS_kj_isolate			333

#define _W_MEMLIMIT				0x10000
#define _W_MEMLIMIT_STACK		0x20000
#define _W_MEMLIMIT_EXEC		0x40000
#define _W_TIMELIMIT			0x80000
#define _W_CLEARMASK			0x0ffff

#define W_WASMEMLIMIT(x)		((x & _W_MEMLIMIT) != 0)
#define W_WASMEMLIMIT_STACK(x)	((x & _W_MEMLIMIT_STACK) != 0)
#define W_WASMEMLIMIT_EXEC(x)	((x & _W_MEMLIMIT_EXEC) != 0)
#define W_WASTIMELIMIT(x)		((x & _W_TIMELIMIT) != 0)
#define W_CLEARBITS(x)			(x &= _W_CLEARMASK)

#define IMEMLIMITATION			0x0001
#define ITIMELIMITATION			0x0002

#define ru_maxvm				ru_isrss

long kj_isolate(unsigned int flags) {
	return syscall(__NR_kj_isolate, flags);
}

#endif
