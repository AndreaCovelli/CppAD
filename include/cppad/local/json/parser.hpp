# ifndef CPPAD_LOCAL_JSON_PARSER_HPP
# define CPPAD_LOCAL_JSON_PARSER_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

  CppAD is distributed under the terms of the
               Eclipse Public License Version 2.0.

  This Source Code may also be made available under the following
  Secondary License when the conditions for such availability set forth
  in the Eclipse Public License, Version 2.0 are satisfied:
        GNU General Public License, Version 2.0 or later.
-------------------------------------------------------------------------- */

# include <string>
# include <cppad/utility/vector.hpp>
# include <cppad/local/json/operator.hpp>

/*
$begin json_parser$$
$spell
    Json
    vec
    arg
    ind
$$

$section Json AD Graph Parser$$

$head Syntax$$
$codei%parser(
    %graph%,
    %n_dynamic_ind%,
    %n_independent%,
    %string_vec%,
    %constant_vec%,
    %operator_vec%,
    %operator_arg%,
    %dependent_vec%
)%$$

$head Input and Output$$
The $icode graph$$ is the only input information.
The input value of all the other arguments does not matter
and their specifications are for their output values.

$head graph$$
The $cref json_ad_graph$$.

$head n_dynamic_ind$$
is the number of independent dynamic parameter in the function;
see $cref/dynamic/Independent/dynamic/$$.

$head n_independent$$
is the number of independent variables in the function.

$head string_vec$$
is the $cref/string_vec/json_ad_graph/string_vec/$$ for this function.

$head constant_vec$$
is the $cref/constant_vec/json_ad_graph/constant_vec/$$ for this function.

$head operator_vec$$
is the $cref/operator_vec/json_ad_graph/operator_vec/$$ for this function.

$head operator_arg$$
For $icode%i% = 0 , %...%, %n_operator%-1%$$,
for $icode%j% = 0 , %...%, %operator_vec%[%i%].n_arg%-1%$$,
the $th j$$ argument for the $th i$$ operator has node index
$codei%
    %operator_arg%[ %operator_vec%[%i%].start_arg + %j% ]
%$$.

$head dependent_vec$$
Is the $cref/dependent_vec/json_ad_graph/dependent_vec/$$ for this function.

$head Prototype$$
$srccode%hpp% */
namespace CppAD { namespace local { namespace json {

void parser(
    const std::string&                        graph                  ,
    size_t&                                   n_dynamic_ind          ,
    size_t&                                   n_independent          ,
    CppAD::vector<std::string>&               string_vec             ,
    CppAD::vector<double>&                    constant_vec           ,
    CppAD::vector<operator_struct>&           operator_vec           ,
    CppAD::vector<size_t>&                    operator_arg           ,
    CppAD::vector<size_t>&                    dependent_vec
);

} } }
/* %$$
$end
*/


# endif
