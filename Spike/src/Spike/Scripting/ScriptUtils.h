//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Scene/Components.h"
#include "Spike/Scripting/ScriptEngine.h"
#include <mono/jit/jit.h>

namespace Spike::Scripting
{
    /* [Spike] Utils begin [Spike] */
    String ConvertMonoStringToCppString(MonoString* message)
    {
        /* [Spike] Any way to make this efficient? we are doing 2 copies here! [Spike] */
        char* ptr = mono_string_to_utf8(message);
        String s{ ptr };
        mono_free(ptr); /* [Spike] Yes, we need to free this with mono_free [Spike] */
        return s;
    }

    char* CovertMonoObjectToCppChar(MonoObject* obj)
    {
        if (obj == NULL)
        {
            char* a = "Null";
            return a;
        }
        else
        {
            MonoString* a = mono_object_to_string(obj, NULL);
            String b = ConvertMonoStringToCppString(a);
            char* s = _strdup(b.c_str());
            return s;
        }

    }

    MonoString* ConvertCppStringToMonoString(MonoDomain* domain, const std::string& str)
    {
        return mono_string_new(domain, str.c_str());
    }

    EntityMap ValidateSceneAndReturnEntityMap(Ref<Scene>& sceneContext, uint64_t entityID)
    {
        SPK_CORE_ASSERT(sceneContext, "No active scene!");
        const auto& entityMap = sceneContext->GetEntityMap();
        SPK_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Entity ID is invalid!");
        return entityMap;
    }

    template<typename T>
    T& ValidateSceneAndReturnAComponent(Ref<Scene>& sceneContext, uint64_t entityID)
    {
        auto& entityMap = ValidateSceneAndReturnEntityMap(ScriptEngine::GetSceneContext(), entityID);
        Entity entity = entityMap.at(entityID);
        SPK_CORE_ASSERT(entity.HasComponent<T>(), "");
        auto& component = entity.GetComponent<T>();
        return component;
    }
    /* [Spike] Utils end [Spike] */
}
