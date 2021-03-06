//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "VaultPanel.h"
#include "Spike/Core/Vault.h"
#include "Spike/Scene/SceneSerializer.h"
#include "Spike/Renderer/RendererAPISwitch.h"
#include "Spike/Utility/FileDialogs.h"
#include "UIUtils/UIUtils.h"
#include "EditorLayer.h"
#include <filesystem>
#include <imgui/imgui.h>

namespace Spike
{
    static void* s_EditorLayerStorage;
    static Ref<Texture> s_TexturePreviewStorage;
    static bool s_Loaded = false; //TEMP (TODO)

    VaultPanel::VaultPanel(const void* editorLayerPtr)
    {
        s_EditorLayerStorage = (EditorLayer*)editorLayerPtr;
        m_ProjectPath.clear();
        s_Loaded = false;
    }

    Vector<DirectoryEntry> VaultPanel::GetFiles(const String& directory)
    {
        Vector<DirectoryEntry> result;
        try
        {
            for (const auto& entry : std::filesystem::directory_iterator(directory))
            {
                String path = entry.path().string();
                DirectoryEntry e = { entry.path().stem().string(), entry.path().extension().string(), path, entry.is_directory() };

                if (entry.is_directory())
                    e.SubEntries = GetFiles(entry.path().string());
                result.push_back(e);
            }
        }
        catch (const std::filesystem::filesystem_error& e)
        {
            SPK_CORE_LOG_ERROR("%s !", e.what());
        }
        catch (...)
        {
            SPK_CORE_LOG_ERROR("Error on filesystem!");
        }
        return result;
    }

