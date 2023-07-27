#ifndef TIMER_H

#define TIMER_H

#include <iostream>
#include <chrono>

class Timer
{
public:
	Timer() 
		: m_Start(std::chrono::high_resolution_clock::now()) 
	{}
	~Timer()
	{
		m_End = std::chrono::high_resolution_clock::now();
		m_Duration = m_End - m_Start;

		std::cout << "Time elapsed in seconds: " << m_Duration.count() << "!";
	}
private:
	std::chrono::time_point<std::chrono::steady_clock> m_Start, m_End;
	std::chrono::duration<double> m_Duration{};

};

#endif // !TIMER.H