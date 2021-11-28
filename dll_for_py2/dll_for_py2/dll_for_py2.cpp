#include"pch.h"

using namespace std;


extern "C" __declspec(dllexport) void __stdcall work_RK31R(double** data, double* start, int* gran, int* _i)
{

//	*_i = m_RK3_1_r(start, gran, "test2.txt", data);

}

extern "C" __declspec(dllexport) void __stdcall del_mem(double** data)
{
	delete[](*data);
	//cout << "- mem" << endl;
}