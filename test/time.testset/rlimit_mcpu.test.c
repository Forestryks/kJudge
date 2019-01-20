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

/* rlimit_mcpu.test.c
 *
 * Checks whether we are able to set time limit in ms via
 * setrlimit(RLIMIT_MCPU)
 */

#include <sys/resource.h>
#include <sys/time.h>

#include <simple_test.h>
#include <kjudge.h>

#define TIME_LIMIT_MS (long)(1000) /* 1000ms == 1s */

int main() {
    struct rlimit rlim = {
        .rlim_cur = TIME_LIMIT_MS,
        .rlim_max = TIME_LIMIT_MS
    };

    ASSERT(setrlimit(RLIMIT_MCPU, &rlim) == 0)
    EXIT_SUCC();
}