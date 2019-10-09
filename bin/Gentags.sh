#!/bin/bash
cd ../
path=`pwd`
echo "Build path:"$path
files=$(ls $path)
for filename in $files
do
	if [[ ${filename:0:6} == "cscope" ]];then
		rm cscope*
		echo "Remove cscope files..."
		break
	fi
done

if [ -f tags ];then
	rm tags
	echo "Remove tags..."
fi
ctags -R
echo "Build tags..."
cscope -Rbq
echo "Build cscope..."
echo "Build succeed!"
