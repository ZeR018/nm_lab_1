#pragma once
#include<math.h>

#define MAX_ARR 100
#define KRAT_ARR 8

using namespace std;

double f(double x, double v,double a1, double a3, double m)
{
	return - (a1 * v + a3 * (v * v * v)) / m;
}


void record(ostream* f, double * d, int max)
{
	for (int i = 1; i <= max; i++)
	{
		if ((i - 1) % KRAT_ARR == 0)
			*f << "\n" << i / KRAT_ARR + 1 << ":\t";
		*f << d[i - 1] << "\t";
	}
}