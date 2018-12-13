#include "eckit/eckit_version.h"

#include "atlas/library/Library.h"
#include "atlas_f/internals/Library.h"

//----------------------------------------------------------------------------------------------------------------------
extern "C" {

void atlas__atlas_init_noargs() {
    atlas::Library::instance().initialise();
}

void atlas__atlas_finalize() {
    atlas::Library::instance().finalise();
}

const char* atlas__eckit_version() {
    return eckit_version();
}

const char* atlas__eckit_git_sha1() {
    return eckit_git_sha1();
}

const char* atlas__eckit_git_sha1_abbrev( int length ) {
    static std::string git_sha1( eckit_git_sha1() );
    if ( git_sha1.empty() )
        git_sha1 = "not available";
    else
        git_sha1 = git_sha1.substr( 0, std::min( length, 40 ) );
    return git_sha1.c_str();
}

const char* atlas__atlas_version() {
    static std::string str = atlas::Library::instance().version();
    return str.c_str();
}

const char* atlas__atlas_git_sha1() {
    static std::string str = atlas::Library::instance().gitsha1();
    return str.c_str();
}

const char* atlas__atlas_git_sha1_abbrev( int length ) {
    static std::string s = atlas::Library::instance().gitsha1( length );
    return s.c_str();
}

}  // extern "C"
