/*
 * (C) Copyright 1996-2017 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include <sstream>
#include <algorithm>
#include <iomanip>

#include "atlas/parallel/mpi/mpi.h"
#include "atlas/library/config.h"
#include "tests/TestMeshes.h"
#include "atlas/output/Gmsh.h"
#include "atlas/mesh/Mesh.h"
#include "atlas/mesh/Nodes.h"
#include "atlas/array/IndexView.h"
#include "atlas/array/ArrayView.h"
#include "atlas/array.h"
#include "atlas/mesh/actions/BuildHalo.h"
#include "atlas/mesh/actions/BuildParallelFields.h"
#include "atlas/mesh/actions/BuildPeriodicBoundaries.h"
#include "atlas/mesh/actions/BuildEdges.h"
#include "atlas/mesh/actions/BuildDualMesh.h"
#include "atlas/util/CoordinateEnums.h"
#include "atlas/util/MicroDeg.h"
#include "atlas/util/Unique.h"
#include "atlas/mesh/IsGhostNode.h"
#include "eckit/types/FloatCompare.h"

#include "tests/AtlasTestEnvironment.h"
#include "eckit/testing/Test.h"

using namespace eckit::testing;
using namespace atlas::output;
using namespace atlas::util;
using namespace atlas::meshgenerator;

namespace atlas {
namespace test {

double dual_volume(Mesh& mesh)
{
  mesh::Nodes& nodes = mesh.nodes();
  mesh::IsGhostNode is_ghost_node(nodes);
  int nb_nodes = nodes.size();
  array::ArrayView<double,1> dual_volumes = array::make_view<double,1>( nodes.field("dual_volumes") );
  double area=0;
  for( int node=0; node<nb_nodes; ++node )
  {
    if( ! is_ghost_node(node) )
    {
      area += dual_volumes(node);
    }
  }

  ATLAS_MPI_STATS( ALLREDUCE ) {
    parallel::mpi::comm().allReduceInPlace(area, eckit::mpi::sum());
  }

  return area;
}

#if 0
CASE( "test_small" )
{
  int nlat = 5;
  int lon[5] = {10, 12, 14, 16, 16};

  Mesh = test::generate_mesh(nlat, lon);

  mesh::actions::build_parallel_fields(*m);
  mesh::actions::build_periodic_boundaries(*m);
  mesh::actions::build_halo(*m,2);


  if( parallel::mpi::comm().size() == 5 )
  {
    IndexView<int,1> ridx ( m->nodes().remote_index() );
    array::array::ArrayView<gidx_t,1> gidx ( m->nodes().global_index() );

    switch( parallel::mpi::comm().rank() ) // with 5 tasks
    {
    case 0:
      EXPECT( ridx(9) ==  9  );
      EXPECT( gidx(9) ==  10 );
      EXPECT( ridx(30) == 9 );
      EXPECT( gidx(30) == 875430066 ); // hashed unique idx
      break;
    }
  }
  else
  {
    if( parallel::mpi::comm().rank() == 0 )
      std::cout << "skipping tests with 5 mpi tasks!" << std::endl;
  }

  mesh::actions::build_edges(*m);
  mesh::actions::build_median_dual_mesh(*m);

  EXPECT( eckit::types::is_approximately_equal( test::dual_volume(*m), 2.*M_PI*M_PI, 1e-6 ));

  std::stringstream filename; filename << "small_halo_p" << parallel::mpi::comm().rank() << ".msh";
  Gmsh(filename.str()).write(*m);
}
#endif

#if 1
CASE( "test_t63" )
{
  // Mesh m = test::generate_mesh( T63() );

  Mesh m = test::generate_mesh( {10, 12, 14, 16, 16, 16, 16, 14, 12, 10} );

  mesh::actions::build_nodes_parallel_fields(m.nodes());
  mesh::actions::build_periodic_boundaries(m);
  mesh::actions::build_halo(m,1);
  //mesh::actions::build_edges(m);
  //mesh::actions::build_pole_edges(m);
  //mesh::actions::build_edges_parallel_fields(m.function_space("edges"),m-.nodes());
  //mesh::actions::build_centroid_dual_mesh(m);
  mesh::actions::renumber_nodes_glb_idx(m.nodes());

  std::stringstream filename; filename << "T63_halo.msh";
  Gmsh(filename.str(),util::Config("ghost",true)).write(m);

//  EXPECT( eckit::types::is_approximately_equal( test::dual_volume(m), 2.*M_PI*M_PI, 1e-6 ));

  auto lonlat = array::make_view<double,2>( m.nodes().lonlat() );

  std::vector<uidx_t> check;
  switch( parallel::mpi::comm().rank() ) {
  case 0: check = {
      607990293346953216,
      607990293382953216,
      607990293418953216,
      607990293454953216,
      607990293490953216,
      607990293526953216,
      607990293562953216,
      607990293598953216,
      607990293634953216,
      607990293670953216,
      644481443595331584,
      644481443625331584,
      644481443655331584,
      644481443685331584,
      644481443715331584,
      644481443745331584,
      644481443775331584,
      644481443805331584,
      644481443835331584,
      644481443865331584,
      644481443895331584,
      644481443925331584,
      681187136050079744,
      681187136075794030,
      681187136101508315,
      681187136127222601,
      681187136152936887,
      681187136178651173,
      607990293706953216,
      644481443955331584,
      681187136204365458,
      681187136230079744,
      681187136255794030,
      681187136281508315,
      681187136307222601,
      681187136332936887,
      681187136358651173,
      681187136384365458,
      681187136410079744,
      717939789677242368,
      717939789699742368,
      717939789722242368,
      717939789744742368,
      717939789767242368,
      717939789789742368,
      717939789812242368,
      754708008265885696,
      754708008288385696,
      754708008310885696,
      754708008333385696,
      754708008355885696,
      754708008378385696,
      754708008400885696,
      717939789834742368,
      717939789857242368,
      717939789879742368,
      717939789902242368,
      754708008423385696,
      717939789924742368,
      717939789947242368,
      717939789969742368,
      717939789992242368,
      717939790014742368,
      717939790037242368,
      607990293310953217,
      644481443565331585,
      681187136024365459,
      717939789654742369,
      754708008243385697,
      607990293742953216,
      644481443985331584,
      681187136435794030,
      717939790059742368
    };
    break;
  case 1:
    check = {
      717939789677242368,
      717939789699742368,
      717939789722242368,
      717939789744742368,
      717939789767242368,
      717939789789742368,
      754708008265885696,
      754708008288385696,
      754708008310885696,
      754708008333385696,
      754708008355885696,
      754708008378385696,
      791480219026630656,
      791480219049130656,
      791480219071630656,
      791480219094130656,
      791480219116630656,
      828248437615273984,
      828248437637773984,
      828248437660273984,
      828248437682773984,
      828248437705273984,
      865001091242436608,
      865001091268150894,
      865001091293865179,
      865001091319579465,
      865001091345293751,
      717939789812242368,
      754708008400885696,
      791480219139130656,
      828248437727773984,
      681187136050079744,
      681187136075794030,
      681187136101508315,
      681187136127222601,
      681187136152936887,
      681187136178651173,
      681187136204365458,
      717939789834742368,
      754708008423385696,
      791480219161630656,
      791480219184130656,
      828248437750273984,
      865001091371008037,
      901706783697184768,
      901706783727184768,
      901706783757184768,
      901706783787184768,
      901706783817184768,
      717939789654742369,
      754708008243385697,
      791480219004130657,
      828248437592773985,
      865001091216722323
    };
    break;
  case 2:
    check = {
      681187136204365458,
      681187136230079744,
      681187136255794030,
      717939789812242368,
      717939789834742368,
      717939789857242368,
      717939789879742368,
      717939789902242368,
      754708008400885696,
      754708008423385696,
      754708008445885696,
      754708008468385696,
      754708008490885696,
      791480219139130656,
      791480219161630656,
      791480219184130656,
      791480219206630656,
      791480219229130656,
      828248437727773984,
      828248437750273984,
      828248437772773984,
      828248437795273984,
      828248437817773984,
      865001091371008037,
      865001091396722322,
      865001091422436608,
      865001091448150894,
      681187136281508315,
      717939789924742368,
      754708008378385696,
      754708008513385696,
      791480219116630656,
      791480219251630656,
      828248437705273984,
      828248437840273984,
      865001091345293751,
      644481443745331584,
      644481443775331584,
      644481443805331584,
      644481443835331584,
      681187136178651173,
      681187136307222601,
      717939789789742368,
      717939789767242368,
      754708008333385696,
      754708008355885696,
      791480219094130656,
      828248437682773984,
      865001091319579465,
      717939789947242368,
      754708008535885696,
      791480219274130656,
      791480219296630656,
      828248437862773984,
      865001091473865179,
      901706783787184768,
      901706783817184768,
      901706783847184768,
      901706783877184768,
      901706783907184768
    };
    break;
  case 3:
    check = {
      681187136281508315,
      681187136307222601,
      681187136332936887,
      681187136358651173,
      681187136384365458,
      717939789924742368,
      717939789947242368,
      717939789969742368,
      717939789992242368,
      717939790014742368,
      754708008513385696,
      754708008535885696,
      754708008558385696,
      754708008580885696,
      754708008603385696,
      791480219251630656,
      791480219274130656,
      791480219296630656,
      791480219319130656,
      791480219341630656,
      791480219364130656,
      828248437840273984,
      828248437862773984,
      828248437885273984,
      828248437907773984,
      828248437930273984,
      828248437952773984,
      681187136410079744,
      717939789902242368,
      717939790037242368,
      754708008490885696,
      754708008625885696,
      791480219229130656,
      791480219386630656,
      828248437817773984,
      828248437975273984,
      644481443805331584,
      644481443835331584,
      644481443865331584,
      644481443895331584,
      644481443925331584,
      644481443955331584,
      681187136255794030,
      717939789879742368,
      754708008445885696,
      754708008468385696,
      791480219206630656,
      828248437795273984,
      865001091422436608,
      865001091448150894,
      865001091473865179,
      865001091499579465,
      865001091525293751,
      865001091551008037,
      865001091576722322,
      865001091602436608,
      681187136435794030,
      717939790059742368,
      754708008648385696,
      791480219409130656,
      828248437997773984
    };
    break;
  case 4:
    check = {
      865001091473865179,
      865001091499579465,
      865001091525293751,
      865001091551008037,
      865001091576722322,
      901706783697184768,
      901706783727184768,
      901706783757184768,
      901706783787184768,
      901706783817184768,
      901706783847184768,
      901706783877184768,
      901706783907184768,
      901706783937184768,
      901706783967184768,
      901706783997184768,
      901706784027184768,
      938197933945563136,
      938197933981563136,
      938197934017563136,
      938197934053563136,
      938197934089563136,
      938197934125563136,
      938197934161563136,
      938197934197563136,
      938197934233563136,
      938197934269563136,
      828248437840273984,
      828248437862773984,
      828248437885273984,
      828248437907773984,
      828248437930273984,
      828248437952773984,
      828248437975273984,
      865001091242436608,
      865001091268150894,
      865001091293865179,
      865001091319579465,
      865001091345293751,
      865001091371008037,
      865001091396722322,
      865001091422436608,
      865001091448150894,
      865001091602436608,
      901706784057184768,
      938197934305563136,
      828248437615273984,
      828248437637773984,
      828248437660273984,
      828248437682773984,
      828248437705273984,
      828248437727773984,
      828248437750273984,
      828248437772773984,
      828248437795273984,
      828248437817773984,
      791480219229130656,
      791480219251630656,
      791480219274130656,
      791480219296630656,
      791480219319130656,
      791480219341630656,
      791480219364130656,
      791480219386630656,
      828248437592773985,
      865001091216722323,
      901706783667184769,
      938197933909563137,
      791480219409130656,
      828248437997773984,
      865001091628150894,
      901706784087184768,
      938197934341563136
    };
    break;
  default:
    check.clear();
  }
  std::vector<uidx_t> uid(m.nodes().size());
  for( size_t j=0; j<m.nodes().size(); ++j ){
    uid[j] = util::unique_lonlat(lonlat[j]);
  }
  if( check.size() && parallel::mpi::comm().size() == 5 ) {
    ATLAS_DEBUG_VAR( uid.size() );
    ATLAS_DEBUG_VAR( check.size() );
    EXPECT( uid.size() == check.size() );
    EXPECT( uid == check );
  }

//  FunctionSpace& edges = m.function_space("edges");
//  array::array::ArrayView<double,1> dual_volumes  ( nodes.field( "dual_volumes" ) );
//  array::array::ArrayView<double,2> dual_normals  ( edges.field( "dual_normals" ) );

//  std::string checksum;
//  checksum = nodes.checksum()->execute(dual_volumes);
//  DEBUG("dual_volumes checksum "<<checksum,0);

//  checksum = edges.checksum()->execute(dual_normals);
//  DEBUG("dual_normals checksum "<<checksum,0);

}
#endif
//-----------------------------------------------------------------------------

}  // namespace test
}  // namespace atlas


int main(int argc, char **argv) {
    atlas::test::AtlasTestEnvironment env( argc, argv );
    return run_tests ( argc, argv, false );
}
