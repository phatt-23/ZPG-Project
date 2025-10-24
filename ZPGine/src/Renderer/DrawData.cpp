//
// Created by phatt on 10/24/25.
//

#include "DrawData.h"

#include "Light/LightManager.h"
#include "Light/LightStruct.h"

namespace ZPG {

static constexpr u32 s_BatchSize = 1024;

DrawData::DrawData()
    : Batch             (s_BatchSize)
    // Mirrored Shader Storage Buffers on CPU
    , MatricesStorage   ()
    , LightsStorage     ()
    , CameraStorage     ()
    , MaterialStorage   ()
    , ModelsStorage     ()
    // Shader Storage Buffers
    // light has a dynamically sized array, so I can't just use sizeof
    , MatricesSSBO      (0, sizeof(MatricesStorageBuffer))
    , LightsSSBO        (1, 
        sizeof(LightsStorageBuffer::LightCount) + 
        sizeof(LightsStorageBuffer::_pad0) +
        sizeof(LightStruct) * LightManager::s_LightCapacity)
    , CameraSSBO        (2, sizeof(CameraStorageBuffer))
    , MaterialSSBO      (3, sizeof(MaterialStorageBuffer))
    , ModelsSSBO        (4, 
        sizeof(ModelsStorageBuffer::ModelCount) + 
        sizeof(ModelsStorageBuffer::_pad_ModelCount) +
        sizeof(m4) * s_BatchSize)
{
    LightsStorage.Lights = new LightStruct[LightManager::s_LightCapacity];

    // since there can be only s_BatchSize draw commands
    // in a single batch, we should only need s_BatchSize
    // model matrices
    ModelsStorage.Models = new m4[s_BatchSize];
}

DrawData::~DrawData() {
    delete[] LightsStorage.Lights;
    delete[] ModelsStorage.Models;
}

}
