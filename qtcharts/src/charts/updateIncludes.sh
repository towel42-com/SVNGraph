#!/usr/bin/bash
headers=`find . -name \*.h` 
privateHeaders=""
publicHeaders=""
dir=""
for file in $headers; do
	currdir=`dirname $file`
	if [ "$currdir" != "$dir" ]; then
		dir=$currdir
		echo "Checking $currdir - $dir"
	fi
	#echo Checking $file for private
	newName=`echo $file | sed s/_p//`
	if [ "$newName" == "$file" ]; then
		publicHeaders="$publicHeaders $file"
	else
		privateHeaders="$privateHeaders $file"
	fi
done

for file in $privateHeaders; do
	base=`basename $file`
	tgt=../../QtCharts/private/$base
#	if [ $file -nt $tgt ]; then
		cp -p $file ../../QtCharts/private
#	else
#		echo "Skipping $file as $tgt is newer"
#	fi
done

for file in $publicHeaders; do
	base=`basename $file`
	tgt=../../QtCharts/$base
#	if [ $file -nt $tgt ]; then
		cp -p $file ../../QtCharts
#	else
#		echo "Skipping $file as $tgt is newer"
#	fi
done


	


