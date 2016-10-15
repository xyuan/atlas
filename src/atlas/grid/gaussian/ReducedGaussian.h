/*
 * (C) Copyright 1996-2016 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */


#ifndef atlas_grid_gaussian_ReducedGaussian_h
#define atlas_grid_gaussian_ReducedGaussian_h

#include "atlas/grid/gaussian/Gaussian.h"


namespace atlas {
namespace grid {
namespace gaussian {


/**
 * @brief Reduced Gaussian Grid
 *
 * This grid is a special case of the class Structured, in which
 * the latitudes are distributed according to the roots of the
 * Legendre Polynomials, and a equidistant distribution in zonal
 * direction, which reduce in number going closer towards poles,
 * essentially making the grid more uniform on the sphere
 * It can be constructed with following definition:
 * * N   = number of latitudes in hemisphere
 * * npts_per_lat[] = number of points on each latitude
 */
class ReducedGaussian : public Gaussian {

  public:

    static std::string grid_type_str();

    static std::string className();

    std::string gridType() const;

    ReducedGaussian(const size_t N, const long pl[], const Domain& domain=Domain::makeGlobal());

    ReducedGaussian(const eckit::Parametrisation&);

    virtual const Domain& domain() const {
        return domain_;
    }

  protected:

    ReducedGaussian() : Gaussian() {}

  private:

    std::string shortName() const;

  private:

    mutable std::string shortName_;

    Domain domain_;

};


}  // namespace gaussian
}  // namespace grid
}  // namespace atlas


#endif