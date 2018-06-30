// Base Develop.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Thread.h"

class Test : public Thread
{
public:
	Test()
	{
		_threadstoprequest = false;

		SetThreadName(std::string("Test Thread"));
	}
	~Test()
	{

	}

private:
	virtual void DoWork()
	{
		int TestVariable = 0;
		while (!_threadstoprequest)
		{
			std::cout << "Thread ID : " << GetCurrentThreadId() << "Independent Variable :" << TestVariable << std::endl;
			TestVariable++;

			Sleep(1000);
		}
	}
	virtual void EmitWakeupSignal()
	{
		_threadstoprequest = true;
	}

private:
	bool _threadstoprequest;
};

int main()
{
	Test t, t2;

	t.Start();
	//t2.Start();

	while(1)
	{ }


    return 0;
}

