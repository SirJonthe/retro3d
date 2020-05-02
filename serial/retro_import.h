#ifndef RETRO_LOADERS_H
#define RETRO_LOADERS_H

#include <string>
#include "../common/MiniLib/MTL/mtlArray.h"
#include "../common/MiniLib/MTL/mtlStringMap.h"
#include "../common/MiniLib/MML/mmlVector.h"
#include "../api/tiny3d/tiny_image.h"
#include "../api/tiny3d/tiny_texture.h"
#include "../graphics/retro_model.h"
#include "../common/retro_containers.h"
#include "../audio/retro_sound.h"
#include "retro_reader.h"

namespace retro3d
{

std::string GetDirectory(const std::string &path);
std::string GetFileName(const std::string &path);
std::string GetFileNameWithExtension(const std::string &path);
std::string GetFileExtension(const std::string &dir);

void CreateDefaultImage(tiny3d::Image &img);
void CreateDefaultModel(retro3d::Array< mmlVector<3> > *v, retro3d::Array< mmlVector<2> > *t, retro3d::Array< mmlVector<3> > *n, retro3d::Array<retro3d::Material> *m);
void CreateDefaultSound(retro3d::Sound &sfx);

bool FileExists(const std::string &file);

struct MTL_Meta
{
	std::string  name;
	std::string  map_Kd;
	mmlVector<3> Kd;
};

// bool LoadMTL(const std::string &file, MTL_Meta &mtl);

struct OBJ_Meta
{
	struct Object
	{
		std::string name;
		uint32_t    f_count;
	};
	retro3d::Array<Object> o;
	uint32_t               v_count;
	uint32_t               t_count;
	uint32_t               n_count;
	uint32_t               f_count;
};
bool LoadOBJMeta(retro3d::Reader &reader, retro3d::OBJ_Meta &obj);
bool LoadOBJMeta(const std::string &file, retro3d::OBJ_Meta &obj);
bool LoadOBJ(retro3d::Reader &reader, retro3d::Array< mmlVector<3> > *v, retro3d::Array< mmlVector<2> > *t, retro3d::Array< mmlVector<3> > *n, retro3d::Array<retro3d::Material> *m);
bool LoadOBJ(const std::string &file, retro3d::Array< mmlVector<3> > *v, retro3d::Array< mmlVector<2> > *t, retro3d::Array< mmlVector<3> > *n, retro3d::Array<retro3d::Material> *m);
bool SaveOBJ(const std::string &file, const retro3d::Array< mmlVector<3> > &v, const retro3d::Array< mmlVector<2> > *t, const retro3d::Array< mmlVector<3> > *n, const retro3d::Array<retro3d::Material> &m);

bool LoadBMP(const std::string &file, tiny3d::Image &img, bool improve_gradients = true);

//struct TGA_Header { width, height, bpp, etc... };

//bool LoadTGAHeader(const std::string &file, retro3d::TGA_Header &header);
bool LoadTGA(retro3d::Reader &reader, tiny3d::Image &img, bool improve_gradients = true);
bool LoadTGA(const std::string &file, tiny3d::Image &img, bool improve_gradients = true);
bool SaveTGA(const std::string &file, const tiny3d::Image &img);

struct WAV_Header
{
	struct Descriptor
	{
		char     chunk_id[4]; // "RIFF"
		uint32_t chunk_size; // 36 + sub_chunk_1_size, i.e. 4 + (8 + sub_chunk_1_size) + (8 + sub_chunk_2_size)
		char     format[4]; // "WAVE"
	};

	struct Format
	{
		char     sub_chunk_1_id[4]; // "fmt "
		uint32_t sub_chunk_1_size; // 16 = PCM
		uint16_t audio_format; // 1 = PCM, other values = some form of compression
		uint16_t num_channels; // 1 = mono, 2 = stereo, etc.
		uint32_t sample_rate; // 8000, 22050, 44100, etc.
		uint32_t byte_rate; // sample_rate * num_channels * bits_per_sample / 8
		uint16_t block_align; // num_channels * bits_per_sample / 8
		uint16_t bits_per_sample; // 8 = uint8_t, 16 = int16_t
		// NOTE: if 'audio_format' is other than 1, then more data follows here (we only support audio_format=1 though)
	};

	struct Data
	{
		char     sub_chunk_2_id[4]; // "data"
		uint32_t sub_chunk_2_size; // num_samples * num_channels * bits_per_sample / 8
	};

	Descriptor descriptor;
	Format     format;
	Data       data;
};

bool    LoadWAVHeader(retro3d::Reader &reader, retro3d::WAV_Header &header);
int32_t VerifyWAVHeader(const retro3d::WAV_Header &header);
bool    LoadWAVBuffer(retro3d::Reader &reader, retro3d::Sound &sfx_buffer);
bool    LoadWAV(retro3d::Reader &reader, retro3d::Sound &sfx);
bool    LoadWAV(const std::string &file, retro3d::Sound &sfx);

}

#endif // RETRO_LOADERS_H
