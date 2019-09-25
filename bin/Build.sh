#!/bin/bash
# var init
bin_dir=`pwd`
cd ../src/
src_dir=`pwd`

# clean files
make clean
if [ -f "./libez.a" ];then
	echo "Clean error!"
	exit 0
else
	echo "Clean finished!"
fi

# complie and link
make -B
if [ -f "./libez.a" ];then
	echo "Build succeed!"
fi

# pack files
mkdir -p $bin_dir/dist/
cp -rf *.h "$bin_dir/dist"
cp libez.a "$bin_dir/dist"

tar -czPf $bin_dir/dist.tar.gz $bin_dir/dist
if [ -f "$bin_dir/dist.tar.gz"  ];then
	echo "Pack success!"
fi
