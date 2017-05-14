#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ $0 != 'bin/version.sh' ]
then
	echo 'bin/version.sh: must be executed from its parent directory'
	exit 1
fi
if [ "$1" != 'get' ] && [ "$1" != 'set' ] && [ "$1" != 'check' ]
then
	echo 'usage: bin/version.sh (get | set | check) [version]'
	echo 'get:   Gets the current version number from CMakeLists.txt.'
	echo 'set:   Sets CMakeLists.txt version number to version.'
	echo '       If version is not present, uses current yyyymmdd.'
	echo 'check: Checks that the version version number in CMakeLists.txt'
	echo '       is the same as in other files. If not, changes the other'
	echo '       files and exits with an error message'
	exit 1
fi
echo_eval() {
     echo $*
     eval $*
}
# -----------------------------------------------------------------------------
if [ "$1" == 'set' ]
then
	if [ "$2" == '' ]
	then
		version=`date +%Y%m%d`
	else
		version="$2"
	fi
	echo 'sed -i.old CMakeLists.txt ...'
	sed  \
	-e "s/(\(cppad_version *\)\"[0-9.]\{8\}[0-9.]*\" *)/(\1\"$version\" )/"  \
		-i.old CMakeLists.txt
	if diff CMakeLists.txt.old CMakeLists.txt
	then
		echo 'No change in CMakeLists.txt'
	fi
	echo_eval rm CMakeLists.txt.old
	#
	echo 'version.sh set: OK'
	exit 0
fi
# -----------------------------------------------------------------------------
# get the current version number
version=`grep '^SET *( *cppad_version ' CMakeLists.txt | \
	sed -e 's|^SET *( *cppad_version *"\([0-9.]\{8\}[0-9.]*\)" *)|\1|'`
if ! (echo $version | grep '[0-9]\{8\}') > /dev/null
then
	echo 'version.sh: Cannot find verison number in CMakeLists.txt'
	exit 1
fi
if [ "$1" == 'get' ]
then
	echo "$version"
	exit 0
fi
# -----------------------------------------------------------------------------
# Make the version number in the relevant files is the same
yyyy_mm_dd=`echo $version | sed \
	-e 's|\([0-9]\{4\}\)0000|\10101|' \
	-e 's|\(....\)\(..\)\(..\).*|\1-\2-\3|'`
sed  \
	-e "s/, [0-9]\{4\}-[0-9]\{2\}-[0-9]\{2\} *,/, $yyyy_mm_dd,/" \
	-i.old authors
#
sed  \
	-e "s/(\[cppad\], *\[[0-9]\{8\}[.0-9]*\] *,/([cppad], [$version],/"  \
	-i.old configure.ac
#
sed \
	-e "s/cppad [0-9]\{8\}[.0-9]*/cppad $version/g" \
	-e "s/VERSION='[0-9]\{8\}[.0-9]*'/VERSION='$version'/g" \
	-e "s/configure [0-9]\{8\}[.0-9]*/configure $version/g" \
	-e "s/config.status [0-9]\{8\}[.0-9]*/config.status $version/g" \
	-e "s/\$as_me [0-9]\{8\}[.0-9]*/\$as_me $version/g" \
	-e "s/Generated by GNU Autoconf.*$version/&./" \
	-i.old configure
list='
	omh/install/autotools.omh
	omh/install/download.omh
	doc.omh
'
for file in $list
do
	sed -e "s/cppad-[0-9]\{8\}\(\.[eg]pl\)/cppad-$version\1/" \
		-e "s/cppad-[0-9]\{8\}\.[0-9]*\(\.[eg]pl\)/cppad-$version\1/" \
		-e "s/cppad-[0-9]\{8\}[0-9.]*\:/cppad-$version:/" \
		-i.old $file
done
list="
	$list
	authors
	configure.ac
	configure
"
ok='yes'
for name in $list
do
	if ! diff $name.old $name > /dev/null
	then
		echo '-------------------------------------------------------------'
		echo "diff $name.old $name"
		if diff $name.old $name
		then
			echo 'version.sh check: program error'
			exit 1
		fi
		ok='no'
	fi
	rm $name.old
done
if [ "$ok" != 'yes' ]
then
	echo 'version.sh check: Found and fixed differences'
	exit 1
fi
echo 'version.sh check: OK'
exit 0
