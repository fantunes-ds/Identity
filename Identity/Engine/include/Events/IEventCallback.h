#pragma once

namespace Engine
{
    class IEventCallback
    {
    public:
        //virtual bool operator == (IEventCallback* p_other) = 0;
        virtual void operator() () = 0;
    };

    template<typename T>
    class EventCallback : public IEventCallback
    {
    public:
        EventCallback(T* p_instance, void(T::* p_function)()) : m_function{ p_function }, m_instance{ p_instance } {}

        virtual void operator () () override { (m_instance->*m_function)(); }

        /*virtual bool operator == (IEventCallback* p_other) override
        {
            EventCallback* otherEventCallback = dynamic_cast<EventCallback>(p_other);
            if (otherEventCallback == nullptr)
                return false;

            return 	(this->m_function == otherEventCallback->m_function) &&
                (this->m_instance == otherEventCallback->m_instance);
        }*/

    private:
        void (T::* m_function)();
        T* m_instance;
    };
}