#pragma once

#include <iostream>
#include "cpp2oct_script_file.h"
#include "c2o_fname_handler.h"
#include <sstream>

#include <sys/stat.h>
#include <sys/types.h>
//#include <stdio.h>
#if defined(_WIN32) || defined(_WIN64)
#include <direct.h> //For makedir
#endif

using namespace std;


namespace cpp2octave{

	class c2o_arg;
	class cpp2oct_core;
	class c2o_datafile_handler;
	class c2o_datafile_script;

	void make_dir( string dirname );

	#define DIR_SEP_CH "/"

	/**
	* "Pointer" to octave object
	*/
	class c2o_obj
	{
	public:
		const static string OBJ_PREFIX;
		const static string OBJ_NUMBER_TAG_SEPARATOR;
	private:
		friend class cpp2oct_core;	

		/**
		* Name of the object as Octave sees it.
		*/
		string	oct_name;

		/**
		* It tells whether the vector has been released already in the script
		*/
		bool	is_released;
	protected:
		c2o_obj( cpp2oct_core * _owner , int obj_num_id , string obj_tag );

		/**
		* Pointer to the owner cpp2oct_core class instance
		*/
		cpp2oct_core * owner;

	public:
		c2o_obj & set(string oct_script) ;
		string get_oct_name () const { return oct_name; }
		bool released( ) const { return is_released; };
	};

	

	class c2o_arg
	{
		string arg_list;
	public:
		c2o_arg(){}

		template< class T >
		c2o_arg( T arg ){ add( arg ); };

		template< class T1 , class T2 >
		c2o_arg( T1 arg_1 , T2 arg_2 ){ add( arg_1 , arg_2 ); };

		template< class T1 , class T2 , class T3 >
		c2o_arg( T1 arg_1 , T2 arg_2 , T3 arg_3 ){ add( arg_1 , arg_2 , arg_3 ); };


		c2o_arg & cat	( c2o_arg arg );	/* It is not add to prevent overload ambiguities. */
		c2o_arg & add	( string arg );		/* octave string - SO FAR IT ALLOWS FOR EMPTY STRINGS, i.e. "" => '' */
		c2o_arg & add	( c2o_obj arg );
		c2o_arg & add	( int arg );	
		c2o_arg & add	( double arg ); 
		c2o_arg & add	( float arg );	

		template< class T1 , class T2 >
		c2o_arg & add	( T1 arg_1 , T2 arg_2 ) { return add( arg_1 ).add( arg_2 ); };

		template< class T1 , class T2 , class T3 >
		c2o_arg & add	( T1 arg_1 , T2 arg_2 , T3 arg_3 ) { return add( arg_1 , arg_2 ).add( arg_3 ); };
		
		c2o_arg & copy	( string arg );	/* copied text, non octave string */
		
		template< class T1 , class T2 >
		c2o_arg & copy	( T1 arg_1 , T2 arg_2 ) { return copy( arg_1 ).copy( arg_2 ); };


		string get_oct_string_precommized() const { return arg_list; };
#define SKIP_FIRST_COMMA 1
		string get_oct_string() const { return arg_list.empty() ? "" : arg_list.substr(SKIP_FIRST_COMMA); };		

	};

	const c2o_arg EMPTY_ARG;


	class c2o_datafile_handler{
	public:
		virtual ~c2o_datafile_handler( void ) {};

		virtual c2o_obj& serial_write( c2o_obj& dest , const int* vect , size_t N ) = 0 ;

		virtual c2o_obj& serial_write( c2o_obj& dest , const double* vect , size_t N ) = 0 ;

		virtual void datafile_close( void ) = 0 ;

		/*virtual void focus_set( void ) = 0;

		virtual void focus_lost( void ) = 0;*/

	protected:
		cpp2oct_core * owner;
		string f_name;
		string path;
		int data_file_idx;
		c2o_datafile_handler( cpp2oct_core * _owner , string _f_name , string _path , int _data_file_idx );
		c2o_datafile_handler( cpp2oct_core * _owner , string _f_name );
	};

	typedef c2o_datafile_handler* c2o_datafile_ptr;

	class c2o_datafile_script :
		public c2o_datafile_handler {
	
	public:
		c2o_datafile_script( cpp2oct_core * _owner , string _f_name , string _path , int _data_file_idx );

		c2o_obj& serial_write( c2o_obj& dest , const int* vect , size_t N );

		c2o_obj& serial_write( c2o_obj& dest , const double* vect , size_t N );

		void datafile_close( void ){ };

	};

	
	/**
	* Deprecated
	*/
	class c2o_datafile_ASCII :
		public c2o_datafile_handler {
	private:
		ofstream out_file;
		c2o_obj out_file_link;

		void out_file_init();

	public:
		c2o_datafile_ASCII( cpp2oct_core * _owner , string _f_name , string _path , int _data_file_idx );

		c2o_datafile_ASCII( cpp2oct_core * _owner , string _f_name );

		c2o_obj& serial_write( c2o_obj& dest , const int* vect , size_t N );

		c2o_obj& serial_write( c2o_obj& dest , const double* vect , size_t N );

		void datafile_close( void );

		
	};

