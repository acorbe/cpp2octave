//#include "StdAfx.h"
#include "cpp2oct_core.h"

namespace cpp2octave {



	void make_dir( string dirname ){
#if defined(_WIN32) || defined(_WIN64)
	_mkdir(dirname.c_str());
#else 
	mkdir(dirname.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif		
	};

	const string O_END_INSTR	= ";";
	const string O_END_CL		= ";\n";
	const string O_EQUALS		= " = ";
	const string O_COMMT		= "% ";
	

#define C_STR(x) x.c_str()
#define STR_PARENTESIZE(x)	append("(").append(x).append(")") //the initial '.' (dot) is missing just to enhance symmetry
#define STR_APEXSIZE(x)		append("'").append(x).append("'") //the initial '.' (dot) is missing just to enhance symmetry
#define STR_COMMA			append(",")
#define STR_SPACE			append(" ")

#define OBJECT_COUNTER_INIT_TO_ZERO 0
#define FWD current_script_file->script_file

	const string cpp2oct_core::MAIN_SCRIPT_INIT_COMMENT = "% Created through cpp2octave \n% A C++ library by Alessandro Corbetta.\n\n";


	cpp2oct_core::cpp2oct_core(string _env_name) :
		env_name( _env_name ) ,
			fname_handler( env_name , default_path , default_support_folder),
			main_script_file( fname_handler.get_main_script_name() ),
			current_script_file( &main_script_file ),
			object_counter( OBJECT_COUNTER_INIT_TO_ZERO ),
			data_file_counter( OBJECT_COUNTER_INIT_TO_ZERO ),
			script_datafile( this , "" , "" , new_data_file() ),
			current_datafile( &script_datafile )
	{
		FWD << C_STR(MAIN_SCRIPT_INIT_COMMENT);
		make_dir( fname_handler.get_support_folder_name() );
	}


	void cpp2oct_core::put_oct_code_line( string code_line ){
		FWD << C_STR( code_line.append( O_END_CL ) );
	}

	void cpp2oct_core::put_oct_comment(string oct_comment){
		string t( O_COMMT );
		t.append( oct_comment ).append( "\n" );
		FWD << C_STR( t );		
	}

	int cpp2oct_core::new_obj_id()		{	return object_counter++; }
	int cpp2oct_core::new_data_file()	{	return data_file_counter++; }//data_file_counter++; return data_file_counter-1;}//

	c2o_obj	cpp2oct_core::oct_new_obj_base( string obj_tag ){ return c2o_obj( this , new_obj_id() , obj_tag ); }
	
#define GENERATE_F_CALL_LINE_PORTION line \
				.append( f_name ) \
				.STR_PARENTESIZE( args.get_oct_string() ) \
				;	

	void cpp2oct_core::oct_f_call ( string f_name , c2o_arg args ){
		string line;
		GENERATE_F_CALL_LINE_PORTION;
		put_oct_code_line(line);	
	};

	void cpp2oct_core::oct_c_call ( string f_name , string status ){
		string line;
		line.append( f_name ).STR_SPACE.append( status );
		put_oct_code_line(line);
	};

	c2o_obj& cpp2oct_core::oct_f_call ( c2o_obj& dest , string f_name , c2o_arg args ){
		string line;
		GENERATE_F_CALL_LINE_PORTION;		
		dest.set(line);
		return dest;
	};	

	/*c2o_datafile_ASCII cpp2oct_core::new_datafile_ASCII( string fname ){

		return c2o_datafile_ASCII(this , fname , fname_handler.get_support_folder_name() , new_data_file() );
	}*/

	cpp2oct_core::~cpp2oct_core(void)
	{
	}

	string cpp2oct_core::default_path = "./";
	string cpp2oct_core::default_support_folder = "support";

/////////////////////////////////////////////////////////

	const string c2o_obj::OBJ_PREFIX = "obj";
	const string c2o_obj::OBJ_NUMBER_TAG_SEPARATOR = "_";

#define OCT_OBJ_INIT_TO_UNRELEASED false
#define TOT_CHAR_FOR_NUM_CONVERSION 5
#define BASE_IS_DECIMAL 10
	c2o_obj::c2o_obj( cpp2oct_core * _owner , int obj_num_id , string obj_tag ) : 
		owner( _owner ) ,
			is_released( OCT_OBJ_INIT_TO_UNRELEASED )
	{	
		char buffer_num_comversion[ TOT_CHAR_FOR_NUM_CONVERSION ];
		//itoa( obj_num_id , buffer_num_comversion , BASE_IS_DECIMAL );
		sprintf(buffer_num_comversion,"%d",obj_num_id);
		oct_name
			.append(OBJ_PREFIX)
			.append(buffer_num_comversion)
			.append(OBJ_NUMBER_TAG_SEPARATOR)
			.append(obj_tag);
	}

	c2o_obj & c2o_obj::set( string oct_script ) {
		string instr;
		instr
			.append( oct_name )
			.append( O_EQUALS )
			.append( oct_script );
		owner->put_oct_code_line( instr );
		is_released = true;
		return *this;
	}
	
/////////////////////////////////////////////////////////

#define COMMA_SPACE_X_SPACE(x) \
	STR_COMMA	\
	.STR_SPACE	\
	.x \
	.STR_SPACE;

	c2o_arg & c2o_arg::cat( c2o_arg arg ){
		arg_list
			//.STR_SPACE
			.append(arg.get_oct_string_precommized()); //the string attached includes the final space already
		return *this;
	}
	
	c2o_arg & c2o_arg::add( string arg ){
		arg_list
			.COMMA_SPACE_X_SPACE(STR_APEXSIZE(arg));
		return *this;
	}

	c2o_arg & c2o_arg::add( c2o_obj arg ){
		if( !arg.released() ) { cerr << "c2o_exc_unreleased_obj_used\n"; throw c2o_exc_unreleased_obj_used(); }
		arg_list
			.COMMA_SPACE_X_SPACE(append(arg.get_oct_name()));
		return *this;
	}

	c2o_arg & c2o_arg::add( int arg ){
		stringstream t; t << arg;
		arg_list
			.COMMA_SPACE_X_SPACE(append(t.str()));
		return *this;
	}

	c2o_arg & c2o_arg::add( double arg ){
		stringstream t; t << arg;
		arg_list
			.COMMA_SPACE_X_SPACE(append(t.str()));
		return *this;
	}

	c2o_arg & c2o_arg::add( float arg ){
		stringstream t; t << arg;
		arg_list
			.COMMA_SPACE_X_SPACE(append(t.str()));
		return *this;
	}

	c2o_arg & c2o_arg::copy( string arg ){
		arg_list
			.COMMA_SPACE_X_SPACE(append(arg));
		return *this;
	}

	c2o_datafile_handler::c2o_datafile_handler( cpp2oct_core * _owner , string _f_name , string _path , int _data_file_idx )
		: 
		owner( _owner ) , 
		f_name( _f_name ) , 
		path( _path ) , 
		data_file_idx( _data_file_idx ) 
	{ };

	c2o_datafile_handler::c2o_datafile_handler( cpp2oct_core * _owner , string _f_name  )
		: 
		owner( _owner ) , 
		f_name( _f_name ) , 
		path( _owner->get_support_folder_name() ) , 
		data_file_idx( _owner->new_data_file() ) 
	{ };

	c2o_datafile_script::c2o_datafile_script( cpp2oct_core * _owner , string _f_name , string _path , int _data_file_idx ) :
		  c2o_datafile_handler( _owner, _f_name, _path, _data_file_idx ) { };

	c2o_obj& c2o_datafile_script::serial_write( c2o_obj& dest , const int* vect , size_t N ){
		stringstream o;
		o << "[ ";
		for( int i = 0 ; i < N ; i++ ){
			o << vect[i] << " " ;
		}
		o << "]";
		dest.set(o.str());
		return dest; 
	};

	c2o_obj& c2o_datafile_script::serial_write( c2o_obj& dest , const double* vect , size_t N ){ 
		stringstream o;
		o << "[ ";
		for( int i = 0 ; i < N ; i++ ){
			o << vect[i] << " " ;
		}
		o << "]";
		dest.set(o.str());
		return dest; 
	};

	



	void c2o_datafile_ASCII::out_file_init( ){
		stringstream s;
		s << path << DIR_SEP_CH << f_name;
		string fname_str = s.str();
		out_file.open( fname_str.c_str() );

		owner->f_fopen_r( out_file_link , fname_str );

	};

	c2o_datafile_ASCII::c2o_datafile_ASCII( cpp2oct_core * _owner , string _f_name , string _path , int _data_file_idx ) :
	  c2o_datafile_handler( _owner , _f_name , _path , _data_file_idx )	  ,
		  out_file_link(_owner->oct_new_obj_base(string("ASCII_f_").append(_f_name)))
	{	
		out_file_init( );
	};

	c2o_datafile_ASCII::c2o_datafile_ASCII( cpp2oct_core * _owner , string _f_name ) :
	  c2o_datafile_handler( _owner , _f_name , _owner->get_support_folder_name() , _owner->new_data_file() ),
		  out_file_link(_owner->oct_new_obj_base(string("ASCII_f_").append(_f_name)))
	{	
		out_file_init( );
	};


	c2o_obj& c2o_datafile_ASCII::serial_write( c2o_obj& dest , const int* vect , size_t N ){
		stringstream o;
		for( int i = 0 ; i < N ; i++ ){
			o << vect[i] << " " ;
		}
		o << "\n";
		out_file << o.str();

		//owner->oct_f_call( dest , "fscanf", c2o_arg().add( out_file_link ).add( (int) N ) );

		owner->oct_f_call( dest , "fscanf", out_file_link , "%d " , static_cast<int> ( N )  );
				
		return dest;
	};

	c2o_obj& c2o_datafile_ASCII::serial_write( c2o_obj& dest , const double* vect , size_t N ){
		stringstream o;
		for( int i = 0 ; i < N ; i++ ){
			o << vect[i] << " " ;
		}
		o << "\n";
		out_file << o.str();

		//owner->oct_f_call( dest , "fscanf", c2o_arg().add( out_file_link ).add( (int) N ) );

		owner->oct_f_call( dest , "fscanf", out_file_link , "%f " , static_cast<int> ( N ) );
				
		return dest;		
	};

	void c2o_datafile_ASCII::datafile_close( void ){ owner->f_fclose( out_file_link ); };







		

};


//#undef OCT_OBJ_INIT_TO_UNRELEASED
//#undef TOT_CHAR_FOR_NUM_CONVERSION
//#undef BASE_IS_DECIMAL
//#undef FWD

