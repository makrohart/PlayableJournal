#include "pch.h"

#include "PlayableManager.h"
#include "PlayableType.h"

void playable::PlayableType::addMethod(const std::string& name, v8::FunctionCallback callback)
{
    if (m_methods.find(name) != m_methods.cend())
        return;

    m_methods[name] = callback;
}

void playable::PlayableType::end()
{
    for (auto toDelete : m_toDeletes)
        delete toDelete;

    PlayableManager::getInstance()->add(this);
}