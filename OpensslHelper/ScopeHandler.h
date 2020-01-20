#pragma once

#include <functional>

class CScopeHandler
{
public:
	CScopeHandler(std::function<void()>  initor, std::function<void()>  uninitor) :
		m_initor(initor),
		m_uninitor(uninitor)
	{
		if (m_initor)
		{
			m_initor();
		}
	}

	~CScopeHandler()
	{
		if (m_uninitor)
		{
			m_uninitor();
		}
	}

private:
	std::function<void()>  m_initor;
	std::function<void()>  m_uninitor;
};