	/*class c2o_datafile_binary :
		public c2o_datafile_handler {
	};*/

	
	/**
	* Core class of the cpp2octave library.
	* It contains all the basic functionalities of the library.
	*/
	class cpp2oct_core
	{
	private:
		static const string MAIN_SCRIPT_INIT_COMMENT;	//Character distringuishing comments, for octave "%"				
	protected:
		static string default_path;				//Default path for initialized cpp2oct_core objects. Once the object has been instantiated it cannot be changed
		static string default_support_folder;	//Path of folder containing datafiles. It cannot be changed after object instantiation.
		
	private:	
		/**
		* name characteristic of the environment, given by the user;
		*/
		string env_name;

		/**
		* object that handles the names;
		*/
		c2o_fname_handler fname_handler;		

		/**
		* Main script file, possibly wrapper of sub scripts;
		*/
		c2o_script_file		main_script_file;

		/**
		* Script file we are currently using;
		*/
		c2o_script_file*	current_script_file;

		/**
		* Counter of the octave object instatiated so far.
		* It can be incremented only through the function int new_obj_id();
		*/
		int object_counter;
		int new_obj_id();


		int data_file_counter;
		int new_data_file();
		
		c2o_datafile_script		script_datafile;
		c2o_datafile_ptr		current_datafile;


	public:
		friend class c2o_datafile_ASCII;  //Old to be deprecated
		friend class c2o_datafile_handler;
		cpp2oct_core( string _env_name );

		void	put_oct_code_line( string code_line );
		void	put_oct_comment( string oct_comment );

		
		c2o_obj& oct_f_call	( c2o_obj& dest , string f_name , c2o_arg args = EMPTY_ARG );
		void	 oct_f_call ( string f_name , c2o_arg args = EMPTY_ARG );

		template < class T1 , class T2 >
		c2o_obj& oct_f_call	( c2o_obj& dest , string f_name , T1 arg_1 , T2 arg_2 ) { return oct_f_call( dest , f_name , c2o_arg( arg_1 , arg_2 ) ); };

		template < class T1 , class T2 >
		void oct_f_call	( string f_name , T1 arg_1 , T2 arg_2 ) { return oct_f_call( f_name , c2o_arg( arg_1 , arg_2 ) ); };


		template < class T1 , class T2 , class T3 >
		c2o_obj& oct_f_call	( c2o_obj& dest , string f_name , T1 arg_1 , T2 arg_2 , T3 arg_3 ) { return oct_f_call( dest , f_name , c2o_arg( arg_1 , arg_2 , arg_3 ) ); };

		template < class T1 , class T2 , class T3 >
		void oct_f_call	( string f_name , T1 arg_1 , T2 arg_2 , T3 arg_3 ) { return oct_f_call( f_name , c2o_arg( arg_1 , arg_2 , arg_3 ) ); };

		void	oct_c_call	( string f_name , string status = "" );	/* to handle commands like grid on; */
		//THE FOLLOWING THROWS RUNTIME EXCEPTION, WHY?
		//void	oct_c_call	( string f_name , bool status ){ oct_c_call( f_name , status ? "on" : "off" ); };

		string get_support_folder_name() { return  fname_handler.get_support_folder_name(); };
		c2o_datafile_ptr get_current_datafile(){ return current_datafile; };
		c2o_datafile_ptr get_inline_datafile(){ return &script_datafile; };

		c2o_datafile_ptr switch_datafile( c2o_datafile_ptr datafile_ ) { 
			current_datafile = datafile_; 
			return current_datafile; 
		};

		//c2o_datafile_ASCII new_datafile_ASCII( string fname );



		c2o_obj	oct_new_obj_base( string obj_tag = "" );

		template< class T__oct_obj >
		T__oct_obj
			oct_new_obj		( string obj_tag = "" ){  return T__oct_obj( this , new_obj_id() , obj_tag ); };
		
		~cpp2oct_core(void);


		c2o_obj& f_fopen( c2o_obj& dest , string f_name , string read_mode = "r" ){ return oct_f_call(dest,"fopen",f_name,read_mode); };
		c2o_obj& f_fopen_r( c2o_obj& dest , string f_name ){ return oct_f_call(dest,"fopen",f_name,"r"); };
		void f_fclose( c2o_obj _obj ){ oct_f_call( "fclose" , _obj ); };

	};


	class c2o_exc_unreleased_obj_used : exception { };



	/*static const string O_END_INSTR;	//	instruction termination 
	static const string O_END_CL;		//	code line termination
	static const string O_EQUALS;		*///
};
