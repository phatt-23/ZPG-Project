//
// Created by phatt on 10/24/25.
//

#include "DrawData.h"

#include "Light/LightManager.h"
#include "Light/LightStruct.h"

namespace ZPG {

DrawData::DrawData()
    : Batch             (1024)
    , MatricesStorage   ()
    , LightsStorage     ()
    , CameraStorage     ()
    , MaterialStorage   ()
    // Shader Storage Buffers
    // light has a dynamically sized array, so I can't just use sizeof
    , MatricesSSBO      (0, sizeof(MatricesStorageBuffer))
    , LightsSSBO        (1, sizeof(i32) + LightManager::s_LightCapacity * sizeof(LightStruct))
    , CameraSSBO        (2, sizeof(CameraStorageBuffer))
    , MaterialSSBO      (3, sizeof(MaterialStorageBuffer))
{
    // Mirrored Shader Storage Buffers on CPU
    LightsStorage.Lights = new LightStruct[LightManager::s_LightCapacity];
}

DrawData::~DrawData() {
    delete[] LightsStorage.Lights;
}

}
