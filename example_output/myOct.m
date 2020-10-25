% Created through cpp2octave 
% A C++ library by Alessandro Corbetta.

disp( 'Hello World!' );
disp( 'Hello World!' );
obj0_ = rand( 3 );
disp( obj0_ );
obj0_ = rand( 10 , 20 );
disp( obj0_ );
obj0_ = [ 0 0.01 0.02 0.03 0.04 0.05 0.06 0.07 0.08 0.09 0.1 0.11 0.12 0.13 0.14 0.15 0.16 0.17 0.18 0.19 0.2 0.21 0.22 0.23 0.24 0.25 0.26 0.27 0.28 0.29 0.3 0.31 0.32 0.33 0.34 0.35 0.36 0.37 0.38 0.39 0.4 0.41 0.42 0.43 0.44 0.45 0.46 0.47 0.48 0.49 0.5 0.51 0.52 0.53 0.54 0.55 0.56 0.57 0.58 0.59 0.6 0.61 0.62 0.63 0.64 0.65 0.66 0.67 0.68 0.69 0.7 0.71 0.72 0.73 0.74 0.75 0.76 0.77 0.78 0.79 0.8 0.81 0.82 0.83 0.84 0.85 0.86 0.87 0.88 0.89 0.9 0.91 0.92 0.93 0.94 0.95 0.96 0.97 0.98 0.99 ];
obj1_ = [ 0 0.0001 0.0004 0.0009 0.0016 0.0025 0.0036 0.0049 0.0064 0.0081 0.01 0.0121 0.0144 0.0169 0.0196 0.0225 0.0256 0.0289 0.0324 0.0361 0.04 0.0441 0.0484 0.0529 0.0576 0.0625 0.0676 0.0729 0.0784 0.0841 0.09 0.0961 0.1024 0.1089 0.1156 0.1225 0.1296 0.1369 0.1444 0.1521 0.16 0.1681 0.1764 0.1849 0.1936 0.2025 0.2116 0.2209 0.2304 0.2401 0.25 0.2601 0.2704 0.2809 0.2916 0.3025 0.3136 0.3249 0.3364 0.3481 0.36 0.3721 0.3844 0.3969 0.4096 0.4225 0.4356 0.4489 0.4624 0.4761 0.49 0.5041 0.5184 0.5329 0.5476 0.5625 0.5776 0.5929 0.6084 0.6241 0.64 0.6561 0.6724 0.6889 0.7056 0.7225 0.7396 0.7569 0.7744 0.7921 0.81 0.8281 0.8464 0.8649 0.8836 0.9025 0.9216 0.9409 0.9604 0.9801 ];
plot( obj0_ , obj1_ );
hold on;
obj2_BIN_f_data = fopen( './support_myOct/data' , 'rb' );
obj1_ = fread( obj2_BIN_f_data , 100 , 'double' );
plot( obj0_ , obj1_ , 'r' );
fclose( obj2_BIN_f_data );
