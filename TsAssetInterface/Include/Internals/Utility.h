#pragma once

#include "structs.h"

namespace TS
{
    // ! テクスチャを読み込みます
    TS::BasicTexture LoadTextureFromFile(const char * fileName);

    TS::BasicModel   LoadModelFromFile(const char * filename);
}

 