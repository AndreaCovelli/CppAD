#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
cat << EOF
Bug report corresponding to:
	https://github.com/coin-or/CppAD/issues/7

This program files with the following assertion:
	Error detected by false result for
		IdenticalPar(left)
	at line 126 in the file 
		../../cppad/local/cskip_op.hpp
EOF
cat << EOF > bug.$$
#include <iostream>
#include <cppad/cppad.hpp>
int main(void) {
	using namespace CppAD;

	typedef AD<double> adouble;
	typedef AD<adouble> a2double;

	std::vector<double> x{0, 1};

	/**
	* First tape 
	* (using AD<AD<double> >)
	*/
	std::vector<a2double> a2x(x.size());
	for (size_t i = 0; i < a2x.size(); i++) {
	a2x[i] = adouble(x[i]);
	}
	Independent(a2x);

	std::vector<a2double> a2y(1);
	a2double a = a2x[0] * a2x[1];
	a2y[0] = CondExpEq(a2x[0], a2double(1.0), a, a2double(0.0));

	// create f: X -> Y 
	ADFun<adouble> f1(a2x, a2y);
	f1.optimize();

	/**
	* Second tape
	* (using AD<double>)
	*/
	std::vector<adouble> ax{adouble(1), adouble(0)};
	Independent(ax);

	std::vector<adouble> ay = f1.Forward(0, ax);

	CppAD::ADFun<double> f2(ax, ay);

	/**
	* Use second tape
	*/
	x = {1, 0.5};

	std::vector<double> y = f2.Forward(0, x);

	//std::cout << y[0] << std::endl;
	assert(std::abs(y[0] - x[0] * x[1]) < 1e-10);

}
EOF
# -----------------------------------------------------------------------------
if [ ! -e build ]
then
    mkdir build
fi
cd build
echo "$0"
name=`echo $0 | sed -e 's|.*/||' -e 's|\..*||'`
mv ../bug.$$ $name.cpp
echo "g++ -I../.. --std=c++11 -g $name.cpp -o $name"
g++ -I../.. --std=c++11 -g $name.cpp -o $name
#
echo "./$name"
if ./$name
then
    echo "OK"
else
    echo "Error"
fi
