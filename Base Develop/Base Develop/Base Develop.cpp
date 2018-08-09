// Base Develop.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

/*
	This Project is Only Test And Experience C++ MultiThread Programming Skils

*/
#include "stdafx.h"
#include "Thread.h"

#include "ObjectPool.h"
#include "MemoryPoolTLS.h"
#include <thread>

struct Test
{
	int a;
};

int main()
{
	CObjectPoolTLS<Test> TestPool(10000, 10000, true);
	vector<Test *>	   TempStore;
	uint64_t			TestCount = 0;


	while (1)
	{
		std::cout << "#1. Alloc Test\n";
		for (int i = 0; i < 10000; i++)
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
	




    return 0;
}

