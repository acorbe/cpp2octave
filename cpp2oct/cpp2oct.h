#ifndef CPP2OCT_H
#define CPP2OCT_H

#include <vector>
#include <valarray>
#include <list>

#include "cpp2oct_core.h"
#include "c2o_datafile_gen.h"



using namespace std;

namespace cpp2octave {

	class c2o_vct1 : 
		public c2o_obj
	{
	private:
		friend class cpp2oct_core;
	protected:
		c2o_vct1( cpp2oct_core * _owner , int obj_num_id , string obj_tag ) : c2o_obj( _owner , obj_num_id , obj_tag ) { };

	public:

		/*
		template< class InputIterator > //TODO
		c2o_vct & assign_iter( InputIterator begin , InputIterator end );
		
		template< class T > //TODO
		c2o_vct & assign_cvec( const T* vect , int dim );

		template< class T > //TODO
		c2o_vct & assign_vect( const vector< T >& vect );

		template< class T > //TODO
		c2o_vct & assign_vect( const valarray< T >& vect );

		template< class T > //TODO
		c2o_vct & assign_conv( const T* obj );
		*/

		void set_linspace( double inf_lim , double sup_lim , int N_points){ owner->oct_f_call( *this , "linspace" , c2o_arg( inf_lim , sup_lim , N_points ) ); };
				

		c2o_vct1 & set(string oct_script){ return *this; /* SHIELDED -- does it make sense? */ };

	};

	class cpp2oct :
		public cpp2oct_core
	{
	private:
		typedef std::list< c2o_datafile_ptr > datafile_set_t;
		datafile_set_t datafile_set;
		static bool delete_datafile_set_ptrs( c2o_datafile_ptr theElement ) { delete theElement; return true; };


	public:
		cpp2oct( string _env_name );
		


		c2o_vct1 oct_new_vct1( string obj_tag = "" ) { return oct_new_obj< c2o_vct1 > ( obj_tag ); };

		template< class T >
		c2o_obj& oct_vct_obj_assign(c2o_obj& dest , const T* vect , size_t dim ){ return this->get_current_datafile()->serial_write( dest , vect , dim ); };

		template< class T >
		c2o_obj& oct_vct_obj_assign(c2o_obj& dest , const vector< T >& vect ){ return this->get_current_datafile()->serial_write(dest , &(vect[0]) , vect.size() ); };

		template< class T >
		c2o_obj& oct_vct_obj_assign(c2o_obj& dest , const valarray< T >& vect ){ return this->get_current_datafile()->serial_write(dest , &(vect[0]) , vect.size() ); };

		/*
		template< class T >
		c2o_vct1& oct_vct1_assign(c2o_vct1& dest , const T* obj , void * param = NULL );
		*/

		template< class P__ >
		c2o_datafile_ptr new_c2o_datafile_gen( string f_name ){
			c2o_datafile_ptr t = new c2o_datafile_gen< P__ >( this , f_name );
			datafile_set.push_back( t );
			return t;
		}

		/*
		*	Functions
		*/
#define FOO_NO_RET_0ARG( fname ) void f_##fname( ){ oct_f_call( #fname ); };

#define FOO_NO_RET_1ARG_INC_TYPE( fname , input_type ) void f_##fname( input_type arg ){ oct_f_call(#fname, arg ); };
#define FOO_NO_RET_1ARG_ARG_TYPE(fname) template < class T > FOO_NO_RET_1ARG_INC_TYPE( fname , T ) // void f_##fname( c2o_arg arg ){ oct_f_call(#fname, arg ); };

#define FOO_NO_RET_2ARG_INC_TYPE( fname , input_type1 , input_type2 ) void f_##fname( input_type1 arg1 , input_type2 arg2 ){ oct_f_call(#fname, arg1 , arg2 ); };
#define FOO_NO_RET_2ARG_ARG_TYPE( fname ) template < class T1 , class T2 > FOO_NO_RET_2ARG_INC_TYPE( fname , T1 , T2 )

#define FOO_NO_RET_3ARG_INC_TYPE( fname , input_type1 , input_type2 , input_type3 ) void f_##fname( input_type1 arg1 , input_type2 arg2 , input_type3 arg3 ){ oct_f_call( #fname, arg1 , arg2 , arg3 ); };
#define FOO_NO_RET_3ARG_ARG_TYPE( fname ) template < class T1 , class T2 , class T3 > FOO_NO_RET_3ARG_INC_TYPE( fname , T1 , T2 , T3 )

		//void plot( c2o_arg arg ){ oct_f_call("plot", arg ); };
		FOO_NO_RET_1ARG_ARG_TYPE( plot );
		FOO_NO_RET_2ARG_ARG_TYPE( plot );
		FOO_NO_RET_3ARG_ARG_TYPE( plot );

		FOO_NO_RET_1ARG_ARG_TYPE( mesh );

		FOO_NO_RET_1ARG_ARG_TYPE( disp );

		FOO_NO_RET_0ARG( pause );
		FOO_NO_RET_1ARG_INC_TYPE( pause , double );


		virtual ~cpp2oct(void);		
				
		
		

	


	};

};



#endif /* CPP2OCT_H */
