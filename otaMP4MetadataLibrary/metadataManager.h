#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>
#include <iomanip>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}

#ifdef OTAMP4METADATALIBRARY_EXPORTS

#define OTAMP4METADATALIB_API __declspec(dllexport) 

#else

#define OTAMP4METADATALIB_API __declspec(dllimport) 

#endif

extern "C"
{
	OTAMP4METADATALIB_API uint32_t loadMetadata(const char* input);
	OTAMP4METADATALIB_API uint32_t getBufferSize();
	OTAMP4METADATALIB_API uint32_t peekMetadata(double time, uint8_t*& data);
}