# (C) Copyright 2013 ECMWF.
#
# This software is licensed under the terms of the Apache Licence Version 2.0
# which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
# In applying this licence, ECMWF does not waive the privileges and immunities
# granted to it by virtue of its status as an intergovernmental organisation nor
# does it submit to any jurisdiction.

if( ECKIT_INCLUDE_DIRS ) # eckit not yet ported to CMake3
    include_directories( ${ECKIT_INCLUDE_DIRS} )
endif()

ecbuild_add_executable(
  TARGET      atlas-main
  OUTPUT_NAME atlas
  SOURCES     atlas.cc
  LIBS        atlas )

ecbuild_add_executable(
  TARGET      atlas-meshgen
  SOURCES     atlas-meshgen.cc
  LIBS        atlas )

ecbuild_add_executable(
  TARGET      atlas-grids
  SOURCES     atlas-grids.cc
  LIBS        atlas )

ecbuild_add_executable(
  TARGET      atlas-gaussian-latitudes
  SOURCES     atlas-gaussian-latitudes.cc
  LIBS        atlas )

