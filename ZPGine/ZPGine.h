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

#include "Controller/ObservableCameraController.h"
#include "Controller/CameraController.h"

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

#include "Light/Light.h"
#include "Light/AmbientLight.h"
#include "Light/SpotLight.h"
#include "Light/PointLight.h"
#include "Light/DirectionalLight.h"

#include "Observer/Observable.h"
#include "Observer/Observer.h"

#endif ///ZPG_ENGINE_H
