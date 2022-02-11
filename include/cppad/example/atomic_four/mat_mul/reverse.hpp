# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_MAT_MUL_REVERSE_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_MAT_MUL_REVERSE_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
/*
$begin atomic_mat_mul_reverse.hpp$$
$spell
    Jacobian
    jac
$$

$section
Atomic Matrix Multiply Reverse Mode: Example Implementation
$$

$head Purpose$$
The $code reverse$$ routine overrides the virtual functions
used by the atomic_four base; see
$cref/reverse/atomic_four_reverse/$$.

$head Theory$$
See mat_mul $cref/reverse/atomic_mat_mul/Theory/Reverse/$$ thoery.

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <cppad/example/atomic_four/mat_mul/mat_mul.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// reverse override for Base matrix multiply
template <class Base>
bool atomic_mat_mul<Base>::reverse(
    size_t                                     call_id     ,
    const CppAD::vector<bool>&                 select_y    ,
    size_t                                     order_up    ,
    const CppAD::vector<Base>&                 taylor_x    ,
    const CppAD::vector<Base>&                 taylor_y    ,
    CppAD::vector<Base>&                       partial_x   ,
    const CppAD::vector<Base>&                 partial_y   )
{
    // q
    size_t q     = order_up + 1;
    //
    // n_left, n_middle, n_right
    size_t n_left, n_middle, n_right;
    get(call_id, n_left, n_middle, n_right);
# ifndef NDEBUG
    // n, m
    size_t n     = taylor_x.size();
    size_t m     = taylor_y.size();
    //
    // check sizes
    assert( n == n_middle * ( n_left +  n_right ) * q );
    assert( m == n_left * n_right * q );
    assert( n == partial_x.size() );
    assert( m == partial_y.size() );
# endif
    //
    // offset
    size_t x_offset = n_left * n_middle;
    //
    // ab, c, ab_offset
    // note that resize only re-alocates when capacity is not large enough
    CppAD::vector<Base> ab;
    CppAD::vector<Base> c;
    size_t ab_offset;
    //
    // partial_x
    for(size_t i = 0; i < partial_x.size(); ++i)
        partial_x[i] = Base(0);
    //
    // k
    size_t k = q;
    while(k > 0)
    {   --k;
        //
        // for ell = 0, ..., k :
        //    bar{A}^ell      += bar{C}^k [ B^{k-ell} ]^T
        //    bar{B}^{k-ell}  += [ A^ell ]^T \bar{C}^k
        for(size_t ell = 0; ell < q; ++ell)
        {   //
            // ab = [ \bar{C}^k, B^{k-ell}^T ]
            ab.resize(0);
            ab.resize( n_left * n_right + n_right * n_middle );
            ab_offset = n_left * n_right;
            for(size_t i = 0; i < n_left * n_right; ++i)
                ab[i] = partial_y[ i * q + k ];
            for(size_t i = 0; i < n_middle; ++i)
            {   for(size_t j = 0; j < n_right; ++j)
                {   size_t ij = i * n_right + j;
                    size_t ji = j * n_middle + i;
                    ab[ab_offset + ji] =
                        taylor_x[(x_offset + ij) * q + (k - ell) ];
                }
            }
            //
            // c = \bar{C} * [ B^{k-ell} ]^T
            c.resize(0);
            c.resize( n_left * n_middle );
            base_mat_mul(n_left, n_right, n_middle, ab, c);
            //
            // \bar{A}^ell += c
            for(size_t i = 0; i < n_left * n_middle; ++i)
                partial_x[i * q + ell] += c[i];
            //
            // ab = [ A^ell^T , \bar{C}^k ]
            ab.resize(0);
            ab.resize( n_middle * n_left + n_left * n_right );
            ab_offset = n_middle * n_left;
            for(size_t i = 0; i < n_left; ++i)
            {   for(size_t j = 0; j < n_middle; ++j)
                {   size_t ij = i * n_middle + j;
                    size_t ji = j * n_left + i;
                    ab[ji] = taylor_x[ij * q + ell];
                }
            }
            for(size_t i = 0; i < n_left * n_right; ++i)
                ab[ab_offset + i] = partial_y[ i * q + k ];
            //
            // c = [ A^ell ]^T * \bar{C}^k
            c.resize(0);
            c.resize( n_middle * n_right );
            base_mat_mul(n_middle, n_left, n_right, ab, c);
            //
            // \bar{B}^{k-\ell} += c
            for(size_t i = 0; i < n_middle * n_right; ++i)
                partial_x[ (x_offset + i) * q + (k - ell) ] += c[i];
        }
    }
    return true;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif