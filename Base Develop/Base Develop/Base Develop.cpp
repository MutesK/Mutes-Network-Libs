// Base Develop.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

/*
	This Project is Only Test And Experience C++ MultiThread Programming Skils

*/
#include "stdafx.h"
#include "Thread.h"

#include "ObjectPool.h"
#include "ObjectPoolTLS.h" 
#include <thread>

struct Test
{
	int a; 

	Test()
	{

	}
};

CObjectPoolTLS<Test> TestPool(1000, 10000, true);

int main()
{


	auto function = []()
	{
		vector<Test *>	   TempStore;
		uint64_t			TestCount = 0;

		while (1)
		{
			std::cout << "#1. Alloc Test\n";
			for (int i = 0; i < 50000; i++)
			{
				auto variable = TestPool.Alloc();
				if (variable == nullptr)
				{
					*((int *)0) = 1;
				}

				variable->a = i;
				TempStore.push_back(variable);
			}

			std::cout << "#2. Free Test\n";
			for (auto ptr : TempStore)
			{
				if (TestPool.Free(ptr) == false)
				{
					*((int *)0) = 1;
				}
			}

			TempStore.clear();
			std::cout << "ObjectPool Test Complete #" << TestCount << endl;
			++TestCount;
		}
	};

	thread th1(function);
	//thread th2(function);
	//thread th3(function);
	//thread th4(function);
	//thread th5(function);
	//thread th6(function);
	//thread th7(function);
	//thread th8(function);

	th1.joinable();
	/*th2.joinable();
	th3.joinable();
	th4.joinable();
	th5.joinable();
	th6.joinable();
	th7.joinable();
	th8.joinable();*/

	while(1)
	{ }

    return 0;
}

