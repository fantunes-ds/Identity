#pragma once
#include <Export.h>
#include <chrono>
#include <queue>

namespace Engine::Tools
{
    class API_ENGINE FPSCounter
    {
    public:
        FPSCounter(int p_numberOfSamples);

        void Start();
        void Stop();

        int GetFPS() const;
        float GetDeltaTime() const { return m_deltaTime / 1000.f; }

    private:
        int m_numberOfSamples;
        float m_deltaTime;
        float m_FPS;
        std::chrono::time_point<std::chrono::system_clock> m_startTime;
        std::chrono::time_point<std::chrono::system_clock> m_endTime;

        std::chrono::time_point<std::chrono::system_clock> m_updateFrameTime;
        std::chrono::time_point<std::chrono::system_clock> m_lastUpdateFrameTime;
        std::deque<float> m_previousTimes;
    };
}
