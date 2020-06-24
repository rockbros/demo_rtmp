#pragma once

class CEventImpl
{
public:
	CEventImpl(void);

	virtual void set_event();
	virtual void reset_event();
	virtual void wait_for_single_object();

public:
	virtual ~CEventImpl(void);
private:
	void *  m_hEventHandle;
};
