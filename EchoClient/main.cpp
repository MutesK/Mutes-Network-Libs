#include "ClientApplication.h"
#include <chrono>

int main()
{
	ClientApplication Application;

	if (!Application.Open())
		return false;


	while (true)
	{
		Application.Monitoring();

		std::this_thread::sleep_for(2s);
	}

	return 0;
}