#include <stdafx.h>
#include <Tools/FPSCounter.h>
#include <windows.h>

Engine::Tools::FPSCounter::FPSCounter(int p_numberOfSamples) : m_numberOfSamples{ p_numberOfSamples } {}

void Engine::Tools::FPSCounter::Start()
{
    /*LARGE_INTEGER frequencyCount;
    QueryPerformanceFrequency(&frequencyCount);

    countsPerSecond = double(frequencyCount.QuadPart);

    QueryPerformanceCounter(&frequencyCount);
    CounterStart = frequencyCount.QuadPart;*/
    m_startTime = std::chrono::system_clock::now();
    m_updateFrameTime = std::chrono::system_clock::now();
}

void Engine::Tools::FPSCounter::Stop()
{
    m_endTime = std::chrono::system_clock::now();
    m_previousTimes.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(m_endTime - m_startTime).count());

    if (m_previousTimes.size() > m_numberOfSamples)
    {
        m_previousTimes.pop_front();
    }

    m_updateFrameTime = std::chrono::system_clock::now();
    const std::chrono::duration<float> time = m_updateFrameTime - m_lastUpdateFrameTime;

    if (time.count() >= 1)
    {
        m_lastUpdateFrameTime = std::chrono::system_clock::now();

        float totalTime = 0.0f;

        for (auto time : m_previousTimes)
        {
            totalTime += time;
        }

        m_frameTime = totalTime / m_previousTimes.size();
    }
}

int Engine::Tools::FPSCounter::GetFPS() const
{
    return 1.0f / m_frameTime * 1000.0f;
}
