#include "stdafx.h"
#include "Thread.h"
#include <windef.h>
#include <process.h>
Thread::Thread():m_sptrTarget(nullptr), m_running(false)
{
}


Thread::Thread(const Thread& other)
{
	m_sptrTarget = other.m_sptrTarget;
	m_running = other.m_running;
}


Thread::Thread(Thread&& other ) noexcept : m_sptrTarget(std::move(other.m_sptrTarget))
{
	m_running = other.m_running;
}


Thread& Thread::operator=(const Thread& other)
{
	if (&other == this)
		return *this;
	m_sptrTarget = other.m_sptrTarget;
	m_running = other.m_running;
	return *this;
}

int Thread::Run() {
	if (m_sptrTarget) {
		return m_sptrTarget->Run();
	}
	return -1;
}

bool Thread::InitializeInThread() {
	return true;
}

void Thread::UninitializeInThread() {
}

unsigned int __stdcall Thread::_threadp(void *parg) {
	std::shared_ptr<Thread> theThread(reinterpret_cast<Thread*>(parg));

	if (theThread) 
	{
		try {

			if (theThread->InitializeInThread()) 
			{
				theThread->Run();
				theThread->UninitializeInThread();
			}
			theThread->m_running = false;
		}
		catch(...)
		{
			throw;
		}
	}
	return 0;
}

Thread::~Thread()
{
}

Thread::Thread(RunnablePtr target): m_running(false)
{
	m_sptrTarget = target;
}
int Thread::Start() {
	if (!m_running) {
		unsigned int	tid;
		m_handle=((HANDLE)::_beginthreadex(NULL, 0, _threadp, this, CREATE_SUSPENDED, &tid));
		if (m_handle)
		{
			m_running = true;
			Resume();
		}
		else {
			throw std::exception("Thread::Start");
		}
	}
	return m_running;
}
void Thread::Suspend() {
	if (::SuspendThread(m_handle) == (DWORD)-1) {
		throw std::exception("Thread::Suspend");
	}
}

bool Thread::Resume() {
	DWORD prevCount = ::ResumeThread(m_handle);
	if (prevCount == (DWORD)-1) {
		throw std::exception("Thread::Resume");
	}
	return prevCount == 1;
}

void Thread::Terminate(DWORD exitCode) {
	if (!::TerminateThread(m_handle, exitCode)) {
		throw std::exception("Thread::Terminate");
		//	OK this kind of sucks, if it was running then we
		//	need to Set m_running to false and also make sure
		//	the reference count decrements.  If not m_running
		//	then the Thread as in the middle of doing this when
		//	we cashed it and I can't do anything, I don't know if I
		//	released or not (although if I did I may not even be here
		//	anymore.
		if (m_running) {
			m_running = false;

		}
	}
}


