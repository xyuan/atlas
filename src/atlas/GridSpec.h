/*
 * (C) Copyright 1996-2014 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#ifndef atlas_GridSpec_H
#define atlas_GridSpec_H

#include "eckit/value/Properties.h"
#include "eckit/utils/MD5.h"

#include "atlas/Grid.h"


namespace atlas {

//------------------------------------------------------------------------------------------------------


/// A concrete class that holds specification that uniquely identifies a Grid
/// The description of the grid is added as name value pairs
/// This class will provides a short name for a GRID (i.e QG48_1)
/// This allows for easier matching with samples files.
/// However this interface is independent of GRIB/NETCDF, because:
///
///      DECODE                   ATLAS                      ENCODE
///  NetCDFParams ---->|-------|         |----------|------>NetCDFGridWrite
///                    | Grid  |<------> | GridSpec |
///  GribParams ------>|-------|         |----------|------>GribGridWrite
///
/// Uses default copy constructor, assignment and equality operators.
///
/// This class also implements the Params interface.


class GridSpec : public eckit::Properties {
public:

  GridSpec(const std::string& grid_type);
  GridSpec(eckit::Stream& s);

  ~GridSpec();

  /// returns the gridType. currently this matches grid _type found in GRIB
  std::string grid_type() const;

  eckit::MD5::digest_t hash() const;

  static const char* className() { return "atlas::GridSpec"; }

  void set_latitudes(const std::vector<double>& latitudes_vec);

  void set_npts_per_lat(const std::vector<int>& rgSpec);
  void set_bounding_box(const BoundBox& bbox );

  void get_npts_per_lat(std::vector<int>& rgSpec) const;
  void get_bounding_box(BoundBox& bbox ) const;

  std::string str() const;

  bool operator==(const GridSpec& rhs) { return str() == rhs.str();  }
  bool operator!=(const GridSpec& rhs) { return !( (*this) == rhs ); }

  friend std::ostream& operator<<( std::ostream& os, const GridSpec& v) { v.print(os); return os;}

private:

  friend eckit::Params::value_t getValue( const GridSpec& p, const eckit::Params::key_t& key );
  friend void print( const GridSpec& p, std::ostream& s );
  friend void encode( const GridSpec& p, eckit::Stream& s );

  void print( std::ostream& ) const;
};

//------------------------------------------------------------------------------------------------------


} // namespace atlas

#endif
