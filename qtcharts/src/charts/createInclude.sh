#!/usr/bin/bash
pwd=`pwd`
dirname=`basename $pwd`

headers=`find . -name \*.h`
for file in $headers; do
	echo Checking $file for Q_OBJECT
	qobj=`grep Q_OBJECT $file`
	isQobj=`wc -w <<< "$qobj"`
	if [ "$isQobj" -ne "0" ]; then
		qtheaders="$qtheaders $file"
	else
		non_qtheaders="$non_qtheaders $file"
	fi
done
	
src=`find . -name \*.cpp`

echo "set(qtproject_SRCS \${qtproject_SRCS}" > include.cmake
for file in $src; do
	echo "        $dirname/$file" >> include.cmake
done
echo ")" >> include.cmake
echo "" >> include.cmake

echo "set(qtproject_H \${qtproject_H}" >> include.cmake
for file in $qtheaders; do
	echo "        $dirname/$file" >> include.cmake
done
echo ")" >> include.cmake
echo "" >> include.cmake

echo "set(qtproject_UIS \${qtproject_UIS}" >> include.cmake
echo ")" >> include.cmake
echo "" >> include.cmake

echo "set(project_H \${project_H}" >> include.cmake
for file in $non_qtheaders; do
	echo "        $dirname/$file" >> include.cmake
done
echo ")" >> include.cmake
echo "" >> include.cmake


