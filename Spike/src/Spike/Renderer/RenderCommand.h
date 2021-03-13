//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "RendererAPI.h"

namespace Spike
{
    class RenderCommand
    {
    public:
        static void Init()
        {
            s_RendererAPI->Init();
        }

        static void SetViewport(Uint x, Uint y, Uint width, Uint height)
        {
            s_RendererAPI->SetViewport(x, y, width, height);
        }

        static void SetClearColor(const glm::vec4& color)
        {
            s_RendererAPI->SetClearColor(color);
        }
        static void Clear()
        {
            s_RendererAPI->Clear();
        }

        static void DrawIndexed(Ref<Pipeline>& pipeline, Uint count = 0)
        {
            s_RendererAPI->DrawIndexed(pipeline, count);
        }

        static void DrawIndexedMesh(Uint indexCount, Uint baseIndex, Uint baseVertex)
        {
            s_RendererAPI->DrawIndexedMesh(indexCount, baseIndex, baseVertex);
        }

        static void BindBackbuffer()
        {
            s_RendererAPI->BindBackbuffer();
        }

        static void BeginWireframe()
        {
            s_RendererAPI->BeginWireframe();
        }

        static void EndWireframe()
        {
            s_RendererAPI->EndWireframe();
        }

        static void SetDepthTest(DepthTest type)
        {
            s_RendererAPI->SetDepthTest(type);
        }

    private:
        static Scope<RendererAPI> s_RendererAPI;
    };
}