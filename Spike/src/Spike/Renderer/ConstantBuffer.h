//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Renderer/Shader.h"

namespace Spike
{
    enum class DataUsage
    {
        DEFAULT = 0,
        DYNAMIC
    };

    class ConstantBuffer : public RefCounted
    {
    public:
        virtual ~ConstantBuffer() = default;
        virtual void Bind() = 0;
        virtual void* GetData() = 0;
        virtual void SetData(void* data) = 0;
        virtual Uint GetSize() = 0;

        virtual RendererID GetNativeBuffer() = 0;
        virtual ShaderDomain GetShaderDomain() = 0;
        virtual DataUsage GetDataUsage() = 0;

        static Ref<ConstantBuffer> Create(const Ref<Shader>& shader, const String& name, void* data, const Uint size, const Uint bindSlot, ShaderDomain shaderDomain = ShaderDomain::VERTEX, DataUsage usage = DataUsage::DYNAMIC);
    };
}