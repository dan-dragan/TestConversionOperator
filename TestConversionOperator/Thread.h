#pragma once
#include "Runnable.h"
class Thread :
	public Runnable
{
	RunnablePtr m_sptrTarget;
public:
	Thread(const Thread& other);
	Thread(Thread&& other) noexcept;
	Thread();
	Thread(RunnablePtr&target);
	Thread& operator=(const Thread& other);
public:
	virtual ~Thread();
	int Start();
	operator RunnablePtr() {
		return RunnablePtr(this);
	}
	void Suspend();
	bool Resume();
	void Terminate(DWORD exitCode = 1);
	bool Wait(DWORD timeout = INFINITE);
protected:
	virtual int Run();
	virtual bool InitializeInThread();
	virtual void UninitializeInThread();
private:
	volatile bool		m_running;
	HANDLE				m_handle;
	std::string		m_name;

	static unsigned int __stdcall _threadp(void *parg);
};
typedef std::shared_ptr<Thread> ThreadPtr;

