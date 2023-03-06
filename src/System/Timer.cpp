#include "Timer.h"

Timer::Timer()
	: m_isRunning(false)
	, m_timerLeft(0)
{

}

void Timer::update(const double delta)
{
	if (m_isRunning)
	{
		m_timerLeft -= delta; 
		if (m_timerLeft <= 0)
		{
			m_isRunning = false;
			m_callBack();
		}
	}
}

void Timer::start(const double duration)
{
	m_timerLeft = duration;
	m_isRunning = true;
}

void Timer::setCallBack(std::function<void()> callBack)
{
	m_callBack = callBack;
}
