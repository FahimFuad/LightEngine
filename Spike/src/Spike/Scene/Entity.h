//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Scene/Scene.h"
#include <entt.hpp>
#include "Spike/Core/Log.h"
#include "Spike/Scene/Components.h"
#include "Spike/Scripting/ScriptEngine.h"

namespace Spike
{
    class Entity
    {
    public:
        Entity() = default;
        Entity(entt::entity handle, Scene* scene)
            :m_EntityHandle(handle), m_Scene(scene) {}

        Entity(const Entity& other) = default;

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            SPK_CORE_ASSERT(!HasComponent<T>(), "Entity already has this component!");
            T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
            m_Scene->OnComponentAdded<T>(*this, component);
            return component;
        }

        template<typename T>
        T& GetComponent()
        {
            SPK_CORE_ASSERT(HasComponent<T>(), "Entity does not have this component!");
            return m_Scene->m_Registry.get<T>(m_EntityHandle);
        }

        template<typename T>
        bool HasComponent()
        {
            return m_Scene->m_Registry.has<T>(m_EntityHandle);
        }

        template<typename T>
        void RemoveComponent()
        {
            m_Scene->m_Registry.remove_if_exists<T>(m_EntityHandle);
        }

        entt::entity Raw()
        {
            return m_EntityHandle;
        }

        bool IsValid()
        {
            return m_Scene->m_Registry.valid(m_EntityHandle);
        }

        void RemoveAllComponent()
        {
            m_Scene->m_Registry.remove_all(m_EntityHandle);
        }

        TransformComponent& Transform() { return m_Scene->m_Registry.get<TransformComponent>(m_EntityHandle); }
        const glm::mat4& Transform() const { return m_Scene->m_Registry.get<TransformComponent>(m_EntityHandle).GetTransform(); }

        operator bool() const { return m_EntityHandle != entt::null; }
        operator entt::entity() const { return m_EntityHandle; }
        operator uint32_t() const { return (uint32_t)m_EntityHandle; }

        bool operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
        bool operator!=(const Entity& other) const { return !(*this == other); }

        UUID GetUUID() { return (GetComponent<IDComponent>().ID); }
        UUID GetSceneUUID() { return m_Scene->GetUUID(); }
    public:
        Scene* m_Scene = nullptr;
    private:
        entt::entity m_EntityHandle{ entt::null };
        friend class Scene;
        friend class ScriptEngine;
    };
}