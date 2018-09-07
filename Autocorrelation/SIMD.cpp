#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include "simd.p5.h"


#define SIZE   32768
float Array[2*SIZE];
float  Sums[1*SIZE];

int main(int argc, char *argv[]){

	// Read in the signal file into the array
	FILE *fp = fopen( "signal.txt", "r" );
	if( fp == NULL ){
		fprintf( stderr, "Cannot open file 'signal.txt'\n" );
		exit( 1 );
	}

	int Size;
	fscanf( fp, "%d", &Size);
	Size = SIZE;

	for( int i = 0; i < Size; i++ ){
		fscanf(fp, "%f", &Array[i]);
		Array[i+Size] = Array[i];		// duplicate the array
	}
	fclose( fp );

	float temp;
	// Get start time
	double time0 = omp_get_wtime();
	
	for( int shift = 0; shift < Size; shift++ ){
		temp = SimdMulSum(Array, &Array[shift], SIZE);	// note the "fix #2" from false sharing if you are using OpenMP
		Sums[shift] = temp;
	}
	
	// Get end time
	double time1 = omp_get_wtime();
	
	// Calculate and print performance results
	double MegaAdds = ((double)(SIZE * SIZE)/(time1-time0)/1000000);
	printf("SIMD Performance = %8.2lf MegaBodies/Sec\n", MegaAdds);
	
	return 0;
}
