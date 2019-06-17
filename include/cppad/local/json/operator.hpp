# ifndef CPPAD_LOCAL_JSON_OPERATOR_HPP
# define CPPAD_LOCAL_JSON_OPERATOR_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

  CppAD is distributed under the terms of the
               Eclipse Public License Version 2.0.

  This Source Code may also be made available under the following
  Secondary License when the conditions for such availability set forth
  in the Eclipse Public License, Version 2.0 are satisfied:
        GNU General Public License, Version 2.0 or later.
-------------------------------------------------------------------------- */

// BEGIN_OPERATOR_ENUM
namespace CppAD { namespace local { namespace json {
    enum operator_enum  {
        add_operator = 0,   // has 2 arguments and 1 result
        mul_operator = 1,   // has 2 arguments and  1 result
        n_operator   = 2    // number of operators defined so far
    };
} } }
// END_OPERATOR_ENUM

// BEGIN_CPPAD_LOCAL_JSON_NAMESPACE
namespace CppAD { namespace local { namespace json {

extern const char* operator_name[];

struct operator_struct {
    size_t  n_result;
    size_t  n_arg;
    size_t  start_arg;
    // code may be smaller so put it last for alignment
    operator_enum code;

};


} } } // END_CPPAD_LOCAL_JSON_NAMESPACE


# endif
