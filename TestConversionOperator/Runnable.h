#pragma once
class Runnable;
typedef std::shared_ptr<Runnable> RunnablePtr;
class Runnable
{
public:
	Runnable(const Runnable& other);
	Runnable(Runnable&&);
	Runnable();
	virtual ~Runnable();
	virtual operator RunnablePtr() = 0;
	virtual int Run() = 0;
};

