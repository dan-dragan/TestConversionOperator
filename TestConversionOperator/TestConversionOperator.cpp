// TestConversionOperator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Thread.h"
#include <conio.h>
class ThreadedDo : public Runnable
{
private:
	volatile bool m_bOver;
public:
	ThreadedDo() :m_bOver(false)
	{

	}
	operator RunnablePtr() {
		return RunnablePtr(this);
	}
	int Run() {
		do
		{
			printf("ThreadedDo!\n");
			Sleep(1000);
		} while (!m_bOver);
		return 0;
	}
	void End()
	{
		m_bOver = true;
	}
};
int main()
{
	std::shared_ptr<ThreadedDo> sptrTDo = std::make_shared<ThreadedDo>();
	ThreadPtr sptrThread = std::make_shared<Thread>((RunnablePtr)sptrTDo);
	sptrThread->Start();
	printf("Press any key to end threaded do");
	_getch();
	sptrTDo->End();
    return 0;
}

