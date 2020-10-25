//#include "StdAfx.h"
#include "cpp2oct.h"

namespace cpp2octave {

	cpp2oct::cpp2oct( string _env_name ) : cpp2oct_core( _env_name )
	{
	}


	cpp2oct::~cpp2oct(void)
	{
		datafile_set.remove_if( delete_datafile_set_ptrs );
	}

};
