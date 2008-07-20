/*
  This file is part of the SC Library.
  The SC Library provides support for parallel scientific applications.

  Copyright (C) 2007,2008 Carsten Burstedde, Lucas Wilcox.

  The SC Library is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  The SC Library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the SC Library.  If not, see <http://www.gnu.org/licenses/>.
*/

/* sc.h comes first in every compilation unit */
#include <sc.h>
#include <sc_blas.h>

const char          sc_transchar[] = { 'N', 'T', 'C' };
const char          sc_antitranschar[] = { 'T', 'N', '?' };
const char          sc_uplochar[] = { 'U', 'L', '?' };
const char          sc_cmachchar[] =
  { 'E', 'S', 'B', 'P', 'N', 'R', 'M', 'U', 'L', 'O' };

/* EOF sc_blas.c */