    void VaultPanel::OnImGuiRender(bool* show)
    {
        if (m_ProjectPath.empty())
            m_ProjectPath = Vault::GetProjectPath();

        ImGui::Begin("Spike Vault", show);
        if (ImGui::Button(ICON_FK_REFRESH))
        {
            if (Vault::IsVaultInitialized())
            {
                Vault::Reload();
                m_ProjectPath = Vault::GetProjectPath();
                m_Files = GetFiles(m_ProjectPath);
            }
            else
                SPK_CORE_LOG_WARN("Open A working directory first! Go to 'File>Open Folder' to open a working directory.");
        }

        if (!s_Loaded && !m_ProjectPath.empty())
        {
            m_Files = GetFiles(m_ProjectPath);
            s_Loaded = true;
        }

        if (Vault::IsVaultInitialized())
            for (auto& file : m_Files)
                DrawPath(file);
        ImGui::End();

        ImGui::Begin("Texture Preview", show, ImGuiWindowFlags_HorizontalScrollbar);
        if (s_TexturePreviewStorage)
        {
            auto rendererID = s_TexturePreviewStorage->GetRendererID();
            glm::vec2 imageRes = { s_TexturePreviewStorage->GetWidth(), s_TexturePreviewStorage->GetHeight() };
            ImVec2 windowRes = ImGui::GetWindowSize();

            DrawImageAtMiddle(imageRes, { windowRes.x, windowRes.y });
            GUI::DrawImageControl(rendererID, { imageRes.x, imageRes.y });
        }
        else
        {
            ImVec2 windowRes = ImGui::GetWindowSize();
            ImGui::SetCursorPos({ windowRes.x * 0.2f, windowRes.y * 0.5f });
            ImGui::TextUnformatted("No Texture is selected. Select an image file\nin the Spike Vault to show it up here!");
        }
        ImGui::End();

        ImGui::Begin("SpikeCache", show);
        if (ImGui::TreeNode("Shaders"))
        {
            auto& shaders = Vault::GetAllShaders();
            for (auto& shader : shaders)
                if (shader)
                    if (ImGui::TreeNode(shader->GetName().c_str()))
                        ImGui::TreePop();
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Textures"))
        {
            auto& textures = Vault::GetAllTextures();
            for (auto& texture : textures)
                if (texture)
                    if (ImGui::TreeNode(texture->GetName().c_str()))
                        ImGui::TreePop();
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Scripts"))
        {
            auto& scripts = Vault::GetAllScripts();
            for (auto& script : scripts)
                if (script.first.c_str())
                    if (ImGui::TreeNode(Vault::GetNameWithExtension(script.first).c_str()))
                        ImGui::TreePop();
            ImGui::TreePop();
        }
        ImGui::End();
    }

    void VaultPanel::DrawPath(DirectoryEntry& entry)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

        if (!entry.IsDirectory)
            flags |= ImGuiTreeNodeFlags_Leaf;

        String nodeString;

        bool codeExtBools = entry.Extension == ".cs" || entry.Extension == ".glsl" || entry.Extension == ".cpp" || entry.Extension == ".lua"
            || entry.Extension == ".py" || entry.Extension == ".hlsl" || entry.Extension == ".js" || entry.Extension == ".c" || entry.Extension == ".h";

        bool imageExtBools = entry.Extension == ".png" || entry.Extension == ".jpg" || entry.Extension == ".gif"
            || entry.Extension == ".bmp" || entry.Extension == ".psd";

        if (codeExtBools)
            nodeString = ICON_FK_CODE + String(" ") + entry.Name + entry.Extension;
        else if (entry.Extension == ".spike" || entry.Extension == ".txt")
            nodeString = ICON_FK_FILE_TEXT_O + String(" ") + entry.Name + entry.Extension;
        else if (imageExtBools)
            nodeString = ICON_FK_FILE_IMAGE_O + String(" ") + entry.Name + entry.Extension;
        else if (entry.IsDirectory)
            nodeString = ICON_FK_FOLDER + String(" ") + entry.Name;
        else nodeString = entry.Name + entry.Extension;

        if (ImGui::TreeNodeEx(nodeString.c_str(), flags))
        {
            if (entry.IsDirectory)
                for (auto& subDirectory : entry.SubEntries)
                    DrawPath(subDirectory);

            /* [Spike] Loading Spike files [Spike] */
            if (entry.Extension == ".spike" && ImGui::IsItemClicked(0))
            {
                int filepath = FileDialogs::AMessageBox("", "Do you want to open this scene?", DialogType::Yes__No, IconType::Question, DefaultButton::No);
                if(filepath)
                {
                    ((EditorLayer*)s_EditorLayerStorage)->m_ActiveFilepath = entry.AbsolutePath;
                    ((EditorLayer*)s_EditorLayerStorage)->m_FirstTimeSave = false;
                    ((EditorLayer*)s_EditorLayerStorage)->m_EditorScene = Ref<Scene>::Create();
                    ((EditorLayer*)s_EditorLayerStorage)->m_EditorScene->OnViewportResize((uint32_t)((EditorLayer*)s_EditorLayerStorage)->m_ViewportSize.x, (uint32_t)((EditorLayer*)s_EditorLayerStorage)->m_ViewportSize.y);
                    ((EditorLayer*)s_EditorLayerStorage)->m_SceneHierarchyPanel.SetContext(((EditorLayer*)s_EditorLayerStorage)->m_EditorScene);

                    SceneSerializer serializer(((EditorLayer*)s_EditorLayerStorage)->m_EditorScene);
                    serializer.Deserialize(entry.AbsolutePath);
                    ImGui::SetWindowFocus(String(ICON_FK_GAMEPAD" Viewport").c_str());
                }
            }

            /* [Spike] Texture [Spike] */
            if (imageExtBools && ImGui::IsItemClicked(0))
            {
                if (s_TexturePreviewStorage)
                    s_TexturePreviewStorage = nullptr;
                s_TexturePreviewStorage = Texture2D::Create(entry.AbsolutePath);
                ImGui::SetWindowFocus(String("Texture Preview").c_str());
            }
            ImGui::TreePop();
        }
    }

    /* [Spike] Simple helper function with simple Math [Spike] */
    void VaultPanel::DrawImageAtMiddle(const glm::vec2& imageRes, const glm::vec2& windowRes)
    {
        glm::vec2 imageMiddle = { imageRes.x * 0.5f, imageRes.y * 0.5f };
        glm::vec2 windowMiddle = { windowRes.x * 0.5f, windowRes.y * 0.5f };

        glm::vec2 result = { windowMiddle - imageMiddle };
        ImGui::SetCursorPos({ result.x, result.y });
    }
}
