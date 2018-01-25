/*
 * (C) Copyright 2013 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "atlas/parallel/omp/omp.h"

#ifdef _OPENMP
#include <omp.h>
#else
extern "C" void omp_set_num_threads(int num_threads);
extern "C" int omp_get_num_threads(void);
extern "C" int omp_get_max_threads(void);
extern "C" int omp_get_thread_num(void);
extern "C" int omp_get_num_procs(void);
extern "C" int omp_in_parallel(void);
extern "C" int omp_set_dynamic(int dynamic_threads);
extern "C" int omp_get_dynamic(void);
extern "C" void omp_set_nested(int nested);
extern "C" int omp_get_nested(void);
#endif

#ifdef ATLAS_HAVE_OMP
extern "C" {
#pragma weak omp_set_num_threads
#pragma weak omp_get_num_threads
#pragma weak omp_get_max_threads
#pragma weak omp_get_thread_num
#pragma weak omp_get_num_procs
#pragma weak omp_in_parallel
#pragma weak omp_set_dynamic
#pragma weak omp_get_dynamic
#pragma weak omp_set_nested
#pragma weak omp_get_nested
}
#endif

void atlas_omp_set_num_threads(int num_threads)
{
#ifdef ATLAS_HAVE_OMP
  if( omp_set_num_threads ) omp_set_num_threads(num_threads);
#endif
}

int atlas_omp_get_num_threads(void)
{
#ifdef ATLAS_HAVE_OMP
  if( omp_get_num_threads ) return omp_get_num_threads();
#endif
  return 1;
}

int atlas_omp_get_max_threads(void)
{
#ifdef ATLAS_HAVE_OMP
  if( omp_get_max_threads ) return omp_get_max_threads();
#endif
  return 1;
}

int atlas_omp_get_thread_num(void)
{
#ifdef ATLAS_HAVE_OMP
  if( omp_get_thread_num ) return omp_get_thread_num();
#endif
  return 0;
}

int atlas_omp_get_num_procs(void)
{
#ifdef ATLAS_HAVE_OMP
  if( omp_get_num_procs ) return omp_get_num_procs();
#endif
  return 1;
}

int atlas_omp_in_parallel(void)
{
#ifdef ATLAS_HAVE_OMP
  if( omp_in_parallel ) return omp_in_parallel();
#endif
  return 0;
}

void atlas_omp_set_dynamic(int dynamic_threads)
{
#ifdef ATLAS_HAVE_OMP
  if( omp_set_dynamic ) omp_set_dynamic(dynamic_threads);
#endif
}

int atlas_omp_get_dynamic(void)
{
#ifdef ATLAS_HAVE_OMP
  if( omp_get_dynamic ) return omp_get_dynamic();
#endif
  return 0;
}

void atlas_omp_set_nested(int nested)
{
#ifdef ATLAS_HAVE_OMP
  if( omp_set_nested ) omp_set_nested(nested);
#endif
}

int atlas_omp_get_nested(void)
{
#ifdef ATLAS_HAVE_OMP
  if( omp_get_nested ) return omp_get_nested();
#endif
  return 0;
}

