
#include "atlas/grid/domain/Domain.h"
#include "atlas/util/Config.h"
#include "atlas/grid/projection/Projection.h"
#include "eckit/exception/Exceptions.h"

#warning TODO: Non-global grids may also include north-pole, south-pole, or be periodic-east-west

namespace atlas {
namespace grid {
namespace domain {

Domain *Domain::create() {
  // default: global domain
  util::Config projParams;
  projParams.set("type","global");
  return Domain::create(projParams);
}

Domain *Domain::create(const eckit::Parametrisation &p) {

  std::string domainType;
  if (p.get("type",domainType)) {
    return eckit::Factory<Domain>::instance().get(domainType).create(p);
  }

  // should return error here
  throw eckit::BadParameter("type missing in Params",Here());
}

bool Domain::includesNorthPole( const projection::Projection& proj ) const {
  double north_pole[] = {0.,90.};
  proj.lonlat2coords(north_pole);
  return contains(north_pole[0],north_pole[1]);
}

bool Domain::includesSouthPole( const projection::Projection& proj ) const {
  double south_pole[] = {0.,-90.};
  proj.lonlat2coords(south_pole);
  return contains(south_pole[0],south_pole[1]);
}

}  // namespace domain
}  // namespace grid
}  // namespace atlas
