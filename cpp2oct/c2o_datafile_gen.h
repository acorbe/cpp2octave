#pragma once
#include "cpp2oct_core.h"

using namespace std;

namespace cpp2octave {

	namespace c2o_datafile_gen_policies{
		class c2o_datafile_WM_ASCII { 
		public:
			static const string WM_FILE_PREFIX; // = "ASCII"
			static const string OCT_READ_MODE; //="r"

			static const ios_base::openmode WM_IOS_MODE;// = ios::out;

			template< class T >	class reading_tags { };



			template< class T >
			static c2o_obj& serial_write(
				bool octFileIsOpen,
				c2o_obj& dest , 
				const T* vect , 
				size_t N , 
				ofstream& out_file , 
				c2o_obj & out_file_link ,
				cpp2oct_core * owner
				){
					if( octFileIsOpen ){
						stringstream o;
						for( int i = 0 ; i < N ; i++ ){
							o << vect[i] << " " ;
						}
						o << "\n";
						out_file << o.str();

						owner->oct_f_call( 
							dest , 
							"fscanf" , 
							out_file_link ,
							reading_tags< T >::READING_TAG , 
							static_cast<int> ( N )
						);
					}

					return dest;
			}
		};

		template< >
		class c2o_datafile_WM_ASCII::reading_tags< double > { public: static const string READING_TAG; };

		template< >
		class c2o_datafile_WM_ASCII::reading_tags< int > { public: static const string READING_TAG; };

		class c2o_datafile_WM_BIN { 
		public:
			static const string WM_FILE_PREFIX; //= "BIN"
			static const string OCT_READ_MODE; //="rb"

			static const ios_base::openmode WM_IOS_MODE;// = ios::out || ios::binary;

			template< class T >	class reading_tags { };



			template< class T >
			static c2o_obj& serial_write(
				bool octFileIsOpen,
				c2o_obj& dest , 
				const T* vect , 
				size_t N , 
				ofstream& out_file , 
				c2o_obj & out_file_link ,
				cpp2oct_core * owner
				){

					if( octFileIsOpen ){
						out_file.write ((char*)vect, N * sizeof (T));

						owner->oct_f_call( 
							dest , 
							"fread" , 
							out_file_link ,
							static_cast<int> ( N ),
							reading_tags< T >::READING_TAG 
						);
					}

					return dest;			

			}
		};
		template< >
		class c2o_datafile_WM_BIN::reading_tags< double > { public: static const string READING_TAG; };

		template< >
		class c2o_datafile_WM_BIN::reading_tags< int > { public: static const string READING_TAG; };
	};


using namespace c2o_datafile_gen_policies;


typedef c2o_datafile_gen_policies::c2o_datafile_WM_ASCII	c2o_datafile_P_ASCII;
typedef c2o_datafile_gen_policies::c2o_datafile_WM_BIN		c2o_datafile_P_BIN;


template< class P__ >
class c2o_datafile_gen :
	public c2o_datafile_handler
{
private:
	ofstream out_file;
	c2o_obj out_file_link;
	bool octFileIsOpen;

	void out_file_init( ){
		stringstream s;
		s << path << DIR_SEP_CH << f_name;
		string fname_str = s.str();
		out_file.open( fname_str.c_str() , /*ios::out | ios::binary*/ P__::WM_IOS_MODE );

		owner->f_fopen( out_file_link , fname_str , P__::OCT_READ_MODE );
		octFileIsOpen = true;
	};

public:
	c2o_datafile_gen( cpp2oct_core * _owner , string _f_name ) : 
	  c2o_datafile_handler( _owner , _f_name ),
		  out_file_link(_owner->oct_new_obj_base( string(P__::WM_FILE_PREFIX).append("_f_").append(_f_name)) )
	  
	{
		out_file_init( );
	};

	c2o_obj& serial_write( c2o_obj& dest , const int* vect , size_t N ){
		return P__::serial_write( octFileIsOpen , dest , vect , N , out_file , out_file_link , owner );
	}

	c2o_obj& serial_write( c2o_obj& dest , const double* vect , size_t N ){
		return P__::serial_write( octFileIsOpen , dest , vect , N , out_file , out_file_link , owner);
	}

	void datafile_close( void ){ 
		if( octFileIsOpen ){
			out_file.close() ; 
			owner->f_fclose( out_file_link ); 
			octFileIsOpen = false;
		}
	};

	~c2o_datafile_gen(void){
		datafile_close( );
	};
};

}
