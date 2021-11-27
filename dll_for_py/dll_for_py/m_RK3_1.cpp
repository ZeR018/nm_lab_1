#include "stdafx.h"
#include"m_RK3_1.h"
#include"fun.h"
#include<fstream>


//данные предоставляют сплошной массив, поэтому для удобства я сделал набор типо понятных символов
//например чтобы обратиться к v1 элементу массива(по сути 2-й по счету) надо написать perem[__v1]
//																что равносильно perem[1]						
enum { __x, __v1, __v2, __s, __h1, __h2, __h3, __u, __E, __c1, __c2 };
enum { __x0, __u0, __h0, __a1, __a3, __m, __e, __max_step, __gran, __toch };
enum { _xu, __contr_e };

#define EPS 0.01
#define P 3
#define P_SIZE 11



// j здесь для сдвига массива по __h, опять же для памяти и быстродействия
double st_RK_1(double* perem, double* start_p, double* k, int j)
{
	//_h = h / 2;
	perem[__h2 + j] = perem[__h1 + j] / 2;
	//k[0] = f(x[0], v1[0]);
	k[0] = f(perem[__x], perem[__v1], start_p[__a1], start_p[__a3], start_p[__m]);
	//k[1] = f(h / 2 + x[0], _h[0] *k[0] + v1);
	k[1] = f(perem[__h1 + j] / 2 + perem[__x], perem[__h2 + j] * k[0] + perem[__v1], start_p[__a1], start_p[__a3], start_p[__m]);
	//k[2] = f(x + h, (-k + 2 * k)*h + v1);
	k[2] = f(perem[__x] + perem[__h1 + j], (-k[0] + 2 * k[1]) * perem[__h1 + j] + perem[__v1], start_p[__a1], start_p[__a3], start_p[__m]);

	if (j)
	{
		double tmp = (k[0] + 4 * k[1] + k[2]) / 6 * perem[__h1 + j] + perem[__v1];

		k[0] = f(perem[__x], perem[__v1], start_p[__a1], start_p[__a3], start_p[__m]);
		//k[1] = f(h / 2 + x[0], _h[0] *k[0] + v1);
		k[1] = f(perem[__h1 + j] / 2 + perem[__x], perem[__h2 + j] * k[0] + perem[__v1], start_p[__a1], start_p[__a3], start_p[__m]);
		//k[2] = f(x + h, (-k + 2 * k)*h + v1);
		k[2] = f(perem[__x] + perem[__h1 + j], (-k[0] + 2 * k[1]) * perem[__h1 + j] + perem[__v1], start_p[__a1], start_p[__a3], start_p[__m]);
	}

	//return (k[0] + 4 * k[1] + k[2]) / 6 *(*h) + (*v1);
	return (k[0] + 4 * k[1] + k[2]) / 6 * perem[__h1 + j] + perem[__v1];
}

double st_RK(double (*f)(double, double, double, double, double), double x, double v, double h, double* start_p, double* k)
{
	k[0] = f(x, v, start_p[__a1], start_p[__a3], start_p[__m]);
	k[1] = f(x + h / 2, v + h * k[0] / 2, start_p[__a1], start_p[__a3], start_p[__m]);
	k[2] = f(x + h / 2, v + h * (-k[0] + 2 * k[1]), start_p[__a1], start_p[__a3], start_p[__m]);
	return (k[0] + 4 * k[1] + k[2]) / 6 * h + v;

}

// Истинное решение задачи 9 в точке perem[__x] при начальных условиях u(x0)=u0
double st_true_sol_ex_9(double* perem, double* start_p)
{

	double c = log(pow(start_p[__u0], 2) / (start_p[__a1] * pow(start_p[__u0], 2) + start_p[__a3])) + (2 * start_p[__a3] * start_p[__x0]) / start_p[__m];

	double _exp = exp((-2 * start_p[__a3] * perem[__x]) / start_p[__m] + c);

	return sqrt((_exp * start_p[__a3]) / (1 - _exp * start_p[__a1]));
}


