/*
 * (C) Copyright 1996-2014 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include <cstring>

#include "atlas/atlas_config.h"

#include <eckit/exception/Exceptions.h>
#include <eckit/log/Log.h>
#include <eckit/filesystem/PathName.h>
#include <eckit/io/DataHandle.h>
#include <eckit/utils/Translator.h>

#ifdef ECKIT_HAVE_GRIB
  #include <eckit/grib/GribFieldSet.h>
  #include <eckit/grib/GribField.h>
  #include <eckit/grib/GribHandle.h>
  #include <eckit/grib/GribParams.h>
#endif

#include "atlas/Parameters.h"
#include "atlas/util/ArrayView.h"
#include "atlas/FunctionSpace.h"
#include "atlas/FieldSet.h"

//------------------------------------------------------------------------------------------------------

using namespace eckit;
using namespace eckit::grib;

namespace atlas {


//------------------------------------------------------------------------------------------------------

Field::Ptr FieldSet::create_field( GribHandle& gh )
{
#ifdef ECKIT_HAVE_GRIB
	if(!grid_)
	{
		GribParams* gp = GribParams::create(gh);
		ASSERT( gp );
		grid_.reset( Grid::create( *gp ) );
	}
	else // check grid is the same
	{
		if( gh.geographyHash() != grid_->hash() )
			throw eckit::UserError("GRIB fields don't match grid within FieldSet", Here() );
	}

	Mesh& mesh = grid_->mesh();
    FunctionSpace&  nodes  = mesh.function_space( "nodes" );

    // get name for this field
	std::string sname = gh.shortName() + "_" + Translator<size_t,std::string>()( fields_.size() );

    // get values

    size_t nvalues = gh.getDataValuesSize();

    // create the field

    if( nodes.shape(0) != nvalues )
        throw SeriousBug( "Size of field in GRIB does not match Grid", Here() );

	Field& f = nodes.create_field<double>(sname,1);

	gh.getDataValues( f.data<double>(), nvalues );

	f.grib( gh.clone() );

	return f.self();

//        {
//            std::ofstream of;
//            of.open( sname.c_str() );
//            of << *hf << std::endl;
//            of.close();
//        }
#else
  throw eckit::Exception("eckit was built without GRIB support\n  --> Cannot create field from GribHandle", Here());
  return Field::Ptr();
#endif
}

bool FieldSet::checkConsistency() const
{
	if( fields_.empty() ) return true;

	bool result = true;

	std::string uid = grid_->uid();

	for( size_t i = 0; i < fields_.size(); ++i )
	{
		if( fields_[i]->grid().uid() != uid )
		{
			Log::error() << "fields_["<< i <<"] (" << fields_[i]->name() << ") doesn't match the same grid as FieldSet @ " << Here() << std::endl;
			result = false;
		}
	}

	return result;
}

//-----------------------------------------------------------------------------

FieldSet::FieldSet( const eckit::PathName& fname ) :
	fields_(),
	grid_()
{
#ifdef ECKIT_HAVE_GRIB
    GribFieldSet gribfs(fname);

    if( gribfs.size() == 0 ) return;

    fields_.reserve( gribfs.size() );

    size_t check_nvalues = 0;

    for( size_t fidx = 0; fidx < gribfs.size(); ++fidx )
    {
        GribField* gf = const_cast<GribField*>( gribfs.get(fidx) );
        ASSERT( gf );

        GribHandle* gh = gf->getHandle();
        ASSERT( gh );

		fields_.push_back( create_field(*gh) ); // grid_ built on first call

        gf->release(); // free this GribField
    }

	ASSERT( grid_ );
	ASSERT( checkConsistency() );
#else
  throw eckit::Exception("eckit was built without GRIB support\n  --> Cannot construct FieldSet from grib", Here());
#endif
}

FieldSet::FieldSet( const Buffer& buf ) :
	fields_(),
	grid_()
{
#ifdef ECKIT_HAVE_GRIB
    GribHandle gh( buf );

	fields_.push_back( create_field(gh) ); // grid_ built inside

	ASSERT( grid_ );
	ASSERT( checkConsistency() );
#else
  throw eckit::Exception("eckit was built without GRIB support\n  --> Cannot construct FieldSet from grib buffer", Here());
#endif
}

FieldSet::FieldSet(const Grid::Ptr grid, std::vector<std::string> nfields ) :
	fields_(),
	grid_(grid)
{
	ASSERT( grid_ );

	Mesh& mesh = grid_->mesh();

	FunctionSpace& nodes = mesh.function_space( "nodes" );
    fields_.reserve(nfields.size());
    for( size_t i = 0; i < nfields.size(); ++i )
    {
		Field& f = nodes.create_field<double>(nfields[i],1);

		ASSERT( grid->uid() == f.grid().uid() );

		fields_.push_back( Field::Ptr( &f ) );
    }

	ASSERT( checkConsistency() );
}

FieldSet::FieldSet(const Field::Vector& fields) :
	fields_(fields),
	grid_()
{
	if( !fields_.empty() )
		grid_.reset( fields_[0]->grid().self() );

	ASSERT( grid_ );
	ASSERT( checkConsistency() );
}

std::vector<std::string> FieldSet::field_names() const
{
    std::vector<std::string> ret;
    ret.reserve(fields_.size());

    for( size_t i = 0; i < fields_.size(); ++i )
		ret.push_back( fields_[i]->name() );

    return ret;
}

//-----------------------------------------------------------------------------


} // namespace eckit
