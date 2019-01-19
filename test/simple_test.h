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

#ifndef _SIMPLE_TEST_H
#define _SIMPLE_TEST_H

#include <stdio.h>
#include <stdlib.h>

#define VA_ARGS(...)	, ##__VA_ARGS__
#define LOG(s, ...)		printf("[%s:%d] " s "\n", __FILE__, __LINE__ VA_ARGS(__VA_ARGS__))
#define FAIL(s, ...)	do { printf("[%s:%d] Epic fail! " s "\n", __FILE__, __LINE__ VA_ARGS(__VA_ARGS__)); exit(-1); } while (0)
#define ASSERT(x)		do { if (!(x)) {FAIL("Assertation \"" #x "\" failed");} } while (0);
#define EXIT_SUCC()			do { LOG("Test passed successfully"); exit(0); } while (0)

#endif
