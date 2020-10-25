//#include "StdAfx.h"
#include "c2o_datafile_gen.h"


namespace cpp2octave {

	namespace c2o_datafile_gen_policies {

		const string c2o_datafile_WM_ASCII::WM_FILE_PREFIX = "ASCII";
		const string c2o_datafile_WM_ASCII::OCT_READ_MODE = "r";

		const string 
			c2o_datafile_WM_ASCII::
			reading_tags< double >::READING_TAG = "%f ";
		
		const string 
			c2o_datafile_WM_ASCII::
			reading_tags< int >::READING_TAG = "%d ";

		const ios_base::openmode
			c2o_datafile_WM_ASCII::
			WM_IOS_MODE = ios::out;


///////////////////////////////////////////////////////

		const string c2o_datafile_WM_BIN::WM_FILE_PREFIX = "BIN";
		const string c2o_datafile_WM_BIN::OCT_READ_MODE = "rb";

		const string 
			c2o_datafile_WM_BIN::
			reading_tags< double >::READING_TAG = "double";
		
		const string 
			c2o_datafile_WM_BIN::
			reading_tags< int >::READING_TAG = "int";

		const ios_base::openmode
			c2o_datafile_WM_BIN::
			WM_IOS_MODE = ios::out | ios::binary;

	};
};

