//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "EditorLayer.h"
#include "Spike/Scene/SceneSerializer.h"
#include "Spike/Utility/FileDialogs.h"
#include "Spike/Math/Math.h"
#include "Spike/Scripting/ScriptEngine.h"
#include "UIUtils/UIUtils.h"
#include "Spike/Core/Vault.h"
#include <FontAwesome.h>
#include <imgui/imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Spike
{
    static bool s_ShowHierarchyAndInspectorPanel = true;
    static bool s_ShowConsolePanel = true;
    static bool s_ShowVaultAndCachePanel = true;
    static bool s_ShowMaterialPanel = true;

    static bool s_ShowRendererSettingsPanel = false;
    static bool s_ShowRendererProfilerPanel = false;

    EditorLayer::EditorLayer()
        : Layer("EditorLayer"), m_VaultPanel(this) {}

    void EditorLayer::OnAttach()
    {
        FramebufferSpecification fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        fbSpec.SwapChainTarget = false;
        fbSpec.BufferDescriptions.emplace_back(FramebufferSpecification::BufferDesc(FormatCode::R32G32B32A32_FLOAT, BindFlag::RENDER_TARGET | BindFlag::SHADER_RESOURCE));
        fbSpec.BufferDescriptions.emplace_back(FramebufferSpecification::BufferDesc(FormatCode::D24_UNORM_S8_UINT, BindFlag::DEPTH_STENCIL));
        m_Framebuffer = Framebuffer::Create(fbSpec);

        m_EditorScene = Ref<Scene>::Create();
        m_EditorCamera = EditorCamera(45.0f, 1.778f, 0.1f, 1000.0f);
        m_SceneHierarchyPanel.SetContext(m_EditorScene);
        UpdateWindowTitle("Spike Engine Startup Window (No project is opened)");
    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::OnScenePlay()
    {
        if (m_ReloadScriptOnPlay)
        {
            ScriptEngine::SetSceneContext(m_EditorScene);
            ScriptEngine::ReloadAssembly(Application::Get().GetScriptEngineAppAssemblyPath());
        }
        m_SceneHierarchyPanel.ClearSelectedEntity();
        m_SceneState = SceneState::Play;

        m_RuntimeScene = Ref<Scene>::Create();
        m_EditorScene->CopySceneTo(m_RuntimeScene);

        m_RuntimeScene->OnRuntimeStart();
        m_SceneHierarchyPanel.SetContext(m_RuntimeScene);
    }

    void EditorLayer::OnSceneStop()
    {
        m_RuntimeScene->OnRuntimeStop();
        m_SceneState = SceneState::Edit;

        // Unload runtime scene
        m_RuntimeScene = nullptr;

        m_SceneHierarchyPanel.ClearSelectedEntity();
        m_SceneHierarchyPanel.SetContext(m_EditorScene);
    }

    void EditorLayer::OnScenePause()
    {
        m_SceneState = SceneState::Pause;
    }

    void EditorLayer::OnSceneResume()
    {
        m_SceneState = SceneState::Play;
    }

    void EditorLayer::OnUpdate(Timestep ts)
    {
        // Resize
        FramebufferSpecification spec = m_Framebuffer->GetSpecification();
        if ( m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
            m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        // Render
        Renderer::UpdateStats();
        Renderer2D::UpdateStats();

        m_Framebuffer->Bind();
        RenderCommand::Clear();
        m_Framebuffer->Clear(m_ClearColor);

        switch (m_SceneState)
        {
        case EditorLayer::SceneState::Edit:
            m_EditorCamera.OnUpdate(ts);
            m_EditorScene->OnUpdateEditor(ts, m_EditorCamera); break;

        case EditorLayer::SceneState::Play:
            if (m_ViewportFocused) m_EditorCamera.OnUpdate(ts);
            m_RuntimeScene->OnUpdate(ts);
            m_RuntimeScene->OnUpdateRuntime(ts); break;

        case EditorLayer::SceneState::Pause:
            if (m_ViewportFocused) m_EditorCamera.OnUpdate(ts);
            m_RuntimeScene->OnUpdateRuntime(ts); break;
        }
        RenderCommand::BindBackbuffer();
        m_Framebuffer->Unbind();
    }

    void EditorLayer::OnImGuiRender()
    {
        GUI::BeginDockspace();
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open Folder", "CTRL+O"))
                    OpenFolder();

                if (ImGui::MenuItem("Open Scene"))
                    OpenScene();

                if (ImGui::MenuItem("New", "CTRL+N"))
                    NewScene();

                if (ImGui::MenuItem("Save", "CTRL+S"))
                    SaveScene();

                if (ImGui::MenuItem("Save As...", "CTRL+SHIFT+S"))
                    SaveSceneAs();

                if (ImGui::MenuItem("Exit"))
                    Application::Get().Close();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Script"))
            {
                ImGui::MenuItem("Reload assembly on play", nullptr, &m_ReloadScriptOnPlay);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View"))
            {
                if (ImGui::MenuItem("Inspector and Hierarchy"))
                    s_ShowHierarchyAndInspectorPanel = true;

                if (ImGui::MenuItem("Console"))
                    s_ShowConsolePanel = true;

                if (ImGui::MenuItem("Vault and SpikeCache"))
                    s_ShowVaultAndCachePanel = true;

                if (ImGui::MenuItem("Material Inspector"))
                    s_ShowMaterialPanel = true;

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Renderer"))
            {
                if (ImGui::MenuItem("Settings"))
                    s_ShowRendererSettingsPanel = true;

                if (ImGui::MenuItem("Profiler"))
                    s_ShowRendererProfilerPanel = true;

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        RenderPanels();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5, 0.5, 0.5, 1.0f));
        ImGui::Begin("ToolBar", false, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
        if (ImGui::Button(ICON_FK_REPEAT))
        {
            ScriptEngine::ReloadAssembly(Application::Get().GetScriptEngineAppAssemblyPath());
            SPK_CORE_LOG_INFO("ScriptEngine reloaded the C# assembly successfully!");
        }

        ImGui::SameLine();
        if (ImGui::Button(ICON_FK_FLOPPY_O)) SaveScene();
        ImGui::SameLine();

        ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2);
        if (m_SceneState == SceneState::Edit)
        {
            if (ImGui::ArrowButton("Play", ImGuiDir_Right))
                OnScenePlay();
            ImGui::SameLine();
            if (ImGui::Button(ICON_FK_PAUSE))
                SPK_CORE_LOG_WARN("You can pause the scene only in Playmode!");
        }
        else if (m_SceneState == SceneState::Play)
        {
            if (ImGui::Button(ICON_FK_STOP))
                OnSceneStop();
            ImGui::SameLine();
            if (ImGui::Button(ICON_FK_PAUSE))
                OnScenePause();
        }
        else if (m_SceneState == SceneState::Pause)
        {
            if (ImGui::Button(ICON_FK_STOP))
                OnSceneStop();
            ImGui::SameLine();
            if (ImGui::Button(ICON_FK_PAUSE))
                OnSceneResume();
        }
        ImGui::End();
        ImGui::PopStyleColor(3);

        GUI::BeginViewport(ICON_FK_GAMEPAD" Viewport");
        auto viewportOffset = ImGui::GetCursorPos();

        if (m_SceneState == SceneState::Play)
            DrawRectAroundWindow({ 1.0f, 1.0f, 0.0f, 1.0f });
        else if (m_SceneState == SceneState::Pause)
            DrawRectAroundWindow({ 0.0f, 0.0f, 1.0f, 1.0f });

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        GUI::DrawImageControl(m_Framebuffer->GetColorViewID(), m_ViewportSize);
        RenderGizmos();
        GUI::EndViewport();

        if (s_ShowRendererSettingsPanel)
        {
            ImGui::Begin("Renderer Settings", &s_ShowRendererSettingsPanel);
            GUI::DrawColorControl4("Clear Color", m_ClearColor);
            ImGui::Separator();

            if (ImGui::TreeNodeEx("Configure SKYBOX", ImGuiTreeNodeFlags_OpenOnArrow))
            {
                ImGui::TextColored({ 0.1f, 0.9f, 0.1f, 1.0f }, ICON_FK_ARROW_DOWN " IMPORTANT notes regarding Skybox " ICON_FK_ARROW_DOWN);
                ImGui::BulletText("Remember the folder must contain 6 exactly image files, nothing else");
                ImGui::BulletText("The image files must be named as \"Aright, Bleft, Ctop, Dbottom, Efront, Fback\"");
                ImGui::BulletText("The names represents the 6 sides of a SKYBOX");
                ImGui::BulletText("Yes, the prefix A, B, C, D, E, F in front of the image file names are necessary!");
                GUI::DrawDynamicToggleButton(ICON_FK_TIMES, ICON_FK_CHECK, { 0.7f, 0.1f, 0.1f, 1.0f }, { 0.2f, 0.5f, 0.2f, 1.0f }, &Renderer::GetSkyboxActivationBool());
                GUI::DrawToolTip("Use Skybox");
                ImGui::SameLine();

                if (ImGui::Button("Open Skybox"))
                {
                    const char* folderpath = FileDialogs::SelectFolder("Open A folder containing skybox");
                    if (folderpath)
                    {
                        m_CurrentSkyboxPath = folderpath;
                        Renderer::GetSkyboxSlot() = Skybox::Create(TextureCube::Create(m_CurrentSkyboxPath));
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("Generate Skybox"))
                {
                    if (!m_CurrentSkyboxPath.empty())
                        Renderer::GetSkyboxSlot() = Skybox::Create(TextureCube::Create(m_CurrentSkyboxPath));
                    else
                        SPK_CORE_LOG_WARN("Select a skybox path first via 'Open Skybox' button!");
                }
                ImGui::SameLine();
                if (ImGui::Button("Destroy Skybox"))
                    Renderer::GetSkyboxSlot().Reset();

                ImGui::PushItemWidth(-1);
                ImGui::Text("Skybox folderpath:");
                ImGui::SameLine();
                ImGui::InputText("##skyboxFilepath", (char*)m_CurrentSkyboxPath.c_str(), 256, ImGuiInputTextFlags_ReadOnly);
                ImGui::PopItemWidth();
                ImGui::TreePop();
            }
            ImGui::Separator();
            ImGui::End();
        }

        GUI::EndDockspace();
    }

    void EditorLayer::OnEvent(Event& e)
    {
        m_SceneHierarchyPanel.OnEvent(e);
        if (m_SceneState == SceneState::Edit)
        {
            if (m_ViewportHovered)
                m_EditorCamera.OnEvent(e);
            m_EditorScene->OnEvent(e);
        }
        else if (m_SceneState == SceneState::Play)
            m_RuntimeScene->OnEvent(e);

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(SPK_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        // Shortcuts
        if (e.GetRepeatCount() > 0)
            return false;

        bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
        bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
        switch (e.GetKeyCode())
        {
            case Key::N: if (control) NewScene();   break;
            case Key::O: if (control) OpenFolder(); break;
            case Key::S: if (control) SaveScene(); if (control && shift) SaveSceneAs(); break;

            // Gizmos
            case Key::Q: if (!m_GizmoInUse) m_GizmoType = -1;                             break;
            case Key::W: if (!m_GizmoInUse) m_GizmoType = ImGuizmo::OPERATION::TRANSLATE; break;
            case Key::E: if (!m_GizmoInUse) m_GizmoType = ImGuizmo::OPERATION::ROTATE;    break;
            case Key::R: if (!m_GizmoInUse) m_GizmoType = ImGuizmo::OPERATION::SCALE;     break;
        }
        return false;
    }

    void EditorLayer::UpdateWindowTitle(const String& sceneName)
    {
        String title = "Spike |" + sceneName + "| " + Application::GetPlatformName() + " - " + Application::GetConfigurationName() + " <" + Application::CurrentGraphicsAPI() + "> ";
        Application::Get().GetWindow().SetTitle(title);
    }

    void EditorLayer::DrawRectAroundWindow(const glm::vec4& color)
    {
        ImVec2 windowMin = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImVec2 windowMax = { windowMin.x + windowSize.x, windowMin.y + windowSize.y };
        ImGui::GetForegroundDrawList()->AddRect(windowMin, windowMax, ImGui::ColorConvertFloat4ToU32(ImVec4(color.x, color.y, color.z, color.w)));
    }

    void EditorLayer::RenderGizmos()
    {
        Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
        if (selectedEntity && m_GizmoType != -1 && m_SceneState != SceneState::Play)
        {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            float windowWidth = (float)ImGui::GetWindowWidth();
            float windowHeight = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

            glm::mat4 cameraView, cameraProjection;

            if (m_SceneState == SceneState::Play)
            {
                auto cameraEntity = m_EditorScene->GetPrimaryCameraEntity();
                const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
                cameraProjection = camera.GetProjection();
                cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());
            }

            if (m_SceneState == SceneState::Edit)
            {
                cameraProjection = m_EditorCamera.GetProjection();
                cameraView = m_EditorCamera.GetViewMatrix();
            }

            // Entity transform
            auto& tc = selectedEntity.GetComponent<TransformComponent>();
            glm::mat4 transform = tc.GetTransform();

            // Snapping
            bool snap = Input::IsKeyPressed(Key::LeftControl);
            float snapValue = 0.5f; // Snap to 0.5m for translation/scale
            // Snap to 45 degrees for rotation
            if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
                snapValue = 45.0f;

            float snapValues[3] = { snapValue, snapValue, snapValue };
            ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
                nullptr, snap ? snapValues : nullptr);

            if (ImGuizmo::IsUsing())
            {
                m_GizmoInUse = true;
                glm::vec3 translation, rotation, scale;
                Math::DecomposeTransform(transform, translation, rotation, scale);

                glm::vec3 deltaRotation = rotation - tc.Rotation;
                tc.Translation = translation;
                tc.Rotation += deltaRotation;
                tc.Scale = scale;
            }
            else
                m_GizmoInUse = false;
        }
    }

    void EditorLayer::RenderPanels()
    {
        if(s_ShowConsolePanel)
            Console::Get()->OnImGuiRender(&s_ShowConsolePanel);

        if(s_ShowHierarchyAndInspectorPanel)
            m_SceneHierarchyPanel.OnImGuiRender(&s_ShowHierarchyAndInspectorPanel);

        if(s_ShowRendererProfilerPanel)
            m_ProfilerPanel.OnImGuiRender(&s_ShowRendererProfilerPanel);

        if(s_ShowVaultAndCachePanel)
            m_VaultPanel.OnImGuiRender(&s_ShowVaultAndCachePanel);

        if(s_ShowMaterialPanel)
            m_MaterialPanel.OnImGuiRender(&s_ShowMaterialPanel, m_SceneHierarchyPanel.GetSelectedEntity());
    }

    /* [Spike] FILE STUFF [Spike] */
    void EditorLayer::NewScene()
    {
        const char* filepath = FileDialogs::SelectFolder("Select a location to save project files");
        if (filepath)
        {
            m_EditorScene = Ref<Scene>::Create();
            m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_SceneHierarchyPanel.SetContext(m_EditorScene);

            m_EditorCamera = EditorCamera(45.0f, 1.778f, 0.1f, 1000.0f);
            m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);

            m_FirstTimeSave = false;
            Vault::Init(filepath);

            String projectName = Vault::GetNameWithoutExtension(filepath);
            m_ActiveFilepath = String(filepath) + "/" + projectName + ".spike";

            SceneSerializer serializer(m_EditorScene);
            serializer.Serialize(m_ActiveFilepath);
            UpdateWindowTitle(projectName);
        }
    }

    void EditorLayer::OpenFolder()
    {
        const char* filepath = FileDialogs::SelectFolder("Select a folder to open");
        if (filepath)
        {
            m_EditorScene = Ref<Scene>::Create();
            m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_SceneHierarchyPanel.SetContext(m_EditorScene);

            m_EditorCamera = EditorCamera(45.0f, 1.778f, 0.1f, 1000.0f);
            m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);

            Vault::Init(filepath);

            String projectName = Vault::GetNameWithoutExtension(filepath);
            UpdateWindowTitle(projectName);
        }
    }

    void EditorLayer::OpenScene()
    {
        const char* pattern[1] = { "*.spike" };
        const char* filepath = FileDialogs::OpenFile("Open Scene", "", 1, pattern, "", false);
        if (filepath)
        {
            m_FirstTimeSave = false;
            m_ActiveFilepath = filepath;
            m_EditorScene = Ref<Scene>::Create();
            m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_SceneHierarchyPanel.SetContext(m_EditorScene);

            SceneSerializer serializer(m_EditorScene);
            serializer.Deserialize(filepath);
            SPK_CORE_LOG_INFO("Succesfully deserialized scene!");
        }
    }

    void EditorLayer::SaveSceneAs()
    {
        const char* pattern[1] = { "*.spike" };
        const char* filepath = FileDialogs::SaveFile("Save Scene", "Scene.spike", 1, pattern, "Spike Scene");
        if (filepath)
        {
            m_FirstTimeSave = false;
            SceneSerializer serializer(m_EditorScene);
            serializer.Serialize(filepath);
            SPK_CORE_LOG_INFO("Scene serialized succesfully!");
        }
    }

    void EditorLayer::SaveScene()
    {
        if (m_FirstTimeSave)
            SaveSceneAs();
        else
        {
            SceneSerializer serializer(m_EditorScene);
            serializer.Serialize(m_ActiveFilepath);
            SPK_CORE_LOG_INFO("Scene Saved!");
        }
    }
}
