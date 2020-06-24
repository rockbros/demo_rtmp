#include "EventImpl.h"
#include <wtypes.h>

CEventImpl::CEventImpl(void)
{
    (m_hEventHandle) = CreateEvent(NULL, TRUE,FALSE , NULL);
}

CEventImpl::~CEventImpl(void)
{
    CloseHandle(((HANDLE)m_hEventHandle));
    (m_hEventHandle) = NULL;
}

void CEventImpl::set_event()
{
	::SetEvent(((HANDLE)m_hEventHandle));
}

void CEventImpl::reset_event()
{
	::ResetEvent(((HANDLE)m_hEventHandle));
}

void CEventImpl::wait_for_single_object()
{
	WaitForSingleObject(((HANDLE)m_hEventHandle),INFINITE); 
}
