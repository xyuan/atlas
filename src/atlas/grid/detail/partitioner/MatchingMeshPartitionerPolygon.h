/*
 * (C) Copyright 1996-2016 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */


#pragma once

#include "MatchingMeshPartitioner.h"

namespace atlas {
namespace grid {
namespace detail {
namespace partitioner {


class MatchingMeshPartitionerPolygon : public MatchingMeshPartitioner {

public:

  static std::string static_type() {return "polygon";}

public:

    MatchingMeshPartitionerPolygon() : MatchingMeshPartitioner() {}
    MatchingMeshPartitionerPolygon(const size_t nb_partitions) : MatchingMeshPartitioner(nb_partitions) {}

    MatchingMeshPartitionerPolygon( const Mesh& mesh ) :
      MatchingMeshPartitioner(mesh) {}

    /**
     * @brief Create a GridDistribution, placing nodes in the same partitions as a
     * given pre-partitioned mesh. The method reconstructs the partition edges polygon
     * and tests every target grid node if it is internal to the polygon.
     * @param gridTarget grid to be distributed
     * @param meshSource mesh already partitioned
     * @return grid partitioner
     */
    void partition( const Grid&, int part[] ) const;

};


}  // partitioner
}  // detail
}  // grid
}  // atlas