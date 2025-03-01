#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void diff(double *u, int N, double dx, double *du) {
	int i;
	du[0] = (u[1] - u[N-1]) / dx / 2;
	for (i = 1; i < N-1; i++)
		du[i] = (u[i+1] - u[i-1]) / dx / 2;
	du[N-1] = (u[0] - u[N-2]) / dx / 2;
}

void init(double *u, int N, double dx) {
	int i;
	for (i = 0; i < N; i++)
		u[i] = sin(i * dx);
}

int main(int argc, char** argv) {
	int N = atoi(argv[1]);
	int M = atoi(argv[2]);

	double *u = malloc(N * sizeof *u);
	double *du = malloc(N * sizeof *du);
	double dx = 2. * M_PI / N;

	init(u, N, dx);

	/* clock() function */
	int i;
	double times[M];
	double total = 0;
	for (i = 0; i < M; i++) {
		clock_t begin1 = clock();

		diff(u, N, dx, du);

		clock_t end1 = clock();
		times[i] = ((double)(end1 - begin1)) / CLOCKS_PER_SEC;
		total += times[i];
	}

	double mean = total / M;
	double var = 0;
	for (i = 0; i < M; i++)
		var += (times[i] - mean) * (times[i] - mean);
	printf("mean elapsed time = %e, variance = %e\n", mean, var/(M-1));

	/* time() function */
	total = 0;
	for (i = 0; i < M; i++) {
		time_t begin2 = time(NULL);

		diff(u, N, dx, du);

		time_t end2 = time(NULL);
		times[i] = difftime(end2, begin2);
		total += times[i];
	}

	mean = total / M;
	var = 0;
	for (i = 0; i < M; i++)
		var += (times[i] - mean) * (times[i] - mean);
	printf("mean elapsed time = %e, variance = %e\n", mean, var/(M-1));

	/* clock_gettime() function */
	total = 0;
	for (i = 0; i < M; i++) {
		struct timespec begin3;
		clock_gettime(CLOCK_MONOTONIC, &begin3);

		diff(u, N, dx, du);

		struct timespec end3;
		clock_gettime(CLOCK_MONOTONIC, &end3);
		times[i] = (end3.tv_sec + end3.tv_nsec / 1.0e9) - (begin3.tv_sec + begin3.tv_nsec / 1.0e9);
		total += times[i];
	}

	mean = total / M;
	var = 0;
	for (i = 0; i < M; i++)
		var += (times[i] - mean) * (times[i] - mean);
	printf("mean elapsed time = %e, variance = %e\n", mean, var/(M-1));
	
	/* gettimeofday() function */
	total = 0;
	for (i = 0; i < M; i++) {
		struct timeval begin4;
		gettimeofday(&begin4, NULL);

		diff(u, N, dx, du);

		struct timeval end4;
		gettimeofday(&end4, NULL);
		times[i] = (end4.tv_sec + end4.tv_usec / 1.0e6) - (begin4.tv_sec + begin4.tv_usec / 1.0e6);
		total += times[i];
	}

	mean = total / M;
	var = 0;
	for (i = 0; i < M; i++)
		var += (times[i] - mean) * (times[i] - mean);
	printf("mean elapsed time = %e, variance = %e\n", mean, var/(M-1));

	free(u);
	free(du);

	return 0;
}
