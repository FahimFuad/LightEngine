/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : Shader
File Type      : h
File created on: 2021/01/09
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim
  that you wrote the original software.
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERFECTLY LEGAL if you are contributing.

3. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#pragma once
#include "Spike/Core/Ref.h"
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

namespace Spike
{
    class Shader : public RefCounted
    {
    public:
        using ShaderReloadedCallback = std::function<void()>;
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        /* [Spike] Shader uniform setters, without and materials... just in case if we need it, we can use it [Spike] */
        virtual void SetInt(const String& name, int value) = 0;
        virtual void SetIntArray(const String& name, int* value, uint32_t count) = 0;
        virtual void SetFloat(const String& name, float value) = 0;
        virtual void SetFloat2(const String& name, const glm::vec2& value) = 0;
        virtual void SetFloat3(const String& name, const glm::vec3& value) = 0;
        virtual void SetFloat4(const String& name, const glm::vec4& value) = 0;
        virtual void SetMat4(const String& name, const glm::mat4& value) = 0;

        virtual void AddShaderReloadedCallback(const ShaderReloadedCallback& callback) = 0;
        virtual void Load(const String& source) = 0;
        virtual void Reload() = 0;
        virtual void DumpShaderData() = 0;
        virtual const String& GetName() const = 0;

        static Ref<Shader> Create(const String& filepath);
        static Vector<Ref<Shader>> s_AllShaders; //TODO: Move to asset manager!
    };

    class ShaderLibrary : public RefCounted
    {
    public:
        void Add(const String& name, const Ref<Shader>& shader);
        void Add(const Ref<Shader>& shader);
        Ref<Shader> Load(const String& filepath);
        Ref<Shader> Load(const String& name, const String& filepath);

        Ref<Shader> Get(const String& name);
        bool Exists(const String& name) const;
    private:
        std::unordered_map<String, Ref<Shader>> m_Shaders;
    };
}