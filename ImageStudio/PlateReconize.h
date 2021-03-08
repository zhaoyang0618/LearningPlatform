#pragma once

struct MNPlateInfoStruct
{
    int x, y, w, h;
    int plate_type;	// 0,1,2,3,
    char plate_code[64];
};

extern "C" int InitmnPlateReco(const char* region_cfg, const char* region_weights, const char* code_cfg, const char* code_weights);
extern "C" int ForwardmnPlateReco(const char* jpgfilename, MNPlateInfoStruct & plateinfo);
extern "C" int DisposemnPlateReco();
extern "C" int PLATE_InitmnReco(const char* region_cfg, const char* region_weights, const char* code_cfg, const char* code_weights);
extern "C" int PLATE_ForwardmnReco(const char* jpgfilename, MNPlateInfoStruct & plateinfo);
extern "C" int PLATE_ForwardmnRecoMat(unsigned char* matbuffer, int w, int h, MNPlateInfoStruct & plateinfo);
extern "C" int PLATE_DisposemnReco();

