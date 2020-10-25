#pragma once


#include <fstream>
#include <iostream>

using namespace std;

namespace cpp2octave{

	class c2o_script_file
	{
	public:

		ofstream script_file;

		c2o_script_file(string script_path_and_name) : 
			script_file(script_path_and_name.c_str())
		{};


		ofstream & fwd(){
			return script_file;
		};



		
	};

};