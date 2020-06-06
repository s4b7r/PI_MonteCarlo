/*
 * main.c
 *
 *  Created on: 25.01.2014
 *      Author: sieb
 */

#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <string.h>

int main( int argc, char *argv[] )
{

	time_t t;
	time_t tnew;
	time_t tstart;
	time_t tend;
	int numTasks = 2;
	int rank = 0;
	int ex = 6;
	unsigned long long int numPoints = 0;
	unsigned long long int numPerThread = 0;
	int i = 0;
	long double rndX = 0;
	long double rndY = 0;
	long double rndR = 0;
	int r = 1;
	unsigned long long int privCount = 0;
	unsigned long long int sumCount = 0;
	unsigned long long int nIn = 0;
	unsigned long long int sumIn = 0;
	long double pi = 0;
	int mpiargc = 0;
	char **mpiargv = malloc(sizeof(argv));
	unsigned long long int p = 0;

	for( i = 0; i < argc; i++ )
	{
		if( strcmp(argv[i], "-n") == 0 )
		{
			ex = atol(argv[i+1]);
			i++;
		}
		else if( strcmp(argv[i], "-p") == 0 )
		{
			p = atol(argv[i+1]);
			i++;
		}
		else
		{
			mpiargv[mpiargc] = argv[i];
			mpiargc++;
		}
	}
	numPoints = pow(10, ex);

	time(&tstart);

	MPI_Init(&mpiargc, &mpiargv);
	MPI_Comm_size(MPI_COMM_WORLD, &numTasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if( rank == 0 )
	{
		printf("Calculating 10 pow %d points ", ex);
		printf("with %d threads\n", numTasks);
		printf("started at %s", ctime(&tstart));
	}

	numPerThread = numPoints / numTasks;

	time(&t);
	srand((unsigned int) t - rank * 30);

	for( i = 0; i < numPerThread; i++ )
	{
		rndX = (long double) rand() / RAND_MAX;
		rndY = (long double) rand() / RAND_MAX;

		rndR = sqrt((pow(rndX, 2) + pow(rndY, 2)));
		if( rndR <= r )
		{
			nIn++;
		}
		privCount++;
		if( p != 0 && privCount % p == 0 )
		{
			time(&tnew);
			pi = nIn / (long double) privCount * 4.0;
			printf("Thread %d/%d: %Ld/%Ld in %2d:%02d: %.16Lf\n", rank, numTasks, nIn, privCount, (int) (difftime(tnew, t) / 60), (int) difftime(tnew, t) % 60, pi);
		}
	}
	MPI_Reduce(&nIn, &sumIn, 1, MPI_UNSIGNED_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Reduce(&privCount, &sumCount, 1, MPI_UNSIGNED_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

	MPI_Finalize();
	if( rank == 0 )
	{
		pi = sumIn / (long double) sumCount * 4.0;
		time(&tend);
		printf("\n%lld / %lld in %.0f sec: %.16Lf\n", sumIn, sumCount, difftime(tend, tstart), pi);
	}

	return 0;
}
