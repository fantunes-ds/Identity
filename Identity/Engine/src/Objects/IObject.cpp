#include "stdafx.h"
#include <Objects/IObject.h>
#include <Tools/IDCounter.h>

Engine::Objects::IObject::IObject()
{
    m_id = Engine::Tools::IDCounter::GetNewID();
}
