/*
  This file is part of the SC Library, version 3.
  The SC Library provides support for parallel scientific applications.

  Copyright (C) 2019 individual authors

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
*/

#include <sc3_openmp.h>
#ifdef SC_ENABLE_OPENMP
#include <omp.h>
#endif

int
sc3_openmp_get_max_threads (void)
{
#ifndef SC_ENABLE_OPENMP
  return 1;
#else
  return omp_get_max_threads ();
#endif
}

int
sc3_openmp_get_num_threads (void)
{
#ifndef SC_ENABLE_OPENMP
  return 1;
#else
  return omp_get_num_threads ();
#endif
}

int
sc3_openmp_get_thread_num (void)
{
#ifndef SC_ENABLE_OPENMP
  return 0;
#else
  return omp_get_thread_num ();
#endif
}

sc3_error_t        *
sc3_openmp_esync_pre_critical (int *rcount, int *ecount,
                               int *error_tid, sc3_error_t ** shared_error)
{
  int                 tmax = sc3_openmp_get_max_threads ();

  SC3E_RETVAL (rcount, 0);
  SC3E_RETVAL (ecount, 0);
  SC3E_RETVAL (error_tid, tmax);
  SC3E_RETVAL (shared_error, NULL);
  return NULL;
}

void
sc3_openmp_esync_in_critical (sc3_error_t * e, int *rcount, int *ecount,
                              int *error_tid, sc3_error_t ** shared_error)
{

  /* TODO input parameter checks */

  if (e != NULL) {
    int                 tid = sc3_openmp_get_thread_num ();

    if (*error_tid > tid) {
      /* we are the lowest numbered error thread */
      if (*shared_error != NULL) {
        if (sc3_error_destroy (shared_error) != NULL) {
          ++*rcount;
        }
      }
      *shared_error = e;
      *error_tid = tid;
    }
    else {
      /* another error thread has lower number */
      if (sc3_error_destroy (&e) != NULL) {
        ++*rcount;
      }
    }
    ++*ecount;
  }
}
