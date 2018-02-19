#!/bin/bash
path=/home/atkrye/Documents/EGGP
projpath=/home/atkrye/Documents/EGGP
pgp2P=( MutateFeedForwardEdge MutateNode GenerateIndividual )
eggpsrc=( eggp.c eggp.h common_functions.c common_functions.h )
projfiles=( circuit.c )
lib_path=/P-GP2-Lib
for pgp2prog in ${pgp2P[@]}
do
    echo "Build $pgp2prog.gp2"
    dirname=$path/$pgp2prog
    rm -rf $dirname && mkdir $dirname
    echo "$path/pgp2 -l $path$lib_path -m $pgp2prog -o $path/$pgp2prog $path/$pgp2prog.gp2"
    $path/gp2 -l $path$lib_path -m $pgp2prog -o $path/$pgp2prog $path/$pgp2prog.gp2
done
rm -rf $path/Build && mkdir $path/Build
for pgp2prog in ${pgp2P[@]}
do
    echo "Copy $pgp2prog"
    dirname=$path/$pgp2prog/.
    cp -a $dirname $path/Build
done
cp $path/Makefile $path/Build/Makefile
for eggps in ${eggpsrc[@]}
do
    echo "Copy $eggps"
    cp $path/$eggps $path/Build/$eggps
done
for projf in ${projfiles[@]}
do
    echo "Copy $projf"
    cp $projpath/$projf $path/Build/$projf
done
cd $path/Build
make
cp $path/Build/eggp $path/eggp2
