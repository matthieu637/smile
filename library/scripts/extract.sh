#!/bin/bash

function extract_data() {

file=$1

i=1

while [ 1 ]
do
	ok=`cat $file | grep "#$i" | wc -l`
	tmpfile=`mktemp`
	if [ $ok -ne 0 ] ; then
		cat $file | grep "^#$i" | sed "s/#$i//" > $tmpfile
		matrixSize=`cat $tmpfile | awk -F ' ' '{print NF+1}' | sort | head -n 1`
#		echo "$matrixSize $ok $i"
		cat $tmpfile | cut -d' ' -f-$matrixSize > "$file#$i"
		rm $tmpfile
	else
		if [ $i -gt 4 ] ; then
			break
		fi
	fi

	i=`expr $i + 1`
done


}
