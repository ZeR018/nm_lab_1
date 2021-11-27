#include "test_task.h"
#include<fstream>
#include <vector>

using namespace std;

enum { __x0, __u0, __h0, __a1, __a3, __m, __e, __max_step, __gran, __toch };
enum { _xu, __contr_e, __method };

// u(0) = u0
double f(double v)
{
	-3 / 2 * v;
}

struct perem {
	double x;
	double v;
	double s;
	double h;
	double u;
	double E;
	double c1;
	double c2;
};

int test_task(double* start_p, int* gran, char* name_txt, double** py) 
{
	perem p;
	p.x = start_p[__x0];
	p.v = start_p[__u0];
	p.s = 0.0;
	p.h = start_p[__h0];
	p.u = start_p[__u0];
	p.E = 0.0;
	p.c1 = 0.0;
	p.c2 = 0.0;

	double* k = new double[4];

	vector<double> d_v;


}