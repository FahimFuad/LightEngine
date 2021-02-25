/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : DX11GraphicsContext
File Type      : h
File created on: 2021/02/11
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim
  that you wrote the original software.

2. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#pragma once
#include "Spike/Renderer/GraphicsContext.h"
#include "Spike/Renderer/Framebuffer.h"
#include <string>
#include <d3d11.h>

struct GLFWwindow;

namespace Spike
{
    class DX11Context : public GraphicsContext
    {
    public:
        DX11Context(GLFWwindow* windowHandle);
        virtual ~DX11Context();
        virtual void Init() override;
        virtual void SwapBuffers() override;

    private:
        GLFWwindow* m_WindowHandle;
    };
}