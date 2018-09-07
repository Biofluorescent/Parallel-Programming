#!/bin/bash

echo Running OpenMp version...

# Thread counts
for thread in 1 8
do
		g++ -DTHREADS=$thread  OpenMP.cpp -o proj7Bomp -lm -fopenmp
		./proj7Bomp
done

echo Running SIMD version...
/usr/local/common/gcc-6.3.0/bin/g++ -c simd.p5.cpp -o simd.p5.o
g++ SIMD.cpp -o proj7Bsimd simd.p5.o -lm -fopenmp
./proj7Bsimd

echo Running OpenCL version...
g++ OpenCL.cpp -o proj7Bocl /scratch/cuda-7.0/lib64/libOpenCL.so -lm -fopenmp
./proj7Bocl
