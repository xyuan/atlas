/*
 * (C) Copyright 1996-2016 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#ifndef atlas_Metadata_h
#define atlas_Metadata_h

#include <string>
#include "eckit/memory/SharedPtr.h"
#include "eckit/value/Properties.h"
#include "eckit/config/Parametrisation.h"

namespace atlas { namespace grid { class Grid; } }
namespace atlas { namespace mesh { class Mesh; } }

namespace atlas {
namespace util {

class Metadata: public eckit::Properties {

public:

  Metadata(): eckit::Properties() {}

  Metadata(const eckit::Properties& p): eckit::Properties(p) {}
  Metadata(const Metadata& p): eckit::Properties(p) {}

  template<typename ValueT>
  Metadata& set(const std::string& name, const ValueT& value);

  Metadata& set(const std::string& name, const char* value);

  Metadata& set( const eckit::Properties& p );

  template<typename ValueT>
  ValueT get(const std::string& name) const;

  template<typename ValueT>
  bool get(const std::string& name, ValueT& value) const;

  bool has( const std::string& name) const;

  friend std::ostream& operator<<(std::ostream& s, const Metadata& v) { v.print(s);  return s; }

  void broadcast();
  void broadcast(const size_t root);
  void broadcast(Metadata&);
  void broadcast(Metadata&, const size_t root);
  void broadcast(Metadata&) const;
  void broadcast(Metadata&, const size_t root) const;
  
  size_t footprint() const;
};

// ------------------------------------------------------------------
// C wrapper interfaces to C++ routines

#define Channel std::ostream
#define Char char
#define grid_Grid grid::Grid
#define mesh_Mesh mesh::Mesh

extern "C"
{
  Metadata* atlas__Metadata__new ();

  void atlas__Metadata__delete (Metadata* This);
  void atlas__Metadata__print (Metadata* This, Channel* channel);
  void atlas__Metadata__json (Metadata* This, Char* &json, int &size, int &allocated);
  int  atlas__Metadata__has (Metadata* This, const char* name);
  void atlas__Metadata__set_int    (Metadata* This, const char* name, int value);
  void atlas__Metadata__set_long   (Metadata* This, const char* name, long value);
  void atlas__Metadata__set_float  (Metadata* This, const char* name, float value);
  void atlas__Metadata__set_double (Metadata* This, const char* name, double value);
  void atlas__Metadata__set_string (Metadata* This, const char* name, const char* value);
  void atlas__Metadata__set_array_int    (Metadata* This, const char* name, int value[], int size);
  void atlas__Metadata__set_array_long   (Metadata* This, const char* name, long value[], int size);
  void atlas__Metadata__set_array_float  (Metadata* This, const char* name, float value[], int size);
  void atlas__Metadata__set_array_double (Metadata* This, const char* name, double value[], int size);
  void atlas__Metadata__set_grid (Metadata* This, const char* name, grid_Grid* value);
  void atlas__Metadata__set_mesh (Metadata* This, const char* name, mesh_Mesh* value);

  int    atlas__Metadata__get_int    (Metadata* This, const char* name);
  long   atlas__Metadata__get_long   (Metadata* This, const char* name);
  float  atlas__Metadata__get_float  (Metadata* This, const char* name);
  double atlas__Metadata__get_double (Metadata* This, const char* name);
  void   atlas__Metadata__get_string (Metadata* This, const char* name, char* output_str, int max_len);
  void   atlas__Metadata__get_array_int    (Metadata* This, const char* name, int* &value, int &size, int &allocated);
  void   atlas__Metadata__get_array_long   (Metadata* This, const char* name, long* &value, int &size, int &allocated);
  void   atlas__Metadata__get_array_float  (Metadata* This, const char* name, float* &value, int &size, int &allocated);
  void   atlas__Metadata__get_array_double (Metadata* This, const char* name, double* &value, int &size, int &allocated);
  grid_Grid*  atlas__Metadata__get_grid (Metadata* This, const char* name);
  mesh_Mesh*  atlas__Metadata__get_mesh (Metadata* This, const char* name);
}
#undef Channel
#undef Char
#undef grid_Grid
#undef mesh_Mesh

// ------------------------------------------------------------------

} // namespace util
} // namespace atlas

#endif // Metadata_h