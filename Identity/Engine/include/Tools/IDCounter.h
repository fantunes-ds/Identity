#pragma once
#include <Export.h>
#include <cstdint>

namespace Engine::Tools
{
    /***
     * @brief Class used to keep a tally of assigned IDs and to generate new IDs.
     */
    class IDCounter
    {
    public:
        IDCounter() = delete;
        ~IDCounter() = delete;

        IDCounter(IDCounter& p_other) = delete;
        static inline uint32_t GetNewID()
        {
            uint32_t tmp = m_id;
            ++m_id;
            return tmp;
        }

    private:
        inline static uint32_t m_id = 0;
    };
}
