#include "test_task.h"
#include<fstream>

#include <math.h>
#include"pch.h"
#include <vector>
#include <iostream>

using std::vector;

#define P 4

enum { __x0, __u0, __h0, __a1, __a3, __m, __e, __max_step, __gran, __toch };
enum { _xu, __contr_e, __method };

// u(0) = u0
double f_1(double x, double v)
{
	return (-3 / 2 * v);
}

double st_RK4(double x, double v, double h, double* start_p, double* k)
{
	k[0] = f_1(x, v);
	k[1] = f_1(x + h / 2, v + h / 2 * k[0]);
	k[2] = f_1(x + h / 2, v + h / 2 * k[1]);
	k[3] = f_1(x + h / 2, v + h * k[2]);

	return v + h / 6 * (k[0] + 2 * k[1] + 2 * k[2] + k[3]);
}

double st_true_sol_test(double x, double v, double v0)
{
	//return exp((-3 * x + 2 * log(v0)) / 2);
	return exp(-1.5 * x) * v0;
}

struct perem {
	double x;
	double v;
	double v2;
	double s;
	double h;
	double u;
	double E;
	double c1;
	double c2;
};

int test_task(double* start_p, int* gran, const char* name_txt, double** py)
{
	// Инициализируем переменные
	perem a;
	a.x = 0.0;
	a.v = start_p[__u0];
	a.v2 = 0.0;
	a.s = 0.0;
	a.h = start_p[__h0];
	a.u = start_p[__u0];
	a.E = 0.0;
	a.c1 = 0.0;
	a.c2 = 0.0;
	perem p;
	p.x = start_p[__x0];
	p.v = start_p[__u0];
	p.s = 0.0;
	p.h = start_p[__h0];
	p.u = start_p[__u0];
	p.E = 0.0;
	p.c1 = 0.0;
	p.c2 = 0.0;

	// Массив К для метода и вектор результатов
	double* k = new double[4];
	std::vector<double> d_v;

	int end = 0;
	double v_temp = 0.0;
	double s_temp = 0.0;
	double control = true;

	// Добавляем первую строку
	d_v.push_back(a.x);  // x
	d_v.push_back(a.v);  // v
	d_v.push_back(a.v2); // v2
	d_v.push_back(0.0);  // Vi - V2i
	d_v.push_back(a.s);  // s
	d_v.push_back(0.0);  // h0
	d_v.push_back(a.u);  // u
	d_v.push_back(a.E);  // E
	d_v.push_back(a.c1); // c1
	d_v.push_back(a.c2); // c2

	for (int i = 0; ; i++)
	{
		//max step-------------------------------------
		if (i >= static_cast<int>(start_p[__max_step]))
		{
			break;
		}

		// gran x
		if (gran[_xu] == 0)
		{

			if (a.x > start_p[__gran] - start_p[__toch])
			{
				break;

			}
			if (a.x + a.h > start_p[__gran] - start_p[__toch])
			{
				// Следующий X выходит за границу
				if (a.x + a.h > start_p[__gran]) {
					a.h /= 2;
					a.c1 += 1.0;
					i--;
					control = false;
					continue;
				}
				// Следующий Х выходит к границе с точностью
				else
				{
					end = 1;
				}
			}
		}

			

		// Вычисление
		v_temp = st_RK4(a.x, a.v, a.h, start_p, k);
		a.v2 = st_RK4(a.x, a.v, a.h/2, start_p, k);
		a.v2 = st_RK4(a.x + a.h / 2, a.v2, a.h/2, start_p, k);

		//gran u--------------------------------------------
		/*if (gran[_xu] && v_temp <= start_p[__toch] + start_p[__gran])
		{
			if (a.v < start_p[__gran] + start_p[__toch])
			{
				break;

			}
			if (v_temp <= start_p[__toch] + start_p[__gran])
			{
				if (v_temp <= start_p[__gran])
				{
					i--;
					control = false;
					a.h /= 2;
					a.c1 += 1.0;
					continue;
				}
				else
				{
					end = 1;
				}
			}
		}*/
		

		s_temp = fabs((a.v2 - v_temp) / (pow(2, P) - 1));

		if (control)
		{
			if (gran[__contr_e]) //c изминением шага или без
			{
				//условие, если рез функции зашел за наши параметры
				if (s_temp > start_p[__e]&& a.h>0.001)
				{
					i--;
					a.h /= 2;
					a.c1 += 1.0;
					continue;
				}
				
				if (s_temp < start_p[__e] / pow(2, P + 1))
				{
					a.h *= 2;
					a.c2 += 1.0;
				}
			}
		}

		a.x += a.h;

		a.v = v_temp;
		a.s = s_temp;
		a.u = st_true_sol_test(a.x, a.v, start_p[__u0]);
		a.E = fabs(a.u - a.v);
		d_v.push_back(a.x);  // x
		d_v.push_back(a.v);  // v
		d_v.push_back(a.v2); // v2
		d_v.push_back(fabs(a.v - a.v2)); 
		d_v.push_back(a.s);  // s
		d_v.push_back(a.h);  // h
		d_v.push_back(a.u);  // u
		d_v.push_back(a.E);  // E
		d_v.push_back(a.c1); // c1
		d_v.push_back(a.c2); // c2
		a.c1 = 0;
		a.c2 = 0;

		if (end)
		{
			break;
		}
	}

	//собираем массив и кидаем в питон
	*py = new double[d_v.size()];
	std::memcpy(*py, d_v.data(), d_v.size() * sizeof d_v[0]);
	int size = d_v.size();

	return size;
}