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
#include <stdexcept>
#include <sstream>
#include "eckit/exception/Exceptions.h"
#include "eckit/parser/JSON.h"
#include "atlas/Metadata.h"
#include "atlas/Field.h"
#include "atlas/ErrorHandling.h"

using std::string;

namespace atlas {

namespace {
void throw_exception(const std::string& name)
{
  std::stringstream msg;
  msg << "Could not find metadata \"" << name << "\"";
  throw eckit::OutOfRange(msg.str(),Here());
}
}

template<> Metadata& Metadata::set(const std::string& name, const bool& value)
{
  eckit::Properties::set(name,value);
  return *this;
}

template<> Metadata& Metadata::set(const std::string& name, const int& value)
{
  eckit::Properties::set(name,value);
  return *this;
}

template<> Metadata& Metadata::set(const std::string& name, const long& value)
{
  eckit::Properties::set(name,value);
  return *this;
}

template<> Metadata& Metadata::set(const std::string& name, const double& value)
{
  eckit::Properties::set(name,value);
  return *this;
}

template<> Metadata& Metadata::set(const std::string& name, const size_t& value)
{
  eckit::Properties::set(name,value);
  return *this;
}

template<> Metadata& Metadata::set(const std::string& name, const std::string& value)
{
  eckit::Properties::set(name,value);
  return *this;
}

Metadata& Metadata::set(const std::string& name, const char* value)
{
  return set(name,std::string(value));
}


template<> Metadata& Metadata::set(const std::string& name, const std::vector<int>& value)
{
  eckit::Properties::set(name,eckit::makeVectorValue(value));
  return *this;
}

template<> Metadata& Metadata::set(const std::string& name, const std::vector<long>& value)
{
  eckit::Properties::set(name,eckit::makeVectorValue(value));
  return *this;
}

template<> Metadata& Metadata::set(const std::string& name, const std::vector<float>& value)
{
  eckit::Properties::set(name,eckit::makeVectorValue(value));
  return *this;
}

template<> Metadata& Metadata::set(const std::string& name, const std::vector<double>& value)
{
  eckit::Properties::set(name,eckit::makeVectorValue(value));
  return *this;
}

template<> bool Metadata::get(const std::string& name) const
{
  if( !has(name) ) throw_exception(name);
  return eckit::Properties::get(name);
}

template<> int Metadata::get(const std::string& name) const
{
  if( !has(name) ) throw_exception(name);
  return eckit::Properties::get(name);
}

template<> long Metadata::get(const std::string& name) const
{
  if( !has(name) ) throw_exception(name);
  return eckit::Properties::get(name);
}

template<> float Metadata::get(const std::string& name) const
{
  if( !has(name) ) throw_exception(name);
  return double(eckit::Properties::get(name));
}

template<> double Metadata::get(const std::string& name) const
{
  if( !has(name) ) throw_exception(name);
  return eckit::Properties::get(name);
}

template<> size_t Metadata::get(const std::string& name) const
{
  if( !has(name) ) throw_exception(name);
  return eckit::Properties::get(name);
}

template<> std::string Metadata::get(const std::string& name) const
{
  if( !has(name) ) throw_exception(name);
  return eckit::Properties::get(name);
}

template<> std::vector<int> Metadata::get(const std::string& name) const
{
  if( !has(name) ) throw_exception(name);
  std::vector<eckit::Value> v = eckit::Properties::get(name);
  std::vector<int> value;
  value.assign(v.begin(),v.end());
  return value;
}

template<> std::vector<long> Metadata::get(const std::string& name) const
{
  if( !has(name) ) throw_exception(name);
  std::vector<eckit::Value> v = eckit::Properties::get(name);
  std::vector<long> value;
  value.assign(v.begin(),v.end());
  return value;
}

template<> std::vector<float> Metadata::get(const std::string& name) const
{
  if( !has(name) ) throw_exception(name);
  std::vector<eckit::Value> v = eckit::Properties::get(name);
  std::vector<float> value(v.size());
  for( size_t i=0; i<v.size(); ++i )
    value[i] = double( v[i] );
  return value;
}

template<> std::vector<double> Metadata::get(const std::string& name) const
{
  if( !has(name) ) throw_exception(name);
  std::vector<eckit::Value> v = eckit::Properties::get(name);
  std::vector<double> value;
  value.assign(v.begin(),v.end());
  return value;
}

template<> bool Metadata::get(const std::string& name, bool& value) const
{
  if( !has(name) ) return false;
  value = eckit::Properties::get(name);
  return true;
}

template<> bool Metadata::get(const std::string& name, long& value) const
{
  if( !has(name) ) return false;
  value = eckit::Properties::get(name);
  return true;
}

template<> bool Metadata::get(const std::string& name, double& value) const
{
  if( !has(name) ) return false;
  value = eckit::Properties::get(name);
  return true;
}

template<> bool Metadata::get(const std::string& name, size_t& value) const
{
  if( !has(name) ) return false;
  value = eckit::Properties::get(name);
  return true;
}

template<> bool Metadata::get(const std::string& name, std::string& value) const
{
  if( !has(name) ) return false;
  value = std::string( eckit::Properties::get(name) );
  return true;
}

// ------------------------------------------------------------------
// C wrapper interfaces to C++ routines

Metadata* atlas__Metadata__new () {
  return new Metadata();
}

void atlas__Metadata__delete (Metadata* This) {
  delete This;
}

void atlas__Metadata__set_int (Metadata* This, const char* name, int value)
{
  ATLAS_ERROR_HANDLING( This->set( std::string(name), long(value) ) );
}
void atlas__Metadata__set_long (Metadata* This, const char* name, long value)
{
  ATLAS_ERROR_HANDLING( This->set( std::string(name), value ) );
}
void atlas__Metadata__set_float (Metadata* This, const char* name, float value)
{
  ATLAS_ERROR_HANDLING( This->set( std::string(name), double(value) ) );
}
void atlas__Metadata__set_double (Metadata* This, const char* name, double value)
{
  ATLAS_ERROR_HANDLING( This->set( std::string(name), value ) );
}
void atlas__Metadata__set_string (Metadata* This, const char* name, const char* value)
{
  ATLAS_ERROR_HANDLING( This->set( std::string(name), std::string(value) ) );
}
void atlas__Metadata__set_array_int (Metadata* This, const char* name, int value[], int size)
{
  ATLAS_ERROR_HANDLING(
    std::vector<int> v;
    v.assign(value,value+size);
    This->set( std::string(name), v );
  );
}
void atlas__Metadata__set_array_long (Metadata* This, const char* name, long value[], int size)
{
  ATLAS_ERROR_HANDLING(
    std::vector<long> v;
    v.assign(value,value+size);
    This->set( std::string(name), v );
  );
}
void atlas__Metadata__set_array_float (Metadata* This, const char* name, float value[], int size)
{
  ATLAS_ERROR_HANDLING(
    std::vector<float> v;
    v.assign(value,value+size);
    This->set( std::string(name), v );
  );
}
void atlas__Metadata__set_array_double (Metadata* This, const char* name, double value[], int size)
{
  ATLAS_ERROR_HANDLING(
    std::vector<double> v;
    v.assign(value,value+size);
    This->set( std::string(name), v );
  );
}
int atlas__Metadata__get_int (Metadata* This, const char* name)
{
  ATLAS_ERROR_HANDLING( return This->get<long>( std::string(name) ) );
}
long atlas__Metadata__get_long (Metadata* This, const char* name)
{
  ATLAS_ERROR_HANDLING( return This->get<long>( std::string(name) ) );
}
float atlas__Metadata__get_float (Metadata* This, const char* name)
{
  ATLAS_ERROR_HANDLING( return This->get<double>( std::string(name) ) );
}
double atlas__Metadata__get_double (Metadata* This, const char* name)
{
  ATLAS_ERROR_HANDLING( return This->get<double>( std::string(name) ) );
}
void atlas__Metadata__get_string( Metadata* This, const char* name, char* output_str, int max_len )
{
  ATLAS_ERROR_HANDLING(
    std::string s = This->get<std::string>( std::string(name) );
    if( s.size() > max_len )
    {
      std::stringstream msg;
      msg << "Cannot copy string `"<<s<<"` of metadata `"<<name<<"`"
             "in buffer of length " << max_len;
      throw eckit::OutOfRange(msg.str(),Here());
    }
    strcpy( output_str, s.c_str() );
  );
}
void atlas__Metadata__get_array_int (Metadata* This, const char* name, int* &value, int& size, int& allocated)
{
  ATLAS_ERROR_HANDLING(
    std::vector<int> v = This->get< std::vector<int> >( std::string(name ) );
    size = v.size();
    value = new int[size];
    for( size_t j=0; j<v.size(); ++j ) value[j] = v[j];
    allocated = true;
  );
}
void atlas__Metadata__get_array_long (Metadata* This, const char* name, long* &value, int& size, int& allocated)
{
  ATLAS_ERROR_HANDLING(
    std::vector<long> v = This->get< std::vector<long> >( std::string(name ) );
    size = v.size();
    value = new long[size];
    for( size_t j=0; j<v.size(); ++j ) value[j] = v[j];
    allocated = true;
  );
}
void atlas__Metadata__get_array_float (Metadata* This, const char* name, float* &value, int& size, int& allocated)
{
  ATLAS_ERROR_HANDLING(
    std::vector<float> v = This->get< std::vector<float> >( std::string(name ) );
    size = v.size();
    value = new float[size];
    for( size_t j=0; j<v.size(); ++j ) value[j] = v[j];
    allocated = true;
  );
}
void atlas__Metadata__get_array_double (Metadata* This, const char* name, double* &value, int& size, int& allocated)
{
  ATLAS_ERROR_HANDLING(
    std::vector<double> v = This->get< std::vector<double> >( std::string(name ) );
    size = v.size();
    value = new double[size];
    for( size_t j=0; j<v.size(); ++j ) value[j] = v[j];
    allocated = true;
  );
}
int atlas__Metadata__has (Metadata* This, const char* name)
{
  ATLAS_ERROR_HANDLING( return This->has( std::string(name) ));
}

void atlas__Metadata__print (Metadata* This, std::ostream* channel)
{
  ATLAS_ERROR_HANDLING(
    ASSERT( This != NULL );
    ASSERT( channel != NULL );
    *channel << *This;
  );
}

void atlas__Metadata__json(Metadata* This, char* &json, int &size, int &allocated)
{
  std::stringstream s;
  eckit::JSON j(s);
  j.precision(16);
  j << *This;
  std::string json_str = s.str();
  size = json_str.size();
  json = new char[size+1]; allocated = true;
  strcpy(json,json_str.c_str());
  allocated = true;
}

// ------------------------------------------------------------------

} // namespace atlas
