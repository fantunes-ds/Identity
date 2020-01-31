#pragma once

namespace Engine
{
    class IEventCallback
    {
    public:
        virtual bool operator == (IEventCallback* p_other) = 0;

        virtual void operator() () = 0;
    };

    template<typename T>
    class EventCallback : public IEventCallback
    {
    public:
        EventCallback(T* p_instance, void(T::* p_function)()) :
            m_function{ p_function }, m_instance{ p_instance }, m_id{ ++m_staticId } {}

        //@warning DO NOT USE, not functional yet
        template<typename ...Args, typename ...funcArgs>
        EventCallback(T* p_instance, void(T::* p_function)(funcArgs...), Args&&... p_args) :
            m_function{ p_function(&p_args...) }, m_instance{ p_instance }, m_id{ ++m_staticId } {}

        void operator () () override { (m_instance->*m_function)(); }

        //@warning: not fully tested

        virtual bool operator == (IEventCallback* p_other) override
        {
            auto callback = dynamic_cast<EventCallback<T>*>(p_other);

            if (p_other == nullptr || callback == nullptr)
                return false;
           

            return (callback->m_function == m_function &&
                callback->m_instance == m_instance);
        }
        bool operator == (EventCallback<T>& p_other)
        {
            if (p_other == nullptr)
                return false;


            return 	(m_function == p_other.m_function) &&
                (m_instance == p_other.m_instance);
        }

        const uint32_t GetID() const { return m_id; }

    private:
        void (T::* m_function)();
        T* m_instance;
        const uint32_t m_id;
        inline static uint32_t m_staticId = 0;
    };
}