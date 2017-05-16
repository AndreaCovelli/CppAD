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
spell_list='
'
revert_list='
'
move_list='
'
move_sed='s|test_runner.hpp|test_boolofvoid.hpp|'
#
cat << EOF > junk.sed
s|group = "cppad_ipopt_example"|group = "cppad_ipopt/example"|
s|group = "cppad_ipopt_test"|group = "cppad_ipopt/test"|
s|group = "example_atmoic"|group = "example/atmoic"|
s|group = "example_ipoot_solve"|group = "example/ipopt_solve"|
s|group = "example_optimize"|group = "example/optimize"|
s|group = "example_sparse"|group = "example/sparse"|
s|group = "example_utility"|group = "example/utility"|
s|group = "introduction_exp_apx"|group = "introduction/exp_apx"|
s|group = "test_more"|group = "test_more"|
EOF
# -----------------------------------------------------------------------------
if [ $0 != "bin/batch_edit.sh" ]
then
	echo "bin/batch_edit.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
cp bin/batch_edit.sh $HOME/trash/batch_edit.sh
git reset --hard
# ---------------------------------------------------------------------------
list_all=`bin/ls_files.sh | sed \
	-e '/^compile$/d' \
	-e '/^config\.guess$/d' \
	-e '/^config\.sub$/d' \
	-e '/^configure$/d' \
	-e '/^depcomp$/d' \
	-e '/^epl-v10.txt$/d' \
	-e '/^gpl-v10.txt$/d' \
	-e '/^makefile.in$/d' \
	-e '/\/makefile.in$/d' \
	-e '/^missing$/d'`
edit_list=''
for file in $list_all
do
	if [ "$file" != 'bin/batch_edit.sh' ]
	then
		sed -f junk.sed  $file > junk.$$
		if ! diff $file junk.$$ > /dev/null
		then
			echo_eval sed -f junk.sed  -i $file
			edit_list="$edit_list $file"
		fi
	fi
done
if [ -e junk.$$ ]
then
	rm junk.$$
fi
# ----------------------------------------------------------------------------
for word in $spell_list
do
#
cat << EOF > junk.sed
/\$spell\$/! b skip
:loop
/\$\\\$/b check
N
b loop
#
: check
/$word/b skip
s/\$spell/&\\n\\t$word/
#
: skip
EOF
	for file in $edit_list
	do
		echo_eval sed -f junk.sed -i $file
	done
done
# ----------------------------------------------------------------------------
for old in $move_list
do
	new=`echo $old | sed -e "$move_sed"`
	if [ -e "$new" ]
	then
		rm -r "$new"
	fi
	dir=`echo $new | sed -e 's|/[^/]*$||'`
	if [ "$dir" != "$new" ] && [ ! -e "$dir" ]
	then
		echo_eval mkdir $dir
	fi
	echo_eval git mv $old $new
done
# ----------------------------------------------------------------------------
# files that were hand edited and cached using 'git_new.sh to'
if [ -e new ]
then
	echo_eval git_new.sh from
fi
# ----------------------------------------------------------------------------
for file in $revert_list
do
	echo_eval git checkout $file
done
# ----------------------------------------------------------------------------
cp $HOME/trash/batch_edit.sh bin/batch_edit.sh
echo "$0: OK"
exit 0
