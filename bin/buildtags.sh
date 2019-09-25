#!/bin/bash
cd ../
path=`pwd`
echo "rebuild path:"$path
files=$(ls $path)
for filename in $files
do
	if [[ ${filename:0:6} == "cscope" ]];then
		rm cscope*
		echo "remove cscope files..."
		break
	fi
done

if [ -f tags ];then
	rm tags
	echo "remove tags..."
fi
ctags -R
echo "build tags..."
cscope -Rbqk
echo "build cscope..."
echo "build succeed!"
