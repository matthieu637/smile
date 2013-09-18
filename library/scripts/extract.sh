#!/bin/bash

function extract_data() {

file=$1

i=1

while [ 1 ]
do
	ok=`cat $file | grep "#$i" | wc -l`
	if [ $ok -ne 0 ] ; then
		cat $file | grep "^#$i" | sed "s/#$i//" > "$file#$i"
	else
		if [ $i -gt 4 ] ; then
			break
		fi
	fi

	i=`expr $i + 1`
done


}
