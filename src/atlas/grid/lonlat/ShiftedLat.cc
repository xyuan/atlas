/*
 * (C) Copyright 1996-2016 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */


#include "atlas/grid/lonlat/ShiftedLat.h"


namespace atlas {
namespace grid {
namespace lonlat {


eckit::ConcreteBuilderT1<Grid, ShiftedLat> builder_ShiftedLat(ShiftedLat::grid_type_str());


std::string ShiftedLat::grid_type_str() {
    return "shifted_lat";
}


std::string ShiftedLat::className() {
    return "atlas.grid.lonlat.ShiftedLat";
}


std::string ShiftedLat::gridType() const {
    return grid_type_str();
}


std::string ShiftedLat::shortName() const {
    if (shortName_.empty()) {
        std::stringstream s;
        if( N() ) {
            s << "Slat" << N();
        } else {
            s << "Slat" << nlon() << "x" << nlat();
        }
        if (!domain_.isGlobal()) {
            s << "-local";
        }
        shortName_ = s.str();
    }
    return shortName_;
}


ShiftedLat::ShiftedLat(const eckit::Parametrisation& params) :
    LonLat(Shift::LAT, Domain::makeGlobal()) {
    setup(params);
}


ShiftedLat::ShiftedLat(const size_t N, const Domain& domain) :
    LonLat(Shift::LAT, domain) {
    LonLat::setup(N, domain);
}


ShiftedLat::ShiftedLat(const size_t nlon, const size_t nlat, const Domain& domain) :
    LonLat(Shift::LAT, domain) {
    LonLat::setup(nlon, nlat, domain);
}


void ShiftedLat::setup(const eckit::Parametrisation& params) {
    size_t nlon, nlat, N(0);

    std::vector<double> p_domain(4);

    if( params.get("domain", p_domain) ) {
        domain_ = Domain(p_domain[0],p_domain[1],p_domain[2],p_domain[3]);
    } else {
        domain_ = Domain::makeGlobal();
    }

    if( params.get("N",N) ) {
        LonLat::setup(N, domain_);
    } else if( params.get("nlon", nlon) && params.get("nlat", nlat) ) {
        LonLat::setup(nlon, nlat, domain_);
    } else {
        throw eckit::BadParameter("Params (nlon,nlat) or N missing", Here());
    }
}


eckit::Properties ShiftedLat::spec() const {
    eckit::Properties grid_spec;
    grid_spec.set("grid_type",  gridType());
    grid_spec.set("short_name", shortName());
    grid_spec.set("N",    N());
    grid_spec.set("nlon", nlon());
    grid_spec.set("nlat", nlat());
    grid_spec.set("domain", domain_spec(domain_) );
    return grid_spec;
}


extern "C" {


    Structured* atlas__grid__lonlat__ShiftedLat(size_t nlon, size_t nlat) {
        return new ShiftedLat(nlon, nlat);
    }


}


}  // namespace lonlat
}  // namespace grid
}  // namespace atlas