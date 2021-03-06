//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#pragma once
#include "Spike/Core/UUID.h"
#include "Spike/Core/Vault.h"
#include "Spike/Renderer/Texture.h"
#include "Spike/Renderer/Mesh.h"
#include "Panels/ConsolePanel.h"
#include "SceneCamera.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <FontAwesome.h>

namespace Spike
{
    struct IDComponent
    {
        UUID ID = 0;
    };

    struct TagComponent
    {
        String Tag;
        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const String tag)
            :Tag(tag) {}
    };

    struct TransformComponent
    {
        glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& translation)
            :Translation(translation) {}

        glm::mat4 GetTransform() const
        {
            glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
            return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
        }
        void Reset() { Translation = { 0.0f, 0.0f, 0.0f }; Rotation = { 0.0f, 0.0f, 0.0f }; Scale = { 1.0f, 1.0f, 1.0f }; }
    };

    struct SpriteRendererComponent
    {
        glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
        Ref<Texture2D> Texture = nullptr;
        String TextureFilepath;
        float TilingFactor = 1.0f;

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& color)
            :Color(color) {}

        void SetTexture(const String& filepath)
        {
            Texture = Texture2D::Create(filepath);
            Vault::Submit<Texture2D>(Texture);
            TextureFilepath = filepath;
        }

        void RemoveTexture() { Texture = nullptr; TextureFilepath = ""; }

        void Reset()
        {
            Color = { 1.0f, 1.0f, 1.0f, 1.0f };
            RemoveTexture();
        }
    };

    struct CameraComponent
    {
        SceneCamera Camera;
        bool Primary = true;
        bool FixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;

        void Reset()
        {
            Primary = true;
            FixedAspectRatio = false;
        }
    };

    struct MeshComponent
    {
        Ref<Spike::Mesh> Mesh;
        String MeshFilepath;

        MeshComponent() = default;
        MeshComponent(const MeshComponent&) = default;

        void SetFilePath(String& path) { MeshFilepath = path; }
        void Reset() { Mesh = nullptr; MeshFilepath.clear(); }
    };

    struct ScriptComponent
    {
        String ModuleName;

        ScriptComponent() = default;
        ScriptComponent(const ScriptComponent&) = default;
        void Reset() { ModuleName = "SpikeNull"; }
    };

    //// 2D Physics //////////////////////////////////////////////////////////////////////
    enum class CollisionDetectionType;
    enum class SleepType;

    struct RigidBody2DComponent
    {
        enum class Type { Static, Dynamic, Kinematic };

        Type BodyType;
        bool FixedRotation = false;
        float Gravity = 1.0f;

        void* RuntimeBody = nullptr; //Needed at runtime
        CollisionDetectionType CollisionDetection;
        SleepType Sleeptype;

        RigidBody2DComponent() = default;
        RigidBody2DComponent(const RigidBody2DComponent & other) = default;

        void Reset()
        {
            BodyType = Type::Static; Gravity = 1.0f;
        }
    };

    struct BoxCollider2DComponent
    {
        glm::vec2 Offset = { 0.0f, 0.0f };
        glm::vec2 Size = { 1.0f, 1.0f };
        glm::vec2 Scale = { 1.0f, 1.0f };

        float Density = 1.0f;
        float Friction = 1.0f;
        bool ShowBounds = false;
        void* RuntimeFixture = nullptr; //Needed at runtime

        BoxCollider2DComponent() = default;
        BoxCollider2DComponent(const BoxCollider2DComponent & other) = default;

        void Reset()
        {
            Scale = { 1.0f, 1.0f };
            Offset = { 0.0f, 0.0f };
            Size = { 1.0f, 1.0f };

            Density = 1.0f;
            Friction = 1.0f;
            ShowBounds = false;
        }
    };

    struct CircleCollider2DComponent
    {
        glm::vec2 Offset = { 0.0f, 0.0f };
        float Radius = 1.0f;

        float Density = 1.0f;
        float Friction = 1.0f;

        void* RuntimeFixture = nullptr;

        CircleCollider2DComponent() = default;
        CircleCollider2DComponent(const CircleCollider2DComponent& other) = default;

        void Reset()
        {
            Offset = { 0.0f, 0.0f };
            Radius = 1.0f;

            Density = 1.0f;
            Friction = 1.0f;
        }
    };
    /////2D Physics End///////////////////////////////////////////////////////////////////

    struct SkyLightComponent
    {
        glm::vec3 Color = { 1.0f, 1.0f, 1.0f };
        float Intensity = 0.2f;

        SkyLightComponent() = default;
        SkyLightComponent(glm::vec3 color, float intensity) :
            Color(color), Intensity(intensity) {}
        void Reset() { Color = { 1.0f, 1.0f, 1.0f }; Intensity = 0.2f; }
    };

    struct PointLightComponent
    {
        glm::vec3 Color = { 1.0f, 1.0f, 1.0f };
        float Intensity = 1.0f;

        float Constant = 1.0f;
        float Linear = 0.09f;
        float Quadratic = 0.032f;

        PointLightComponent() = default;
        PointLightComponent(float constant, float linear, float quadratic, glm::vec3 color, float intensity) :
            Constant(constant), Linear(linear), Quadratic(quadratic), Color(color), Intensity(intensity) {}

        void Reset()
        {
            Color = { 1.0f, 1.0f, 1.0f };
            Intensity = 1.0f;
            Constant = 1.0f;
            Linear = 0.09f;
            Quadratic = 0.032f;
        }
    };

}
