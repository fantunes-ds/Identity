#pragma once
#include <Export.h>
#include <Events/IEventCallback.h>
#include <Tools/IDCounter.h>
#include <map>

namespace Engine
{
    class API_ENGINE Event
    {
    public:
        Event() = default;
        ~Event() = default;

        /*
         *@brief Subscribes a function or method to this Event. The function or method will be called if thisEvent.fire() is called.
         *@return Returns ID of callback if successful, returns -1 if callback has already been added to this Event (avoids duplicates).
         */
        template<typename T>
        const uint32_t AddListener(T* p_instance, void(T::* p_function)())
        {
            auto newCallback = std::make_shared<EventCallback<T>>(p_instance, p_function);

            //check if delegate is already added to vector
            for (auto callback : m_actions)
            {
                EventCallback<T>* cast = dynamic_cast<EventCallback<T>*>(&*callback.second);

                if (cast != nullptr)
                {
                    if (*cast == *newCallback)
                        return -1;
                }
            }

            int32_t id = Tools::IDCounter::GetNewID();

                m_actions.insert_or_assign(id, newCallback);

                return id;
        }

        //@warning DO NOT USE, not functional yet
        /*template<typename T, typename ...Args, typename ...funcArgs>
        const uint32_t AddListener(T* p_instance, void(T::* p_function)(funcArgs...), Args&&... p_args)
        {
            auto newCallback = std::make_shared<EventCallback<T>>(p_instance, p_function, &p_args...);

            //check if delegate is already added to vector
            for (auto callback: m_actions)
            {
                  if  (*dynamic_cast<EventCallback<T>*>(callback.second.get()) == &*newCallback)
                    return -1;
            }

            m_actions.insert_or_assign(newCallback.get()->GetNewID(), newCallback);

            return newCallback.get()->GetNewID();
        }*/

        void RemoveListener(const int32_t p_id)
        {
            m_actions.erase(p_id);
        }

        void RemoveAllListeners()
        {
            m_actions.clear();
        }

        std::shared_ptr<IEventCallback> GetCallback(const uint32_t p_id)
        {
            return m_actions.at(p_id);
        }

        void Fire();

    private:
        std::map<const uint32_t, std::shared_ptr<IEventCallback>> m_actions;
    };
}
