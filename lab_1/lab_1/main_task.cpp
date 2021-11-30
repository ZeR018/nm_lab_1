#include"main_task.h"
#include"fun.h"
#include<fstream>
#include"pch.h"
#include <vector>
#include <iostream>

using namespace std;

enum { __x0, __u0, __h0, __a1, __a3, __m, __e, __max_step, __gran, __toch };
enum { _xu, __contr_e, __method };


#define P 4

struct perem {
	double x;
	double v;
	double s;
	double h;
	double c1;
	double c2;
};

double f_2(double x, double v)
{
	double a = pow(1 + x * x, 1 / 3);
	//std::cout << sin(10) << "\t" << x << "\n";
	return (v * v) / a + v - v * v * v * sin(572*x);
}

double st_RK(double x, double v, double h, double* start_p, double* k)
{
	k[0] = f_2(x, v);
	k[1] = f_2(x + h / 2, v + h  * k[0] / 2);
	k[2] = f_2(x + h / 2, v + h  * k[1]/2);
	k[3] = f_2(x + h, v + h * k[2]);

	return v + h  * (k[0] + 2 * k[1] + 2 * k[2] + k[3]) / 6;
}

int main_task(double* start_p, int* gran, const char* name_txt, double** py)
{
	perem p;
	p.x = start_p[__x0];
	p.v = start_p[__u0];
	p.s = 0.0;
	p.h = start_p[__h0];
	p.c1 = 0.0;
	p.c2 = 0.0;


	double* k = new double[3];

	vector<double> d_v;

	d_v.push_back(p.x); //x
	d_v.push_back(p.v); //v1
	d_v.push_back(p.v); //v2
	d_v.push_back(0.0);  // Vi - V2i
	d_v.push_back(p.s); //s
	d_v.push_back(0.0);  // h0
	d_v.push_back(0.0);  // u
	d_v.push_back(0.0);  // E
	d_v.push_back(p.c1);//c1
	d_v.push_back(p.c2);//c2

	int j = 1;
	int y = 0;
	int z = 0;
	double v_temp = 0.0;
	double v2 = 0.0;
	double s_temp = 0.0;


	//string name = string(name_txt);
	//ofstream _f(name);


	for (int i = 0; ; i++)
	{
		
		//max step-------------------------------------
		if (i >= static_cast<int>(start_p[__max_step]))
		{
			break;
		}


		if (z)
		{
			p.h = p.h * 2;
			p.c2 += 1.0;
		}

		z = 0;


		if (p.v > start_p[__gran])
		{
			break;
		}

		//gran x------------------------------------------------
		/*if (gran[_xu] == 0 && start_p[__gran] + start_p[__toch] < p.x)
		{
			p.x -= p.h;
			i--;
			j = 0;
			p.h /= 2;
			if (p.h < start_p[__toch])
			{
				break;
			}
			p.c1 += 1.0;
			continue;
		}*/
		//-----------------------------------------------------------


		//вычисление 
		//v_temp = st_RK_1(perem,start_p, k, 0);
		//v2 = st_RK_1(perem,start_p, k, 1);

		v_temp = st_RK(p.x, p.v, p.h, start_p, k);

		v2 = st_RK(p.x, p.v, p.h / 2, start_p, k);
		v2 = st_RK(p.x + p.h / 2, v2, p.h / 2, start_p, k);

		//gran u--------------------------------------------
		/*if (gran[_xu] && v_temp <= start_p[__toch] + start_p[__gran])
		{
			if (v_temp <= start_p[__gran])
			{
				p.x -= p.h;
				i--;
				j = 0;
				p.h /= 2;
				p.c1 += 1.0;
				continue;
			}
			else
			{
				y = 1;
			}
		}*/



		s_temp = fabs((v2 - v_temp) / (pow(2, P) - 1));
		//std::cout << v2<<"   " << v_temp << "\n";

		if (j)
		{
			if (gran[__contr_e]) //c изминением шага или без
			{
				//условие, если рез функции зашел за наши параметры
				if (s_temp > start_p[__e])
				{
					i--;
					//p.x -= p.h;
					p.h = p.h / 2;
					p.c1 += 1.0;
					continue;
				}

				if (s_temp < start_p[__e] / pow(2, P + 1))
				{
					z = 1;
				}
			}
		}


		//----------------------------------------------------------------------




		//пихаем значения и погрешность
		p.v = v_temp;
		p.s = s_temp * pow(2, P);


		//увеличиваем x
		p.x += p.h;


		//кидаем в вектор то что нужно
		d_v.push_back(p.x);
		d_v.push_back(p.v); //v1
		d_v.push_back(v2); //v2
		d_v.push_back(fabs(p.v-v2)); //v2-v1
		d_v.push_back(p.s);
		d_v.push_back(p.h);
		d_v.push_back(0.0);
		d_v.push_back(0.0);
		d_v.push_back(p.c1);
		d_v.push_back(p.c2);
		p.c1 = 0.0;
		p.c2 = 0.0;

		if (y) {
			break;
		}

	}
	
	*py = new double[d_v.size()];
	std::memcpy(*py, d_v.data(), d_v.size() * sizeof d_v[0]);
	int size = d_v.size();
	return size;

}