/*
 * (C) Copyright 1996-2014 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */



#include <iostream>
#include <cmath>

#include "atlas/mesh/Mesh.h"
#include "atlas/mesh/FunctionSpace.h"
#include "atlas/actions/BuildPeriodicBoundaries.h"
#include "atlas/mesh/Parameters.h"
#include "atlas/mesh/Util.h"
#include "atlas/util/Array.h"
#include "atlas/util/ArrayView.h"
#include "atlas/util/IndexView.h"

namespace atlas {
namespace actions {

void build_periodic_boundaries( Mesh& mesh )
{
  int mypart = MPL::rank();

  FunctionSpace& nodes = mesh.function_space("nodes");

  IndexView<int,1> ridx ( nodes.field("remote_idx") );
  ArrayView<int,1> part ( nodes.field("partition")      );

  int nb_nodes = nodes.shape(0);

  ArrayView<double,2> latlon ( nodes.field("coordinates") );

  // Identify my master and slave nodes on own partition
  // master nodes are at x=0,  slave nodes are at x=2pi
  std::map<LatLonPoint,int> master_lookup;
  std::map<LatLonPoint,int>  slave_lookup;
  std::vector<int> master_nodes; master_nodes.reserve( 3*nb_nodes );
  std::vector<int>  slave_nodes;  slave_nodes.reserve( 3*nb_nodes );

  for( int jnode=0; jnode<nodes.shape(0); ++jnode)
  {
    int x = microdeg(latlon(jnode,XX));
    if( x == BC::WEST && (part(jnode)==mypart && jnode==ridx(jnode))  )
    {
      master_lookup[ LatLonPoint(latlon[jnode]) ] = jnode;
      master_nodes.push_back( x );
      master_nodes.push_back( microdeg(latlon(jnode,YY)) );
      master_nodes.push_back( jnode );
    }
    if( x >= BC::EAST )
    {
      slave_lookup[ LatLonPoint(latlon[jnode]) ] = jnode;
      slave_nodes.push_back( x );
      slave_nodes.push_back( microdeg(latlon(jnode,YY)) );
      slave_nodes.push_back( jnode );
      ridx( jnode ) = -1;
    }
  }

//  std::cout << "found " << master_nodes.size()/3 << " master nodes " << std::endl;
//  std::cout << "found " <<  slave_nodes.size()/3 << "  slave nodes " << std::endl;

                      //  std::vector< std::vector<int> > found_slave(MPL::size());
                      //  // Find slaves on other tasks to send to me
                      //  {
                      //    int sendcnt = master_nodes.size();
                      //    std::vector< int > recvcounts( MPL::size() );
                      //    MPL_CHECK_RESULT( MPI_Allgather(&sendcnt,           1, MPI_INT,
                      //                                     recvcounts.data(), 1, MPI_INT, MPI_COMM_WORLD ) );

                      //    std::vector<int> recvdispls( MPL::size() );
                      //    recvdispls[0] = 0;
                      //    int recvcnt = recvcounts[0];
                      //    for( int jproc=1; jproc<MPL::size(); ++jproc )
                      //    {
                      //      recvdispls[jproc] = recvdispls[jproc-1] + recvcounts[jproc-1];
                      //      recvcnt += recvcounts[jproc];
                      //    }
                      //    std::vector<int> recvbuf(recvcnt);

                      //    MPL_CHECK_RESULT( MPI_Allgatherv(
                      //                      master_nodes.data(), master_nodes.size(), MPI_INT,
                      //                      recvbuf.data(), recvcounts.data(), recvdispls.data(),
                      //                      MPI_INT, MPI_COMM_WORLD) );


                      //    for( int jproc=0; jproc<MPL::size(); ++jproc )
                      //    {
                      //      found_slave.reserve(slave_nodes.size());
                      //      ArrayView<int,2> recv_master_latlon(recvbuf.data()+recvdispls[jproc], Extents(recvcounts[jproc]/2,2).data() );
                      //      for( int jnode=0; jnode<recv_master_latlon.shape(0); ++jnode )
                      //      {
                      //        LatLon master( recv_master_latlon(jnode,XX)+east, recv_master_latlon(jnode,YY) );
                      //        if( slave_lookup.count( master ) )
                      //        {
                      //          int loc_idx = slave_lookup[ master ];
                      //          found_slave[jproc].push_back( loc_idx );
                      //        }
                      //      }
                      //    }
                      //  }

  std::vector< std::vector<int> > found_master(MPL::size());
  std::vector< std::vector<int> > send_slave_idx(MPL::size());
  // Find masters on other tasks to send to me
  {
    int sendcnt = slave_nodes.size();
    std::vector< int > recvcounts( MPL::size() );
    MPL_CHECK_RESULT( MPI_Allgather(&sendcnt,           1, MPI_INT,
                                     recvcounts.data(), 1, MPI_INT, MPI_COMM_WORLD ) );

    std::vector<int> recvdispls( MPL::size() );
    recvdispls[0] = 0;
    int recvcnt = recvcounts[0];
    for( int jproc=1; jproc<MPL::size(); ++jproc )
    {
      recvdispls[jproc] = recvdispls[jproc-1] + recvcounts[jproc-1];
      recvcnt += recvcounts[jproc];
    }
    std::vector<int> recvbuf(recvcnt);

    MPL_CHECK_RESULT( MPI_Allgatherv(
                      slave_nodes.data(), slave_nodes.size(), MPI_INT,
                      recvbuf.data(), recvcounts.data(), recvdispls.data(),
                      MPI_INT, MPI_COMM_WORLD) );


    for( int jproc=0; jproc<MPL::size(); ++jproc )
    {
      found_master.reserve(master_nodes.size());
      send_slave_idx.reserve(master_nodes.size());
      ArrayView<int,2> recv_slave(recvbuf.data()+recvdispls[jproc], make_shape(recvcounts[jproc]/3,3).data() );
      for( int jnode=0; jnode<recv_slave.shape(0); ++jnode )
      {
        LatLonPoint slave( recv_slave(jnode,XX)-BC::EAST, recv_slave(jnode,YY) );
        if( master_lookup.count( slave ) )
        {
          int master_idx = master_lookup[ slave ];
          int slave_idx  = recv_slave(jnode,2);
          found_master[jproc].push_back( master_idx );
          send_slave_idx[jproc].push_back( slave_idx );
         // std::cout << master_idx << " master of " << slave_idx << std::endl;
        }
      }
    }
  }

  // Fill in data to communicate
  std::vector< std::vector<int> > recv_slave_idx( MPL::size() );
  std::vector< std::vector<int> > send_master_part( MPL::size() );
  std::vector< std::vector<int> > recv_master_part( MPL::size() );
  std::vector< std::vector<int> > send_master_ridx( MPL::size() );
  std::vector< std::vector<int> > recv_master_ridx( MPL::size() );

                      //  std::vector< std::vector<int> > send_slave_part( MPL::size() );
                      //  std::vector< std::vector<int> > recv_slave_part( MPL::size() );
                      //  std::vector< std::vector<int> > send_slave_ridx( MPL::size() );
                      //  std::vector< std::vector<int> > recv_slave_ridx( MPL::size() );

  {
    for( int jproc=0; jproc<MPL::size(); ++jproc )
    {
      int nb_found_master = found_master[jproc].size();
      send_master_part   [jproc].resize(nb_found_master);
      send_master_ridx    [jproc].resize(nb_found_master);
      for( int jnode=0; jnode<nb_found_master; ++jnode )
      {
        int loc_idx = found_master[jproc][jnode];
        send_master_part[jproc][jnode] = part   ( loc_idx );
        send_master_ridx[jproc][jnode] = loc_idx;
      }

                      //      int nb_found_slaves = found_slave[jproc].size();
                      //      send_slave_glb_idx[jproc].resize(nb_found_slaves);
                      //      send_slave_part   [jproc].resize(nb_found_slaves);
                      //      send_slave_ridx   [jproc].resize(nb_found_slaves);
                      //      for( int jnode=0; jnode<nb_found_slaves; ++jnode )
                      //      {
                      //        int loc_idx = found_slave[jproc][jnode];
                      //        send_slave_glb_idx[jproc][jnode] = glb_idx( loc_idx );
                      //        send_slave_part   [jproc][jnode] = part   ( loc_idx );
                      //        send_slave_ridx   [jproc][jnode] = loc_idx;
                      //      }
    }
  }

  // Communicate
  MPL::Alltoall( send_slave_idx,      recv_slave_idx      );
  MPL::Alltoall( send_master_part,    recv_master_part    );
  MPL::Alltoall( send_master_ridx,    recv_master_ridx     );
                    //  MPL::Alltoall( send_slave_part,     recv_slave_part    );
                    //  MPL::Alltoall( send_slave_loc,      recv_slave_ridx    );

  // Fill in periodic
  int nb_recv_master = 0;
  for( int jproc=0; jproc<MPL::size(); ++jproc )
  {
    int nb_recv = recv_slave_idx[jproc].size();
    for( int jnode=0; jnode<nb_recv; ++jnode )
    {
      int slave_idx     = recv_slave_idx  [jproc][jnode];
      part( slave_idx ) = recv_master_part[jproc][jnode];
      ridx( slave_idx ) = recv_master_ridx[jproc][jnode];

      //std::cout << part( slave_idx ) <<"["<<ridx( slave_idx ) << "] master of " << mypart << "["<<slave_idx<<"]" << std::endl;

    }
  }

}

// ------------------------------------------------------------------
// C wrapper interfaces to C++ routines

void atlas__build_periodic_boundaries ( Mesh* mesh) {
  build_periodic_boundaries(*mesh);
}
// ------------------------------------------------------------------



} // namespace actions
} // namespace atlas
