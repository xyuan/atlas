/*
 * (C) Copyright 1996-2017 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#pragma once

#include <string>

#include "atlas/grid/detail/grid/Grid.h"
#include "atlas/grid/detail/grid/Structured.h"
#include "atlas/grid/detail/grid/Regular.h"
#include "atlas/grid/Projection.h"
#include "atlas/grid/Domain.h"

//---------------------------------------------------------------------------------------------------------------------

namespace atlas {
namespace grid {

//---------------------------------------------------------------------------------------------------------------------
// classes defined in this file

class Grid;
class StructuredGrid;
class RegularGrid;
class RegularGaussianGrid;
class ReducedGaussianGrid;
class RegularLonLatGrid;
class ShiftedLonLatGrid;

//---------------------------------------------------------------------------------------------------------------------

class Iterator {

public:

    Iterator( detail::grid::Grid::Iterator* iterator ):
        iterator_(iterator) {
    }

    bool next( PointXY& xy ) { return iterator_->next(xy); }

private:

    std::unique_ptr<detail::grid::Grid::Iterator> iterator_;
};

//---------------------------------------------------------------------------------------------------------------------

class Grid {

public:

  using grid_t   = detail::grid::Grid;
  using Config   = grid_t::Config;
  using Spec     = grid_t::Spec;

public:

    Grid();
    Grid( const Grid& );
    Grid( const grid_t* );
    Grid( const std::string& );
    Grid( const Config& );

    operator bool() const { return grid_; }
    operator const grid_t&() const { return *get(); }
    bool operator==( const Grid& other ) const { return grid_ == other.grid_; }
    bool operator!=( const Grid& other ) const { return grid_ != other.grid_; }

    size_t npts() const { return grid_->npts(); }

    const Projection& projection() const { return grid_->projection(); }
    const Domain& domain() const { return grid_->domain(); }
    std::string name() { return grid_->name(); }
    std::string uid() { return grid_->uniqueId(); }

    Spec spec() const { return grid_->spec(); }

    Iterator iterator() const { return grid_->iterator(); }

    const grid_t* get() const { return grid_.get(); }

private:

    eckit::SharedPtr<const grid_t> grid_;

};

//---------------------------------------------------------------------------------------------------------------------

class StructuredGrid: public Grid {

public:

  using structured_t = detail::grid::Structured;
  using YSpace = structured_t::YSpace;

public:

    StructuredGrid();
    StructuredGrid( const Grid& );
    StructuredGrid( const grid_t* );
    StructuredGrid( const std::string& );
    StructuredGrid( const Config& );

    operator bool() const { return grid_ && valid(); }

    inline size_t ny() const {
        return grid_->ny();
    }

    inline size_t nx( size_t j ) const {
        return grid_->nlon(j);;
    }

    inline const std::vector<long>& nx() const {
        return grid_->pl();
    }

    inline const long nxmax() const {
        return grid_->nlonmax();
    }

    inline const std::vector<double>& y() const {
        return grid_->latitudes();
    }

    inline double x( const size_t i, const size_t j ) const {
        return grid_->lon(j,i);
    }

    inline double y( const size_t j ) const {
        return grid_->lat(j);
    }

    void xy( const size_t i, const size_t j, double xy[] ) const {
      xy[0] = x(i,j);
      xy[1] = y(j) ;
    }

    PointXY xy( const size_t i, const size_t j ) const {
      return PointXY( x(i,j), y(j) );
    }

    PointLonLat lonlat( const size_t i, const size_t j ) const {
      return grid_->geolonlat(i,j);
    }

    inline bool reduced() const {
        return grid_->reduced();
    }

    bool periodic() const { return grid_->isPeriodicX(); }

    const YSpace& yspace() const { return grid_->yspace(); }

private:

    virtual bool valid() const { return true; }
    const structured_t* grid_;
};

//---------------------------------------------------------------------------------------------------------------------

class RegularGrid: public StructuredGrid {

public:

    using regular_t = structured_t;

public:

    RegularGrid();
    RegularGrid( const Grid& );
    RegularGrid( const grid_t* );
    RegularGrid( const std::string& );
    RegularGrid( const Config& );

    operator bool() const { return grid_ && valid(); }

    size_t nx() const { return nx_; }

    inline double x( const size_t i ) const {
        return StructuredGrid::x(i,0);
    }

    using StructuredGrid::y;

    inline double y( const size_t j ) const {
        return grid_->lat(j);
    }

    using StructuredGrid::xy;

    PointXY xy( const size_t i, const size_t j ) const {
      return PointXY( x(i), y(j ) );
    }

private:

    virtual bool valid() const { return true; }
    static regular_t* create( const Config& );
    const regular_t* grid_;
    size_t nx_ = {0};
};

//---------------------------------------------------------------------------------------------------------------------

class ReducedGaussianGrid : public StructuredGrid {

public:

    using StructuredGrid::StructuredGrid;

    long N() const { return ny()/2; }

private:

    virtual bool valid() const {
      return domain().global()
        &&   not projection()
        &&   yspace().type() == "gaussian";
    }
};

//---------------------------------------------------------------------------------------------------------------------

class RegularGaussianGrid : public RegularGrid {

public:

    using RegularGrid::RegularGrid;

    long N() const { return ny()/2; }

private:

    bool valid() const {
      return domain().global()
        &&   not projection()
        &&   yspace().type() == "gaussian";
    }
};

//---------------------------------------------------------------------------------------------------------------------

class RegularLonLatGrid : public RegularGrid {

public:

    using RegularGrid::RegularGrid;

private:

    bool valid() const {
      return domain().global()
        &&   not projection()
        &&   x(0) == 0.
        &&   y(0) == 90.
        &&   ny()%2 == 1;
    }
};

//---------------------------------------------------------------------------------------------------------------------

class ShiftedLonLatGrid : public RegularGrid {

public:

    using RegularGrid::RegularGrid;

private:

    bool valid() const {
      return domain().global()
        &&   not projection()
        &&   x(0) == 0.5*360./nx()
        &&   y(0) == 90.-0.5*180./ny()
        &&   ny()%2 == 0;
    }
};

//---------------------------------------------------------------------------------------------------------------------

}
}
