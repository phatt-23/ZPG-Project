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
#include "Core/Scene.h"
#include "Core/SceneContext.h"
#include "Core/SceneManager.h"
#include "Core/Window.h"
#include "Core/Timestep.h"
#include "Core/Utility.h"
#include "Core/ResourceManager.h"

#include "Debug/Asserter.h"
#include "Debug/Logger.h"

#include "Event/Event.h"
#include "Event/ApplicationEvent.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"

#include "Renderer/Renderer.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Camera.h"
#include "Renderer/CameraController.h"

#include "Texture/Texture.h"
#include "Texture/TextureLibrary.h"

#include "Shader/ShaderDataType.h"
#include "Shader/Shader.h"
#include "Shader/ShaderProgram.h"

#include "Model/Mesh.h"
#include "Model/Model.h"
#include "Model/ModelLoader.h"
#include "Model/ModelLibrary.h"

#include "Entity/Entity.h"

#include "Transform/CompoundTransform.h"
#include "Transform/Transform.h"
#include "Transform/StaticTransform.h"
#include "Transform/DynTransform.h"
#include "Transform/StaticTransform/RotationTransform.h"
#include "Transform/StaticTransform/ScaleTransform.h"
#include "Transform/StaticTransform/TranslationTransform.h"
#include "Transform/DynamicTransform/DynRotationTransform.h"
#include "Transform/DynamicTransform/DynScaleTransform.h"
#include "Transform/DynamicTransform/DynTranslationTransform.h"
#include "Transform/EffectTransform/PulseScaleTransform.h"

#include "Light/Light.h"
#include "Light/AmbientLight.h"
#include "Light/SpotLight.h"
#include "Light/PointLight.h"
#include "Light/DirectionalLight.h"


#endif ///ZPG_ENGINE_H
