#include <stdafx.h>
#include <Objects/IObject.h>
#include <Tools/IDCounter.h>

using namespace Engine::Objects;

IObject::IObject()
{
    m_id = Tools::IDCounter::GetNewID();
}
