/*
 * (C) Copyright 2013 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation
 * nor
 * does it submit to any jurisdiction.
 */

#include "atlas/array.h"
#include "atlas/array/MakeView.h"
#include "atlas/library/config.h"
#include "eckit/memory/SharedPtr.h"
#include "tests/AtlasTestEnvironment.h"

#ifdef ATLAS_HAVE_GRIDTOOLS_STORAGE
#if ATLAS_GRIDTOOLS_STORAGE_BACKEND_CUDA
#define PADDED 1
#else
#define PADDED 0
#endif
#else
#define PADDED 0
#endif
#define NOT_PADDED 1 - PADDED

using namespace atlas::array;

namespace atlas {
namespace test {

//-----------------------------------------------------------------------------

#ifdef ATLAS_HAVE_GRIDTOOLS_STORAGE
CASE( "test_array" ) {
    Array* ds = Array::create<double>( 4ul );
    auto hv   = atlas::array::gridtools::make_gt_host_view<double, 1>( *ds );
    hv( 3 )   = 4.5;

    atlas::array::ArrayView<double, 1> atlas_hv = make_host_view<double, 1>( *ds );

    EXPECT( hv( 3 ) == 4.5 );
    EXPECT( atlas_hv( 3 ) == 4.5 );

    delete ds;
}
#endif

CASE( "test_array_zero_size" ) {
    Array* ds = Array::create<double>( 0 );

    EXPECT( ds->size() == 0 );
    delete ds;
}

#ifdef ATLAS_HAVE_GRIDTOOLS_STORAGE
CASE( "test_create" ) {
    Array* ds  = Array::create( array::DataType::create<int>(), ArrayShape( {4, 3} ) );
    auto hv    = atlas::array::gridtools::make_gt_host_view<int, 2>( *ds );
    hv( 3, 2 ) = 4;

    atlas::array::ArrayView<int, 2> atlas_hv = make_host_view<int, 2>( *ds );

    EXPECT( hv( 3, 2 ) == 4 );
    EXPECT( atlas_hv( 3, 2 ) == 4 );

    delete ds;
}
#endif

#ifdef ATLAS_HAVE_GRIDTOOLS_STORAGE
CASE( "test_make_view" ) {
    Array* ds = Array::create<double>( 4ul );
    auto hv   = atlas::array::gridtools::make_gt_host_view<double, 1>( *ds );
    hv( 3 )   = 4.5;

    atlas::array::ArrayView<double, 1> atlas_hv = make_view<double, 1>( *ds );

    EXPECT( hv( 3 ) == 4.5 );
    EXPECT( atlas_hv( 3 ) == 4.5 );

    delete ds;
}
#endif

CASE( "test_localview" ) {
    Array* ds = Array::create<double>( 8ul, 4ul, 2ul );
    auto hv   = make_host_view<double, 3>( *ds );

    EXPECT( hv.shape( 0 ) == 8ul );
    EXPECT( hv.shape( 1 ) == 4ul );
    EXPECT( hv.shape( 2 ) == 2ul );
    EXPECT( hv.size() == 8ul * 4ul * 2ul );

    // Initialize fields
    for ( size_t i = 0; i < ds->shape( 0 ); ++i ) {
        for ( size_t j = 0; j < ds->shape( 1 ); ++j ) {
            for ( size_t k = 0; k < ds->shape( 2 ); ++k ) {
                hv( i, j, k ) = ( i * 100 ) + ( j * 10 ) + ( k );
            }
        }
    }

    // Check values
    for ( size_t i = 0; i < ds->shape( 0 ); ++i ) {
        LocalView<double, 2> lv = hv.slice( i, Range::all(), Range::all() );
        for ( size_t j = 0; j < lv.shape( 0 ); ++j ) {
            for ( size_t k = 0; k < lv.shape( 1 ); ++k ) {
                EXPECT( lv( j, k ) == ( i * 100 ) + ( j * 10 ) + ( k ) );
            }
        }
    }

    delete ds;
}

#ifdef ATLAS_HAVE_GRIDTOOLS_STORAGE
CASE( "test_array_shape" ) {
    ArrayShape as{2, 3};
    Array* ds                                   = Array::create<double>( as );
    auto gt_hv                                  = atlas::array::gridtools::make_gt_host_view<double, 2>( *ds );
    atlas::array::ArrayView<double, 2> atlas_hv = make_host_view<double, 2>( *ds );

    gt_hv( 1, 1 ) = 4.5;

    EXPECT( gt_hv( 1, 1 ) == 4.5 );
    EXPECT( atlas_hv( 1, 1 ) == 4.5 );

    EXPECT( ds->size() == 6 );
    EXPECT( ds->rank() == 2 );
    EXPECT( ds->stride( 0 ) == gt_hv.storage_info().stride<0>() );
    EXPECT( ds->stride( 1 ) == gt_hv.storage_info().stride<1>() );
    EXPECT( ds->contiguous() == NOT_PADDED );
    delete ds;
}
#endif

CASE( "test_spec" ) {
    Array* ds = Array::create<double>( 4, 5, 6 );
    EXPECT( ds->spec().rank() == 3 );
    EXPECT( ds->spec().size() == 4 * 5 * 6 );
    EXPECT( ds->spec().shape()[0] == 4 );
    EXPECT( ds->spec().shape()[1] == 5 );
    EXPECT( ds->spec().shape()[2] == 6 );
    EXPECT( ds->spec().shapef()[0] == 6 );
    EXPECT( ds->spec().shapef()[1] == 5 );
    EXPECT( ds->spec().shapef()[2] == 4 );

    if ( NOT_PADDED ) {
        EXPECT( ds->spec().strides()[0] == 6 * 5 );
        EXPECT( ds->spec().strides()[1] == 6 );
        EXPECT( ds->spec().strides()[2] == 1 );
    }
    EXPECT( ds->spec().hasDefaultLayout() == true );

    delete ds;
}

CASE( "test_spec_layout" ) {
    Array* ds = Array::create<double>( make_shape( 4, 5, 6 ), make_layout( 0, 1, 2 ) );
    EXPECT( ds->spec().rank() == 3 );
    EXPECT( ds->spec().size() == 4 * 5 * 6 );
    EXPECT( ds->spec().shape()[0] == 4 );
    EXPECT( ds->spec().shape()[1] == 5 );
    EXPECT( ds->spec().shape()[2] == 6 );
    EXPECT( ds->spec().shapef()[0] == 6 );
    EXPECT( ds->spec().shapef()[1] == 5 );
    EXPECT( ds->spec().shapef()[2] == 4 );
    if ( NOT_PADDED ) {
        EXPECT( ds->spec().strides()[0] == 6 * 5 );
        EXPECT( ds->spec().strides()[1] == 6 );
        EXPECT( ds->spec().strides()[2] == 1 );
        EXPECT( ds->spec().size() == ds->spec().allocatedSize() );
    }
    EXPECT( ds->spec().hasDefaultLayout() == true );
    EXPECT( ds->spec().layout()[0] == 0 );
    EXPECT( ds->spec().layout()[1] == 1 );
    EXPECT( ds->spec().layout()[2] == 2 );

    delete ds;
}

#ifdef ATLAS_HAVE_GRIDTOOLS_STORAGE
CASE( "test_spec_layout_rev" ) {
    Array* ds = Array::create<double>( make_shape( 4, 5, 6 ), make_layout( 2, 1, 0 ) );
    EXPECT( ds->spec().rank() == 3 );
    EXPECT( ds->spec().size() == 4 * 5 * 6 );
    EXPECT( ds->spec().shape()[0] == 4 );
    EXPECT( ds->spec().shape()[1] == 5 );
    EXPECT( ds->spec().shape()[2] == 6 );
    EXPECT( ds->spec().shapef()[0] == 4 );
    EXPECT( ds->spec().shapef()[1] == 5 );
    EXPECT( ds->spec().shapef()[2] == 6 );
    if ( NOT_PADDED ) {
        EXPECT( ds->spec().strides()[0] == 1 );
        EXPECT( ds->spec().strides()[1] == 4 );
        EXPECT( ds->spec().strides()[2] == 4 * 5 );
    }
    EXPECT( ds->spec().hasDefaultLayout() == false );
    EXPECT( ds->spec().layout()[0] == 2 );
    EXPECT( ds->spec().layout()[1] == 1 );
    EXPECT( ds->spec().layout()[2] == 0 );

    delete ds;

    EXPECT_THROWS_AS( Array::create<double>( make_shape( 4, 5, 6, 2 ), make_layout( 0, 1, 3, 2 ) ),
                      eckit::BadParameter );
}
#endif

CASE( "test_resize_throw" ) {
    Array* ds = Array::create<double>( 32, 5, 33 );

    EXPECT_NO_THROW( ds->resize( 32, 5, 33 ) );
    EXPECT_THROWS_AS( ds->resize( 32, 4, 33 ), eckit::BadParameter );
    EXPECT_THROWS_AS( ds->resize( 32, 5, 32 ), eckit::BadParameter );
    EXPECT_THROWS_AS( ds->resize( 32, 5, 33, 4 ), eckit::BadParameter );

    delete ds;
}

CASE( "test_copy_ctr" ) {
    Array* ds                          = Array::create<int>( 3, 2 );
    atlas::array::ArrayView<int, 2> hv = make_host_view<int, 2>( *ds );
    hv( 2, 1 )                         = 4;
    hv( 1, 1 )                         = 7;

    atlas::array::ArrayView<int, 2> hv2( hv );

    EXPECT( hv2( 2, 1 ) == 4 );
    EXPECT( hv2( 1, 1 ) == 7 );

    delete ds;
}

#ifdef ATLAS_HAVE_GRIDTOOLS_STORAGE
CASE( "test_copy_gt_ctr" ) {
    Array* ds                          = Array::create<int>( 3, 2 );
    atlas::array::ArrayView<int, 2> hv = make_host_view<int, 2>( *ds );
    hv( 2, 1 )                         = 4;
    hv( 1, 1 )                         = 7;

    atlas::array::ArrayView<int, 2> hv2( hv );

    EXPECT( hv2( 2, 1 ) == 4 );
    EXPECT( hv2( 1, 1 ) == 7 );

    auto dims = hv.data_view().storage_info().dims();
    ATLAS_DEBUG_VAR( dims[0] );
    ATLAS_DEBUG_VAR( dims[1] );
    EXPECT( dims[0] == 3 );
    if ( NOT_PADDED ) EXPECT( dims[1] == 2 );
    delete ds;
}
#endif

CASE( "test_resize" ) {
    {
        Array* ds = Array::create<double>( 0 );
        EXPECT( ds->size() == 0 );
        ds->resize( 0 );
        delete ds;
    }

    {
        Array* ds = Array::create<double>( 1 );
        EXPECT( ds->size() == 1 );
        ds->resize( 2 );
        delete ds;
    }

    {
        Array* ds = Array::create<double>( 0 );
        EXPECT( ds->size() == 0 );
        ds->resize( make_shape( 5 ) );
        delete ds;
    }

    {
        Array* ds = Array::create<double>( 2, 3, 4 );
        {
            atlas::array::ArrayView<double, 3> hv = make_host_view<double, 3>( *ds );
            hv( 1, 1, 1 )                         = 4.5;
            hv( 1, 2, 2 )                         = 7.5;
        }
        ds->resize( 3, 4, 5 );

        atlas::array::ArrayView<double, 3> hv = make_host_view<double, 3>( *ds );

        EXPECT( ds->spec().shape()[0] == 3 );
        EXPECT( ds->spec().shape()[1] == 4 );
        EXPECT( ds->spec().shape()[2] == 5 );

        EXPECT( ds->spec().rank() == 3 );
        EXPECT( ds->spec().size() == 3 * 4 * 5 );

        EXPECT( hv( 1, 1, 1 ) == 4.5 );
        EXPECT( hv( 1, 2, 2 ) == 7.5 );

        delete ds;
    }

    {
        Array* ds = Array::create<int>( 3, 2 );
        {
            atlas::array::ArrayView<int, 2> hv = make_host_view<int, 2>( *ds );
            hv( 2, 1 )                         = 4;
            hv( 1, 1 )                         = 7;
        }
        ds->resize( 6, 2 );
        atlas::array::ArrayView<int, 2> hv = make_host_view<int, 2>( *ds );

        EXPECT( ds->spec().shape()[0] == 6 );
        EXPECT( ds->spec().shape()[1] == 2 );

        EXPECT( ds->spec().rank() == 2 );
        EXPECT( ds->spec().size() == 6 * 2 );

        EXPECT( hv( 2, 1 ) == 4 );
        EXPECT( hv( 1, 1 ) == 7 );

        delete ds;
    }
    // test the resize with wrap
    {
        int vals[6] = {3, 4, 6, 7, 5, 4};

        Array* ds = Array::wrap<int>( vals, array::ArrayShape{3, 2} );
        {
            atlas::array::ArrayView<int, 2> hv = make_host_view<int, 2>( *ds );
            hv( 2, 1 )                         = 4;
            hv( 1, 1 )                         = 7;
        }
        ds->resize( 6, 2 );
        atlas::array::ArrayView<int, 2> hv = make_host_view<int, 2>( *ds );

        EXPECT( ds->spec().shape()[0] == 6 );
        EXPECT( ds->spec().shape()[1] == 2 );

        EXPECT( ds->spec().rank() == 2 );
        EXPECT( ds->spec().size() == 6 * 2 );

        EXPECT( hv( 2, 1 ) == 4 );
        EXPECT( hv( 1, 1 ) == 7 );

        delete ds;
    }
}

CASE( "test_resize_shape" ) {
    Array* ds = Array::create<double>( 7, 5, 8 );
    {
        atlas::array::ArrayView<double, 3> hv = make_host_view<double, 3>( *ds );
        hv( 3, 3, 3 )                         = 4.5;
        hv( 6, 4, 7 )                         = 7.5;
    }
    ds->resize( ArrayShape{32, 5, 33} );

    atlas::array::ArrayView<double, 3> hv = make_host_view<double, 3>( *ds );
    EXPECT( ds->spec().shape()[0] == 32 );
    EXPECT( ds->spec().shape()[1] == 5 );
    EXPECT( ds->spec().shape()[2] == 33 );

    EXPECT( ds->spec().rank() == 3 );
    EXPECT( ds->spec().size() == 32 * 5 * 33 );

    EXPECT( hv( 3, 3, 3 ) == 4.5 );
    EXPECT( hv( 6, 4, 7 ) == 7.5 );

    delete ds;
}

CASE( "test_insert" ) {
    Array* ds = Array::create<double>( 7, 5, 8 );

    EXPECT( ds->hostNeedsUpdate() == false );
    auto hv = make_host_view<double, 3>( *ds );
    hv.assign( -1. );

    EXPECT( hv( 0, 0, 0 ) == -1. );
    hv( 1, 3, 3 ) = 1.5;
    hv( 2, 3, 3 ) = 2.5;
    hv( 3, 3, 3 ) = 3.5;
    hv( 6, 4, 7 ) = 6.5;

    ds->insert( 3, 2 );

    EXPECT( ds->spec().shape()[0] == 9 );
    EXPECT( ds->spec().shape()[1] == 5 );
    EXPECT( ds->spec().shape()[2] == 8 );

    EXPECT( ds->spec().rank() == 3 );
    EXPECT( ds->spec().size() == 9 * 5 * 8 );

    auto hv2 = make_host_view<double, 3>( *ds );

// currently we have no mechanism to invalidate the old views after an insertion
// into the Array
// The original gt data store is deleted and replaced, but the former
// atlas::array::ArrayView keeps a pointer to it
// wihtout noticing it has been deleted
#ifdef ATLAS_HAVE_GRIDTOOLS_STORAGE
    // Following statement seems to contradict previous comment
    EXPECT( hv.valid() == false );
#endif

    EXPECT( hv2.valid() == true );

    EXPECT( hv2( 1, 3, 3 ) == 1.5 );
    EXPECT( hv2( 2, 3, 3 ) == 2.5 );
    EXPECT( hv2( 5, 3, 3 ) == 3.5 );
    EXPECT( hv2( 8, 4, 7 ) == 6.5 );

    delete ds;
}

CASE( "test_insert_throw" ) {
    Array* ds = Array::create<double>( 7, 5, 8 );

    EXPECT_THROWS_AS( ds->insert( 8, 2 ), eckit::BadParameter );
}

CASE( "test_wrap_storage" ) {
    {
        Array* ds = Array::create<double>( 4, 5, 6 );

        atlas::array::ArrayView<double, 3> hv = make_host_view<double, 3>( *ds );

        hv( 2, 3, 3 ) = 2.5;

        Array* ds_ext = Array::wrap<double>( hv.data(), ds->spec() );

        atlas::array::ArrayView<double, 3> hv_ext = make_host_view<double, 3>( *ds_ext );

        EXPECT( hv_ext( 2, 3, 3 ) == 2.5 );

        delete ds;
        delete ds_ext;
    }
    {
        std::vector<double> v( 4 * 5 * 6, 0. );
        v[2 * ( 5 * 6 ) + 3 * 6 + 3] = 2.5;
        ArrayShape shape{4, 5, 6};
        Array* ds_ext = Array::wrap<double>( v.data(), shape );

        atlas::array::ArrayView<double, 3> hv_ext = make_host_view<double, 3>( *ds_ext );

        EXPECT( hv_ext( 2, 3, 3 ) == 2.5 );

        delete ds_ext;
    }
}

CASE( "test_assign" ) {
    Array* ds = Array::create<double>( 2ul, 3ul, 4ul );
    auto hv   = make_host_view<double, 3>( *ds );

    hv.assign( 2.5 );

    EXPECT( hv( 1, 2, 3 ) == 2.5 );

    auto lv = hv.slice( 1, Range::all(), Range::all() );
    lv.assign( 5.0 );

    EXPECT( hv( 0, 2, 3 ) == 2.5 );
    EXPECT( hv( 1, 2, 3 ) == 5.0 );
    EXPECT( lv( 2, 3 ) == 5.0 );

    delete ds;
}

CASE( "test_ArrayT" ) {
    {
        ArrayT<double> ds( 2, 3, 4 );

        EXPECT( ds.size() == 2 * 3 * 4 );
        if ( NOT_PADDED ) {
            EXPECT( ds.stride( 0 ) == 3 * 4 );
            EXPECT( ds.stride( 1 ) == 4 );
            EXPECT( ds.stride( 2 ) == 1 );
        }
        EXPECT( ds.shape( 0 ) == 2 );
        EXPECT( ds.shape( 1 ) == 3 );
        EXPECT( ds.shape( 2 ) == 4 );
    }

    {
        ArrayT<double> ds( make_shape( 2, 3, 4 ) );

        EXPECT( ds.size() == 2 * 3 * 4 );
        if ( NOT_PADDED ) {
            EXPECT( ds.stride( 0 ) == 3 * 4 );
            EXPECT( ds.stride( 1 ) == 4 );
            EXPECT( ds.stride( 2 ) == 1 );
        }
        EXPECT( ds.shape( 0 ) == 2 );
        EXPECT( ds.shape( 1 ) == 3 );
        EXPECT( ds.shape( 2 ) == 4 );
    }

    {
        ArrayT<double> ds( ArraySpec( make_shape( 2, 3, 4 ) ) );

        EXPECT( ds.size() == 2 * 3 * 4 );
        if ( NOT_PADDED ) {
            EXPECT( ds.stride( 0 ) == 3 * 4 );
            EXPECT( ds.stride( 1 ) == 4 );
            EXPECT( ds.stride( 2 ) == 1 );
        }
        EXPECT( ds.shape( 0 ) == 2 );
        EXPECT( ds.shape( 1 ) == 3 );
        EXPECT( ds.shape( 2 ) == 4 );
    }
}

CASE( "test_valid" ) {
    {
        Array* ds = Array::create<double>( 2, 3, 4 );

        atlas::array::ArrayView<double, 3> hv = make_host_view<double, 3>( *ds );
        hv( 1, 1, 1 )                         = 4.5;
        hv( 1, 2, 2 )                         = 7.5;

        EXPECT( hv.valid() == true );
        ds->resize( 3, 4, 5 );

// Only implemented using gridtools storage for now
#ifdef ATLAS_HAVE_GRIDTOOLS_STORAGE
        EXPECT( hv.valid() == false );
#endif

        delete ds;
    }
}

CASE( "test_wrap" ) {
    array::ArrayT<int> arr_t( 3, 2 );
    EXPECT( arr_t.shape( 0 ) == 3 );
    if ( NOT_PADDED ) EXPECT( arr_t.stride( 0 ) == 2 );
    EXPECT( arr_t.shape( 1 ) == 2 );
    EXPECT( arr_t.stride( 1 ) == 1 );
    EXPECT( arr_t.rank() == 2 );

    array::ArrayView<int, 2> arrv_t = array::make_view<int, 2>( arr_t );
    for ( size_t i = 0; i < arrv_t.shape( 0 ); ++i ) {
        for ( size_t j = 0; j < arrv_t.shape( 1 ); ++j ) {
            arrv_t( i, j ) = i * 10 + j - 1;
        }
    }

    eckit::SharedPtr<array::Array> arr( array::Array::wrap<int>(
        arrv_t.data(), array::ArraySpec{array::make_shape( 3 ), array::make_strides( arr_t.stride( 0 ) )} ) );

    EXPECT( arr->shape( 0 ) == 3 );
    EXPECT( arr->stride( 0 ) == arr_t.stride( 0 ) );
    EXPECT( arr->rank() == 1 );

    auto view = make_host_view<int, 1, array::Intent::ReadOnly>( *arr );

    EXPECT( view.shape( 0 ) == 3 );
    EXPECT( view.stride( 0 ) == arr_t.stride( 0 ) );
    EXPECT( view.rank() == 1 );

    EXPECT( view( 0 ) == -1 );
    EXPECT( view( 1 ) == 9 );
    EXPECT( view( 2 ) == 19 );
}

#ifdef ATLAS_HAVE_ACC
CASE( "test_acc_map" ) {
    Array* ds = Array::create<double>( 2, 3, 4 );
#ifdef ATLAS_HAVE_ACC
    EXPECT( ds->accMap() == true );
    EXPECT( ds->accMap() == true );
#else
    EXPECT( ds->accMap() == false );
#endif
}
#endif

//-----------------------------------------------------------------------------

}  // namespace test
}  // namespace atlas

int main( int argc, char** argv ) {
    return atlas::test::run( argc, argv );
}
