#!/bin/bash
function control_c() {
	if [ -e $out ]
	then
		rm -f $out
	fi
}

function make_temp() {
	if [ $(uname) == "Darwin" ]; then
		out=$(mktemp -t $PROG)
	else
		out=$(mktemp --suffix=$PROG)
	fi
}

function check_leaks {
	got_leak=$(grep -E -c "are definitely lost" $out)
	if [ "$got_leak" -ne 0 ]
	then
		echo "$PROG have a memory leak for test case $i"
	fi
}

function check_invalid_read {
	got_invalid_read=$(grep -E -c "Invalid read" $out) 
	use_uninit_value=$(grep -E -c "uninitialised value" $out)
	if [ "$got_invalid_read" -ne 0 ] || [ "$use_uninit_value" -ne 0 ]
	then
		echo "$PROG has an invalid memory read for test case $i"
	fi
}

function check_invalid_write {
	got_invalid_write=$(grep -E -c "Invalid write" $out)
	if [ "$got_invalid_write" -ne 0 ]
	then
		echo "$PROG has an invalid memory write for test case $i"
	fi
}

trap control_c INT
PROG=$1
i=1
if [ ! -e $PROG ]
then
	echo "$PROG does not exist.  Have you compiled it with make?"
	exit 1
fi
while true
do
	if [ -e inputs/$PROG.$i.in ] && [ -e outputs/$PROG.$i.out ]
	then
		make_temp
		timeout 5s valgrind --leak-check=full ./$PROG < inputs/$PROG.$i.in 2>> $out > /dev/null
		status="$?"
		if [ "$status" -ne "0" ]
		then
			echo "$PROG: return non-zero status $status for test case $i"
		else 
			if [ -e $out ] 
			then
				check_leaks
				check_invalid_read	
				check_invalid_write	
				rm -f $out
			else
				echo "$PROG: cannot find output file. Execution interrupted?"
			fi
		fi
		i=$((i + 1))
	else
		break
	fi
done

if [ $i -eq 1 ] 
then
	echo "$PROG: no test cases found"
	exit 0
fi

if [ "$got_invalid_write" -eq 0 ] && [ "$got_invalid_read" -eq 0 ] && [ "$got_leak" -eq 0 ] && [ "$use_uninit_value" -eq 0 ]
then
	exit 0
else
	exit 1
fi
# vim:noexpandtab:sw=4:ts=4
