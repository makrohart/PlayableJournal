#include "pch.h"
#include "PlayableManager.h"

#include "mutex"

namespace {
    std::mutex s_mutex; 
    playable::PlayableManager* s_pInstance;
} 

playable::PlayableManager* playable::PlayableManager::getInstance()
{
    if (!s_pInstance) {
        s_mutex.lock(); if (!s_pInstance) {
            s_pInstance = new playable::PlayableManager();
        } s_mutex.unlock();
    } return s_pInstance;
}
