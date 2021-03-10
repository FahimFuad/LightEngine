//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "UIUtils.h"
#include "Spike/Renderer/RendererAPISwitch.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <FontAwesome.h>

namespace Spike::GUI
{
    bool DrawScriptTextControl(const char* label, String& value, float columnWidth, bool foundScript)
    {
        bool modified = false;
        ImGui::PushID(label);

        if (!foundScript && value != "SpikeNull")
            ImGui::TextColored({ 0.9f, 0.1f, 0.1f, 1.0f }, ICON_FK_TIMES" Not Connected with ScriptEngine");
        if (value == "SpikeNull")
            ImGui::TextColored({ 1.0f, 1.0f, 0.0f, 1.0f }, ICON_FK_MINUS_SQUARE" SpikeNull is used");
        if (foundScript && value != "SpikeNull")
            ImGui::TextColored({ 0.1f, 0.9f, 0.1f, 1.0f }, ICON_FK_CHECK" Connected with ScriptEngine");

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        if (!foundScript && value != "SpikeNull")
            ImGui::PushStyleColor(ImGuiCol_Text, { 0.9f, 0.1f, 0.1f, 1.0f });
        if (value == "SpikeNull")
            ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 1.0f, 0.0f, 1.0f });
        if (foundScript && value != "SpikeNull")
            ImGui::PushStyleColor(ImGuiCol_Text, { 0.1f, 0.9f, 0.1f, 1.0f });

        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), value.c_str());

        if (ImGui::InputText("##value", buffer, sizeof(buffer)))
        {
            value = buffer;
            modified = true;
        }

        ImGui::PopStyleColor();
        ImGui::Columns(1);
        ImGui::PopID();

        return modified;
    }

    bool DrawTextControl(const char* label, const char* value, float columnWidth)
    {
        bool modified = false;

        ImGui::PushID(label);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);

        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), value);

        if (ImGui::InputText("##value", buffer, sizeof(buffer)))
        {
            value = buffer;
            modified = true;
        }

        ImGui::Columns(1);
        ImGui::PopID();
        return modified;
    }

    bool DrawTextControlWithoutLabel(String* source)
    {
        bool modified = false;
        ImGui::PushItemWidth(-1);
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), source->c_str());

        if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
        {
            *source = String(buffer);
        }
        ImGui::PopItemWidth();
        return modified;
    }

    bool DrawBoolControl(const char* label, bool* boolean, float columnWidth)
    {
        bool modified = false;
        ImGui::PushID(label);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();

        if (ImGui::Checkbox("##value", boolean))
            modified = true;

        ImGui::Columns(1);
        ImGui::PopID();
        return modified;
    }

    bool DrawIntControl(const char* label, int* value, float columnWidth)
    {
        bool modified = false;
        ImGui::PushID(label);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();

        if (ImGui::DragInt("##value", value, 0.1f))
            modified = true;

        ImGui::Columns(1);
        ImGui::PopID();
        return modified;
    }

    bool DrawFloatControl(const char* label, float* value, float columnWidth)
    {
        bool modified = false;
        ImGui::PushID(label);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();

        if (ImGui::DragFloat("##value", value, 0.1f))
            modified = true;

        ImGui::Columns(1);
        ImGui::PopID();
        return modified;
    }

    bool DrawFloat2Control(const char* label, glm::vec2& value, float columnWidth)
    {
        bool modified = false;
        ImGui::PushID(label);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();

        if (ImGui::DragFloat2("##value", glm::value_ptr(value), 0.1f))
            modified = true;

        ImGui::Columns(1);
        ImGui::PopID();
        return modified;
    }

    bool DrawFloat3Control(const char* label, glm::vec3& value, float columnWidth)
    {
        bool modified = false;
        ImGui::PushID(label);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();

        if (ImGui::DragFloat3("##value", glm::value_ptr(value), 0.1f))
            modified = true;

        ImGui::Columns(1);
        ImGui::PopID();
        return modified;
    }

    bool DrawFloat4Control(const char* label, glm::vec4& value, float columnWidth)
    {
        bool modified = false;
        ImGui::PushID(label);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();

        if (ImGui::DragFloat4("##value", glm::value_ptr(value), 0.1f))
            modified = true;

        ImGui::Columns(1);
        ImGui::PopID();
        return modified;
    }

    bool DrawColorControl4(const char* label, glm::vec4& value, float columnWidth)
    {
        bool modified = false;
        ImGui::PushID(label);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();

        ImGui::PushItemWidth(-std::numeric_limits<float>::min());
        if (ImGui::ColorEdit4("##value", glm::value_ptr(value)))
            modified = true;

        ImGui::Columns(1);
        ImGui::PopID();
        return modified;
    }

    bool DrawColorControl3(const char* label, glm::vec3& value, float columnWidth /*= 100.0f*/)
    {
        bool modified = false;
        ImGui::PushID(label);

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();

        ImGui::PushItemWidth(-std::numeric_limits<float>::min());
        if (ImGui::ColorEdit3("##value", glm::value_ptr(value)))
            modified = true;

        ImGui::Columns(1);
        ImGui::PopID();
        return modified;
    }

    void DrawVec3Control(const String& label, glm::vec3& values, float resetValue, float columnWidth)
    {
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0];

        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 0.0f });
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("X", buttonSize))
            values.x = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Y", buttonSize))
            values.y = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Z", buttonSize))
            values.z = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar(2);

        ImGui::Columns(1);

        ImGui::PopID();
    }

    void DrawToggleButton(const char* text, const ImVec4& color, bool* boolToToggle)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, color);
        if (ImGui::Button(text))
            *boolToToggle ^= true;
        ImGui::PopStyleColor();
    }

    void DrawDynamicToggleButton(const char* offLabel, const char* onLabel, const ImVec4& offColor, const ImVec4& onColor, bool* boolToModify)
    {
        if (*boolToModify)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, onColor);
            if (ImGui::Button(onLabel))
                *boolToModify = false;
        }
        else if (!*boolToModify)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, offColor);
            if (ImGui::Button(offLabel))
                *boolToModify = true;
        }
        ImGui::PopStyleColor();
    }

    void DrawColorChangingToggleButton(const char* label, const ImVec4& offBgColor, const ImVec4& onBgColor, const ImVec4& textColor, bool* boolToModify)
    {
        if (*boolToModify)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, textColor);
            ImGui::PushStyleColor(ImGuiCol_Button, onBgColor);
            if (ImGui::Button(label))
                *boolToModify = false;
        }
        else if (!*boolToModify)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, textColor);
            ImGui::PushStyleColor(ImGuiCol_Button, offBgColor);
            if (ImGui::Button(label))
                *boolToModify = true;
        }
        ImGui::PopStyleColor(2);
    }

    void DrawImageControl(const RendererID imageID, const glm::vec2& viewportDimensions)
    {
    #ifdef RENDERER_API_DX11
        ImGui::Image(imageID, ImVec2{ viewportDimensions.x, viewportDimensions.y });
    #elif defined RENDERER_API_OPENGL
        ImGui::Image(imageID, ImVec2{ viewportDimensions.x, viewportDimensions.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    #endif
    }

    bool DrawImageButtonControl(const RendererID imageID, glm::vec2 buttonSize)
    {
    #ifdef RENDERER_API_DX11
        return ImGui::ImageButton(imageID, { buttonSize.x, buttonSize.y });
    #elif defined RENDERER_API_OPENGL
        return ImGui::ImageButton(imageID, { buttonSize.x, buttonSize.y }, { 0, 1 }, { 1, 0 });
    #endif
    }

    void BeginViewport(const char* name)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin(name);
    }

    void EndViewport()
    {
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void BeginDockspace()
    {
        static bool dockspaceOpen = true;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar(3);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 270.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        style.WindowMinSize.x = minWinSizeX;
    }

    void EndDockspace() { ImGui::End(); }
}
