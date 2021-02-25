﻿using System;
using System.Runtime.CompilerServices;

namespace Spike
{
    public abstract class Component
    {
        public Entity Entity { get; set; }
    }
    public class TagComponent : Component
    {
        public string Tag
        {
            get => GetTag_Native(Entity.ID);
            set => SetTag_Native(Entity.ID, value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern string GetTag_Native(ulong entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void SetTag_Native(ulong entityID, string tag);

    }

    public class TransformComponent : Component
    {
        public Transform Transform
        {
            get
            {
                GetTransform_Native(Entity.ID, out Transform result);
                return result;
            }

            set
            {
                SetTransform_Native(Entity.ID, ref value);
            }
        }

        public Vector3 Translation
        {
            get
            {
                GetTranslation_Native(Entity.ID, out Vector3 result);
                return result;
            }

            set
            {
                SetTranslation_Native(Entity.ID, ref value);
            }
        }

        public Vector3 Rotation
        {
            get
            {
                GetRotation_Native(Entity.ID, out Vector3 result);
                return result;
            }

            set
            {
                SetRotation_Native(Entity.ID, ref value);
            }
        }

        public Vector3 Scale
        {
            get
            {
                GetScale_Native(Entity.ID, out Vector3 result);
                return result;
            }

            set
            {
                SetScale_Native(Entity.ID, ref value);
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void GetTransform_Native(ulong entityID, out Transform outTransform);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetTransform_Native(ulong entityID, ref Transform inTransform);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void GetTranslation_Native(ulong entityID, out Vector3 outTranslation);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetTranslation_Native(ulong entityID, ref Vector3 inTranslation);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void GetRotation_Native(ulong entityID, out Vector3 outRotation);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetRotation_Native(ulong entityID, ref Vector3 inRotation);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void GetScale_Native(ulong entityID, out Vector3 outScale);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetScale_Native(ulong entityID, ref Vector3 inScale);
    }

    public class RigidBody2DComponent : Component
    {
        public void ApplyLinearImpulse(Vector2 impulse, Vector2 offset, bool wake)
        {
            ApplyLinearImpulse_Native(Entity.ID, ref impulse, ref offset, wake);
        }

        public Vector2 GetLinearVelocity()
        {
            GetLinearVelocity_Native(Entity.ID, out Vector2 velocity);
            return velocity;
        }

        public void SetLinearVelocity(Vector2 velocity)
        {
            SetLinearVelocity_Native(Entity.ID, ref velocity);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void ApplyLinearImpulse_Native(ulong entityID, ref Vector2 impulse, ref Vector2 offset, bool wake);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void GetLinearVelocity_Native(ulong entityID, out Vector2 velocity);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetLinearVelocity_Native(ulong entityID, ref Vector2 velocity);
    }

    public enum CameraType
    {

    }

    /*TODO: Make this work fully, extend the API so that it can control every thing like a camara component has*/
    public class CameraComponent : Component
    {
        public bool IsPrimary()
        {
            return IsPrimary_Native(Entity.ID);
        }

        public void SetAsPrimary(bool isPrimary)
        {
            SetAsPrimary_Native(Entity.ID, isPrimary);
        }

        public bool IsFixedAspectRatio()
        {
            return IsFixedAspectRatio_Native(Entity.ID);
        }

        public void SetFixedAspectRatio(bool isFixedAspectRatio)
        {
            SetFixedAspectRatio_Native(Entity.ID, isFixedAspectRatio);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool IsPrimary_Native(ulong entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetAsPrimary_Native(ulong entityID, bool isPrimary);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetFixedAspectRatio_Native(ulong entityID, bool isFixedAspectRatio);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool IsFixedAspectRatio_Native(ulong entityID);

    }

    public class SpriteRendererComponent : Component
    {
        public Vector4 GetColor()
        {
            GetColor_Native(Entity.ID, out Vector4 color);
            return color;
        }
        public void SetColor(Vector4 color)
        {
            SetColor_Native(Entity.ID, ref color);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void GetColor_Native(ulong entityID, out Vector4 color);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetColor_Native(ulong entityID, ref Vector4 color);
    }
}