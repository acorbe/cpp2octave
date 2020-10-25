#pragma once

#include <iostream>

using namespace std;

namespace cpp2octave{

class c2o_fname_handler
{

private:
	string	main_script_name;
	string	support_folder_name;
public:

	static const string P__SCRIPT_DEF_EXTENSION;


	c2o_fname_handler( string env_name , string path , string support_folder ){
		main_script_name
			.append( path )
			.append( env_name )
			.append( P__SCRIPT_DEF_EXTENSION );

		support_folder_name
			.append( path )
			//.append( "/" ) //TODO: discuss better path form
			.append( support_folder )
			.append( "_" )
			.append( env_name );

	}
	
	string get_main_script_name(){ return main_script_name;	}
	string get_support_folder_name(){ return support_folder_name; }


};

};
