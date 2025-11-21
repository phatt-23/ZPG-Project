//
// Created by phatt on 9/22/25.
//

#ifndef ZPG_ENGINE_H
#define ZPG_ENGINE_H

#include "Core/Application.h"
#include "Core/Input.h"
#include "Core/Core.h"
#include "Core/Layer.h"
#include "Core/LayerStack.h"
#include "Core/Window.h"
#include "Core/Timestep.h"
#include "Core/Utility.h"
#include "Core/KeyCodes.h"

#include "Resource/ResourceManager.h"
#include "Resource/CommonResources.h"

#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

#include "Debug/Asserter.h"
#include "Debug/Logger.h"

#include "Event/Event.h"
#include "Event/ApplicationEvent.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/RenderStatistics.h"
#include "Renderer/Renderer.h"

#include "Buffer/VertexArray.h"
#include "Buffer/VertexBuffer.h"
#include "Buffer/IndexBuffer.h"
#include "Buffer/BufferElement.h"
#include "Buffer/BufferLayout.h"

#include "Camera/Camera.h"
#include "Camera/ObservableCameraController.h"
#include "Camera/CameraController.h"
#include "Camera/FlashlightCameraController.h"

#include "Texture/Texture2D.h"
#include "Texture/TextureLibrary.h"

#include "Shader/ShaderDataType.h"
#include "Shader/Shader.h"
#include "Shader/ShaderProgram.h"

#include "Model/Mesh.h"
#include "Model/Model.h"
#include "Model/ModelLoader.h"
#include "Model/ModelLibrary.h"

#include "Entity/Entity.h"
#include "Entity/PointLightEntity.h"
#include "Entity/SpotLightEntity.h"

#include "Transform/TransformGroup.h"
#include "Transform/Transform.h"
#include "Transform/StaticTransform.h"
#include "Transform/DynTransform.h"
#include "Transform/StaticTransform/Rotate.h"
#include "Transform/StaticTransform/Scale.h"
#include "Transform/StaticTransform/Translate.h"
#include "Transform/DynamicTransform/DynRotate.h"
#include "Transform/DynamicTransform/DynScale.h"
#include "Transform/DynamicTransform/DynTranslate.h"
#include "Transform/EffectTransform/PulseScale.h"

#include "Transform/MovementTransform/LineMovement.h"
#include "Transform/MovementTransform/CircleMovement.h"
#include "Transform/MovementTransform/BezierMovement.h"

#include "Light/Light.h"
#include "Light/AmbientLight.h"
#include "Light/SpotLight.h"
#include "Light/PointLight.h"
#include "Light/DirectionalLight.h"

#include "Observer/Observable.h"
#include "Observer/Observer.h"
#include "Observer/Payload.h"

#include "Material/Material.h"
#include "Material/MaterialLibrary.h"

#include "Sky/Skybox.h"
#include "Sky/Skydome.h"

#endif ///ZPG_ENGINE_H
