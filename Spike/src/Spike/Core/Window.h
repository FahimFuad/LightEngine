//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once

#include <sstream>

#include "Spike/Core/Base.h"
#include "Spike/Events/Event.h"

namespace Spike
{
    struct WindowProps
    {
        String Title;
        Uint Width;
        Uint Height;

        WindowProps(const String& title = "Spike",
                    Uint width = 1280,
                    Uint height = 720)
            : Title(title), Width(width), Height(height) {}
    };

    // Interface representing a desktop system based Window
    class  Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() = default;

        virtual void OnUpdate() = 0;

        virtual Uint GetWidth() const = 0;
        virtual Uint GetHeight() const = 0;

        // Window attributes
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        virtual void SetTitle(const String& name) = 0;
        virtual const String& GetTitle() const = 0;

        virtual void* GetNativeWindow() const = 0;

        static Window* Create(const WindowProps& props = WindowProps());
    };

}