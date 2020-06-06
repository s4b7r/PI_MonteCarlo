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

int main( int argc, char *argv[] )
{

	time_t t;
	int i;
	int sp = 5;
	long double j;
	int n = 0;
	int ex = 6;
	int nIn = 0;
	int r = 1;
	long double pi = 0;

	if( argc == 2 )
	{
		ex = atol(argv[1]);
	}
	n = pow(10, ex);
	printf("10 pow %d times ", ex);

	time(&t);
	srand((unsigned int) t);

	for( i = 0; i < n; i++ )
	{
		long double rndX = (long double) rand() / RAND_MAX;
		long double rndY = (long double) rand() / RAND_MAX;

		long double rndR = sqrt((pow(rndX, 2) + pow(rndY, 2)));
		if( rndR <= r )
		{
			nIn++;
		}

		if( i % (n / (100 / sp)) == 0 )
		{
			j = i;
			pi = nIn / j * 4.0;
			printf("%3d %% : %.16Lf \n", (int) (j / n * 100), pi);
		}
	}
	pi = nIn / (long double) n * 4.0;
	printf("\n%d / %d : %.16Lf", nIn, n, pi);

	return 0;
}
