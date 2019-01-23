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

/*
 * exec_target.test.cc
 *
 * Executed by some tests to check what happens on exec of program with
 * large data and BSS segments
 * 
 * .cc file extension is used to only compile and not run this file as test
 * TODO: fix it
 */

#include <simple_test.h>
#include <kjudge.h>

#define	SIZE (long)(50 * 1024 * 1024) /* 50 MB */

char a[SIZE];

int main() {
	return 0;
}
