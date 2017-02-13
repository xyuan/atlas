#ifndef atlas_grid_regular_ShiftedLonLat_h
#define atlas_grid_regular_ShiftedLonLat_h

#include "atlas/grid/regular/GlobalLonLat.h"
#include "atlas/grid/domain/Domain.h"
#include "atlas/grid/spacing/Spacing.h"
#include "atlas/grid/projection/Projection.h"

namespace atlas {
namespace grid {
namespace regular {

class ShiftedLonLat: public GlobalLonLat {

public:

    static std::string grid_type_str();

    static std::string className();

    virtual std::string shortName() const;
    virtual std::string gridType() const { return "shifted_lonlat"; }

    ShiftedLonLat(const util::Config& params);
    ShiftedLonLat(long nlon, long nlat);

};


}  // namespace regular
}  // namespace grid
}  // namespace atlas


#endif
