#include <stdio.h>
#include <omp.h>
#include <stdlib.h>


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

	// Set number of threads to use
	omp_set_num_threads(THREADS);
	
	// Get start time
	double time0 = omp_get_wtime();
	
	// Parallelize operations
	#pragma omp parallel for default(none) shared(Sums, Array, Size)
	for( int shift = 0; shift < Size; shift++ ){
		float sum = 0.;
		for( int i = 0; i < Size; i++ ){
			sum += Array[i] * Array[i + shift];
		}
		Sums[shift] = sum;	// note the "fix #2" from false sharing if you are using OpenMP
	}
	
	// Get end time
	double time1 = omp_get_wtime();
	
	// Calculate and print performance results
	double MegaAdds = ((double)(SIZE * SIZE)/(time1-time0)/1000000);
	printf("OpenMP Performance = %8.2lf MegaBodies/Sec for Threads = %d\n", MegaAdds, THREADS);

// Used to print out numbers for sine wave scatterplot
/*
	for(int j = 1; j <513; j++){
		printf("%f\n", Sums[j]);
	}
*/	
	return 0;
}
