#include "system_task.h"
#include <vector>
#include"pch.h"

#define P 4

enum { __u0, __h0, __a, __b, __u20, __e, __max_step, __gran, __toch };
enum { _xu, __contr_e, __method };

double f_3_1(double v2)
{
	return v2;
}

double f_3_2(double v1, double v2, double a, double b)
{
	return -a * v2 * v2 - b * v1;
}

struct v_value
{
	double v1;
	double v2;
};

v_value st_RK4_S(double x, double v1, double v2, double h, double* start_p)
{
	double k1_1 = f_3_1(v2);
	double k2_1 = f_3_2(v1, v2, start_p[__a], start_p[__b]);
	double k1_2 = f_3_1(v2 + h / 2 * k2_1);
	double k2_2 = f_3_2(v1 + h / 2 * k1_1, v2 + h / 2 * k2_1, start_p[__a], start_p[__b]);
	double k1_3 = f_3_1(v2 + h / 2 * k2_2);
	double k2_3 = f_3_2(v1 + h / 2 * k1_2, v2 + h / 2 * k2_2, start_p[__a], start_p[__b]);
	double k1_4 = f_3_1(v2 + h * k2_3);
	double k2_4 = f_3_2(v1 + h * k1_3, v2 + h * k2_3, start_p[__a], start_p[__b]);

	v_value res;
	res.v1 = v1 + h / 6 * (k1_1 + 2 * k1_2 + 2 * k1_3 + k1_4);
	res.v2 = v2 + h / 6 * (k2_1 + 2 * k2_2 + 2 * k2_3 + k2_4);
}

struct perem {
	double x;
	double v11;
	double v12;
	double v21;
	double v22;
	double s;
	double h;
	double u;
	double E;
	double c1;
	double c2;
};

int systen_task(double* start_p, int* gran, const char* name_txt, double** py)
{
	perem a;
	a.x = 0.0;
	a.v11 = start_p[__u0];
	a.v12 = 0.0;
	a.v21 = start_p[__u20];
	a.v22 = 0.0;
	a.s = 0.0;
	a.h = start_p[__h0];
	a.c1 = 0.0;
	a.c2 = 0.0;

	std::vector<double> d_v;

	int end = 0;
	v_value temp1, temp2;
	double s_temp = 0.0;
	double control = true;

	d_v.push_back(a.x);   // x
	d_v.push_back(a.v11); // v
	d_v.push_back(a.v12); // v2
	d_v.push_back(0.0);   // Vi - V2i
	d_v.push_back(a.s);   // s
	d_v.push_back(0.0);   // h0
	d_v.push_back(a.c1);  // c1
	d_v.push_back(a.c2);  // c2

	for (int i = 0; ; i++)
	{
		//max step-------------------------------------
		if (i >= static_cast<int>(start_p[__max_step]))
		{
			break;
		}

		//gran x------------------------------------------------
		if (gran[_xu] == 0 && start_p[__gran] + start_p[__toch] < a.x)
		{
			i--;
			//j = 0;
			if (a.h < start_p[__toch])
			{
				break;
			}
			a.h /= 2;
			a.c1 += 1.0;
			continue;
		}
		//-----------------------------------------------------------

		temp1 = st_RK4_S(a.x, a.v11, a.v21, a.h, start_p);
		temp2 = st_RK4_S(a.x, a.v11, a.v21, a.h / 2, start_p);
		temp2 = st_RK4_S(a.x + a.h / 2, temp2.v1, temp2.v2, a.h / 2, start_p);

		s_temp = fabs((temp1.v1 - temp2.v1) / (pow(2, P) - 1));

		if (gran[__contr_e]) //c изминением шага или без
		{
			//условие, если рез функции зашел за наши параметры
			if (s_temp > start_p[__e])
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
				continue;
			}
		}

		a.x += a.h;

		a.v11 = temp1.v1;
		a.v12 = temp2.v1;
		a.v21 = temp1.v2;
		a.v22 = temp2.v2;

		d_v.push_back(a.x);   // x
		d_v.push_back(a.v11); // v
		d_v.push_back(a.v12); // v2
		d_v.push_back(a.v11 - a.v12);   // Vi - V2i
		d_v.push_back(a.s);   // s
		d_v.push_back(a.h);   // h
		d_v.push_back(a.c1);  // c1
		d_v.push_back(a.c2);  // c2
	}

	//собираем массив и кидаем в питон
	*py = new double[d_v.size()];
	std::memcpy(*py, d_v.data(), d_v.size() * sizeof d_v[0]);
	int size = d_v.size();

	return size;
}