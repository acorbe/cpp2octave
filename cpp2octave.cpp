#include <algorithm>

//Header to be included to use the library cpp2octave
#include "cpp2oct.h"

//Namespace used from now on
using namespace cpp2octave;

int main(int argc, char* argv[])
{
	
	// A simple demo of the library.
	// An octave script named "myOct.m" will be initialized in the default path.
	// A directory named support_myOct will be created as well to contain data and support files.
	cpp2octave::cpp2oct myCore("myOct");

	// "Hello world!" is displayed by the console
	myCore.f_disp("Hello World!");
	// RESULT in myOct.m: disp( 'Hello World!' );

	// The latter is indeed a shortcut of the more general call
	myCore.oct_f_call("disp","Hello World! 2");
	// RESULT in myOct.m: disp( 'Hello World!' );

	// You can instantiate octave objects, as the following object "ob".
	// "Ob" will serve as an handler/pointer to the octave object "obj0_" automatically created. Indeed you don't need to consider "obj0_", in the cpp side you will use just "ob".
	c2o_obj ob = myCore.oct_new_obj_base();
	
	// You can ask octave to calculate a 3x3 random matrix and to assign it to the object handled by ob.
	myCore.oct_f_call(ob,"rand",3);
	// RESULT in myOct.m: obj0_ = rand( 3 );
	//
	// Let's display the matrix
	myCore.f_disp(ob);
	// RESULT in myOct.m: disp( obj0_ );
	
	// You may want a 3x4 random matrix. Nothing easier...
	myCore.oct_f_call(ob,"rand",3,4);
	// RESULT in myOct.m: obj0_ = rand( 3 , 4 );
	//
	// Again, let's display the matrix
	myCore.f_disp(ob);
	// RESULT in myOct.m: disp( obj0_ );

	// Good, let's take a brake
	//	myCore.f_pause();
	// RESULT in myOct.m: pause( );
	
	// Ok, nothing really useful so far, but...
	// Let's suppose our code performs some very interesting calculations, and we want to plot them.

	// Let's consider the C++ vectors which we will use as support for our calculation. 
	// In the end we want something like v_y = foo( v_x );
	const int v_size = 100;
	vector<double> v_x(v_size), v_y(v_size);

	// Let us fill v_x with some regular sample
	const double X_STEP = .01;
	for(int i = 0; i < v_size ; i++ ){ v_x[i] = i * X_STEP ; };
	// and say that y = x^2
	std::transform(v_x.begin() , v_x.end() , v_y.begin() , [] ( double x ){ return x*x ; }); 
	
	
	// OK, time to communicate our choice to octave	
	myCore.oct_vct_obj_assign( ob , v_x );
	// RESULT in in myOct.m: obj0_ = [ 0 , .01 , .02, ...];

	// We need a new octave object to store the values of v_y in
	c2o_obj ob2 = myCore.oct_new_obj_base();
	myCore.oct_vct_obj_assign(ob2,v_y);
	// RESULT in in myOct.m: obj1_ = [ 0 , .0001 , .0004, ...];

	// Now it is time for the magic to happen
	myCore.f_plot( ob , ob2 );
	// RESULT in in myOct.m: plot( obj0_ , obj1_ );

	// Let us bring the plot along...
	myCore.oct_c_call("hold","on");
	// RESULT in in myOct.m: hold on ;
	// the member function oct_c_call is intended for instructions as grid on; and so on.


	// OK, having vector written inline in the script is not very efficient...
	// Let's create a new binary datafile reckognised by the tag "data"
	// the support file "./support_myOct/data" will be created.
	c2o_datafile_ptr data_h = myCore.new_c2o_datafile_gen< c2o_datafile_P_BIN >( "data" );
	// By using the policy c2o_datafile_P_ASCII instead of c2o_datafile_P_BIN the data file would have been binary

	// Let us set data_h as the default outstream for the converted vectors.
	myCore.switch_datafile( data_h );

	// Say that y = x*sin(10*x)
	std::transform( v_x.begin() , v_x.end() , v_y.begin() , [] ( double x ){ return x*std::sin(10*x) ; }); 

	// Let us plot the new function, now in red.
	myCore.f_plot(ob , myCore.oct_vct_obj_assign( ob2,v_y) , "r" );

	// data_h->datafile_close();
	// Memory is managed, so the open file is closed in octave automatically.
	// you can do it on your own though.



	//delete data_h;
	//myCore.f_plot(ob);

	

	//myCore.oct_f_call(ob2,"linspace",c2o_arg(10,20,3));

	//ob2.set_linspace(1.2,2,10);

	//c2o_obj ob3 = myCore.oct_new_obj_base();

	//myCore.oct_f_call(ob3,"sin",ob2);

	//myCore.f_plot(c2o_arg(ob2,ob3));


	//c2o_datafile_ASCII outf(&myCore,"data"); 

	//myCore.f_plot(outf.serial_write(ob2,v,4));

	//outf.datafile_close();

	/*double v[] = {3.1 , 2.3 , 5.6 , 4.0};

	c2o_datafile_gen< c2o_datafile_gen_policies::c2o_datafile_WM_ASCII >
		outf2( &myCore ,"data2");

	myCore.f_plot(outf2.serial_write(ob2,v,4),"r");
	myCore.f_plot(ob,outf2.serial_write(ob2,&v_y[0],v_y.size()),"g");

	outf2.datafile_close();*/
	


	//getchar();
	return 0;
}