int m_RK3_1_r(double* start_p, int* gran, char* name_txt, double** py)
{
	double v_temp = 0.0;
	double v2 = 0.0;
	double s_temp = 0.0;
	//------------------x---v1---e---h
	double perem[P_SIZE] = {};
	double k[3] = {};
	int z = 0;
	double tmp = 0.0;
	vector<double> d_v;
	double C[2] = {};

	string name = string(name_txt);
	ofstream _f(name);


	//инициализация массива
	perem[__x] = start_p[__x0];
	perem[__v1] = start_p[__u0];
	perem[__v2] = start_p[__u0];
	perem[__s] = 0.0;
	perem[__h1] = 0.0;
	perem[__h2] = 0.0;
	perem[__h3] = 0.0;
	perem[__u] = start_p[__u0];
	perem[__E] = 0.0;
	perem[__c1] = 0.0;
	perem[__c2] = 0.0;

	//добавление в вектор 1-х значений
	d_v.push_back(perem[__x]);
	d_v.push_back(perem[__v1]);
	d_v.push_back(perem[__v2]);
	d_v.push_back(perem[__s]);
	d_v.push_back(0.0);
	d_v.push_back(perem[__u]);
	d_v.push_back(perem[__E]);
	d_v.push_back(perem[__c1]);
	d_v.push_back(perem[__c2]);


	perem[__h1] = start_p[__h0];


	int j = 1;
	int y = 0;

	for (int i = 0; ; i++)
	{
		//max step-------------------------------------
		if (i >= static_cast<int>(start_p[__max_step]))
		{
			break;
		}


		if (z)
		{
			perem[__h1] = perem[__h1] * 2;
			perem[__c2] += 1.0;
		}

		z = 0;

		//увеличиваем x
		perem[__x] += perem[__h1];

		//gran x------------------------------------------------
		if (gran[_xu] == 0 && start_p[__gran] + start_p[__toch] < perem[__x])
		{
			perem[__x] -= perem[__h1];
			i--;
			j = 0;
			perem[__h1] /= 2;
			if (perem[__h1] < start_p[__toch])
			{
				break;
			}
			perem[__c1] += 1.0;
			continue;
		}
		//-----------------------------------------------------------


		//вычисление 
		//v_temp = st_RK_1(perem,start_p, k, 0);
		//v2 = st_RK_1(perem,start_p, k, 1);

		v_temp = st_RK(f, perem[__x], perem[__v1], perem[__h1], start_p, k);

		v2 = st_RK(f, perem[__x], perem[__v1], perem[__h1] / 2, start_p, k);
		v2 = st_RK(f, perem[__x] + perem[__h1] / 2, v2, perem[__h1] / 2, start_p, k);

		//gran u--------------------------------------------
		if (gran[_xu] && v_temp <= start_p[__toch] + start_p[__gran])
		{
			if (v_temp <= start_p[__gran])
			{
				perem[__x] -= perem[__h1];
				i--;
				j = 0;
				perem[__h1] /= 2;
				perem[__c1] += 1.0;
				continue;
			}
			else
			{
				y = 1;
			}
		}



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
					perem[__x] -= perem[__h1];
					perem[__h1] = perem[__h1] / 2;
					perem[__c1] += 1.0;
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
		perem[__v1] = v_temp;
		perem[__v2] = v2;
		perem[__s] = s_temp * pow(2, P);

		perem[__u] = st_true_sol_ex_9(perem, start_p);
		//std::cout << perem[__u] << "\n";
		perem[__E] = fabs(perem[__u] - perem[__v1]);



		//кидаем в вектор то что нужно
		d_v.push_back(perem[__x]);
		d_v.push_back(perem[__v1]);
		d_v.push_back(perem[__v2]);
		d_v.push_back(perem[__s]);
		d_v.push_back(perem[__h1]);
		d_v.push_back(perem[__u]);
		d_v.push_back(perem[__E]);
		d_v.push_back(perem[__c1]);
		d_v.push_back(perem[__c2]);
		perem[__c1] = 0.0;
		perem[__c2] = 0.0;

		if (y) {
			break;
		}

	}

	//собираем массив и кидаем в питон
	*py = new double[d_v.size()];
	std::memcpy(*py, d_v.data(), d_v.size() * sizeof d_v[0]);
	int size = d_v.size();

	//запись в файл
	record(&_f, *py, size);

	//возвращаем размер массива 
	return size;
}
