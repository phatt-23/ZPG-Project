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
#include "Renderer/ShaderDataType.h"

#include "Model/DrawableObject.h"
#include "Model/Material.h"
#include "Model/Mesh.h"
#include "Model/Model.h"

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

#endif ///ZPG_ENGINE_H
