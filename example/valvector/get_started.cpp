# include <cppad/example/valvector/class.hpp>
# include <cppad/cppad.hpp>
//
// get_started
bool get_started(void)
{  // ok
   bool ok = true;
   //
   // ad_valvector
   typedef CppAD::AD<valvector> ad_valvector;
   //
   // ax
   CPPAD_TESTVECTOR( ad_valvector ) ax(2);
   ax[0] = valvector( {1.0} );
   ax[1] = valvector( {2.0} );
   CppAD::Independent(ax);
   //
   // f
   CPPAD_TESTVECTOR( ad_valvector ) ay(1);
   ay[0] = ax[0] * ax[1];
   CppAD::ADFun<valvector> f(ax, ay);
   //
   // x
   CPPAD_TESTVECTOR( valvector ) x(2);
   x[0] = valvector( {1.0, 2.0, 3.0} );
   x[1] = valvector( {4.0, 3.0, 2.0} );
   //
   // y
   CPPAD_TESTVECTOR( valvector ) y(1);
   y = f.Forward(0, x);
   //
   // ok
   valvector check;
   check = valvector( {4.0, 6.0, 6.0} );
   ok   &= y[0] == check;
   //
   //  dw
   CPPAD_TESTVECTOR( valvector ) w(1), dw(2);
   w[0] = valvector( 1.0 );
   dw   = f.Reverse(1, w);
   //
   // ok
   ok &= dw[0] == x[1];
   ok &= dw[1] == x[0];
   //
   return ok;
}
