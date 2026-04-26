#pragma once

#include <d3d11.h>

bool LoadTextureFromFile(ID3D11Device* d3d_device, const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);