#pragma once

#include <functional>

class Timer
{
public:
	Timer();
	void update(const double delta);
	void start(const double duration);
	void setCallBack(std::function<void()> callBack);

private:
	std::function<void()> m_callBack;
	double m_timerLeft;
	bool m_isRunning;
};