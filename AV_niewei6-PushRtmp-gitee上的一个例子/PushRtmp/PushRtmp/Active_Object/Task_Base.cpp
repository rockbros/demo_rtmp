#include "Task_Base.h"
#include <process.h>
#include "CriticalSection.h"
#include "EventImpl.h"
Task_Base::Task_Base(void)
{
	m_poCriSection	= new CCriticalSection;
	m_p_event		=new CEventImpl;
}

Task_Base::~Task_Base(void)
{
	wait();
	delete m_poCriSection;
	delete m_p_event;
}

unsigned __stdcall Task_Base::svc_run( void *apParam)
{
	return ((Task_Base *)apParam)->svc();
}

int Task_Base::activate( int n_threads /*= 1*/ )
{
	unsigned threadID;

	while(n_threads)
	{
		HANDLE hThreadRecv		= (HANDLE)_beginthreadex( NULL, 0, &Task_Base::svc_run, (LPVOID)this, 0, &threadID );//接受网络数据

		h_threads.push((int)hThreadRecv);

		n_threads --;
	}
	return TRUE;
}

int	Task_Base::thr_count (void) const
{
	return h_threads.size();
}

int Task_Base::putq (const std::string &str)
{
	{
		CCriticalAutoLock loAutoLock(*m_poCriSection);
		msg_queue_.push(str);
	}	

	m_p_event->set_event();	//设置活动事件

	return TRUE;
}

int Task_Base::getq (std::string &str)
{
	do
	{
		{
			CCriticalAutoLock loAutoLock(*m_poCriSection);

			if(!msg_queue_.empty())
			{
				str = msg_queue_.front();
				msg_queue_.pop();
				return true;
			}
		}

		m_p_event->reset_event();//队列为空了 不能活动 阻塞
		m_p_event->wait_for_single_object();
	}
	while(true);//msg_queue_.empty()
	return TRUE;
}

void Task_Base::wait()
{
	std::queue<int>  temp = h_threads;
	if(!temp.empty())
	{
		HANDLE *pHandleArray = new HANDLE[temp.size()];
		HANDLE *pHandle = pHandleArray;
		while(!temp.empty())
		{
			*pHandle = (HANDLE)temp.front();
			pHandle ++;
			temp.pop();
		}	
		WaitForMultipleObjects(h_threads.size(),pHandleArray,TRUE,INFINITE); //无限等待可以用infinite

		for (int i =0 ;i<h_threads.size();i++)
		{
			CloseHandle(pHandleArray[i]);	
		}
		delete []pHandleArray;

		while(!h_threads.empty())
			h_threads.pop();
	}
}
