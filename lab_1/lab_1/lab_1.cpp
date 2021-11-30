#include"pch.h"
#include "test_task.h"
#include "main_task.h"
#include "system_task.h"
#include <iostream>

using namespace std;


extern "C" __declspec(dllexport) void __stdcall work_RK31R(double** data, double* start, int* gran, int* _i)
{

	//	*_i = m_RK3_1_r(start, gran, "test2.txt", data);
	if (*_i == 0)
	{
		*_i = test_task(start, gran, "test2.txt", data);
	}
	else if (*_i == 1)
	{
		*_i = main_task(start, gran, "test2.txt", data);
	}
	else if (*_i == 2)
	{
		*_i = systen_task(start, gran, "test2.txt", data);
	}


	
	//std::cout << *_i << "\n";
}

extern "C" __declspec(dllexport) void __stdcall del_mem(double** data)
{
	delete[](*data);
	//cout << "- mem" << endl;
}