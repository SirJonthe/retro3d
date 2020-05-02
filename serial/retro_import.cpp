#include <fstream>
#include <sstream>
#include <list>
#include <iostream>
#include "../common/MiniLib/MTL/mtlParser.h"
#include "../common/retro_assert.h"
#include "retro_import.h"

#ifdef RETRO3D_USE_SDL1
	#include <SDL/SDL.h>
#elif defined(RETRO3D_USE_SDL2)
	#include <SDL2/SDL.h>
#endif

using namespace tiny3d;
using namespace retro3d;

std::string retro3d::GetDirectory(const std::string &path)
{
	size_t dir_sep1 = path.find_last_of('\\');
	size_t dir_sep2 = path.find_last_of('/');
	size_t dir_sep = mmlMax(dir_sep1, dir_sep2);
	if (dir_sep == std::string::npos) {
		dir_sep = mmlMin(dir_sep1, dir_sep2);
	}
	return (dir_sep != std::string::npos) ? path.substr(0, dir_sep + 1) : "";
}

std::string retro3d::GetFileName(const std::string &path)
{
	std::string filename = retro3d::GetFileNameWithExtension(path);
	size_t ext_sep = filename.find_last_of('.');
	if (ext_sep == 0) {
		ext_sep = std::string::npos;
	}
	size_t rem_len = ext_sep != std::string::npos ? ext_sep : filename.size();
	return filename.substr(0, rem_len);
}

std::string retro3d::GetFileNameWithExtension(const std::string &path)
{
	size_t dir_sep1 = path.find_last_of('\\');
	size_t dir_sep2 = path.find_last_of('/');
	size_t dir_sep = mmlMax(dir_sep1, dir_sep2);
	if (dir_sep == std::string::npos) {
		dir_sep = mmlMin(dir_sep1, dir_sep2);
	}
	size_t rem_len = path.size() - (dir_sep + 1);
	return (dir_sep != std::string::npos) ? path.substr(dir_sep + 1, rem_len) : path.substr(0, rem_len);
}

std::string retro3d::GetFileExtension(const std::string &path)
{
	std::string filename = GetFileNameWithExtension(path);
	size_t ext_sep = filename.find_last_of('.');
	if (ext_sep == 0) {
		ext_sep = std::string::npos;
	}
	return (ext_sep != std::string::npos) ? filename.substr(ext_sep + 1, filename.size() - (ext_sep + 1)) : "";
}

void retro3d::CreateDefaultImage(tiny3d::Image &img)
{
	img.Create(tiny3d::Texture::MinDimension());
	for (UInt y = 0; y < img.GetHeight(); ++y) {
		for (UInt x = 0; x < img.GetWidth(); ++x) {
			tiny3d::UPoint p = { x, y };
			img.SetColor(p, ((p.x + p.y) & 1) ? tiny3d::Color{ 255, 0, 255, tiny3d::Color::Solid } : tiny3d::Color{ 128, 128, 128, tiny3d::Color::Solid });
		}
	}
}

void retro3d::CreateDefaultModel(retro3d::Array< mmlVector<3> > *v, retro3d::Array< mmlVector<2> > *t, retro3d::Array< mmlVector<3> > *n, retro3d::Array<retro3d::Material> *m)
{
	if (v != nullptr) {
		v->Create(8);
		(*v)[0] = mmlVector<3>(-0.5,  0.5, -0.5);
		(*v)[1] = mmlVector<3>( 0.5,  0.5, -0.5);
		(*v)[2] = mmlVector<3>( 0.5, -0.5, -0.5);
		(*v)[3] = mmlVector<3>(-0.5, -0.5, -0.5);
		(*v)[4] = mmlVector<3>(-0.5,  0.5,  0.5);
		(*v)[5] = mmlVector<3>( 0.5,  0.5,  0.5);
		(*v)[6] = mmlVector<3>( 0.5, -0.5,  0.5);
		(*v)[7] = mmlVector<3>(-0.5, -0.5,  0.5);
	}

	if (t != nullptr) {
		t->Create(4);
		(*t)[0] = mmlVector<2>(0.0, 1.0);
		(*t)[1] = mmlVector<2>::Fill(1.0);
		(*t)[2] = mmlVector<2>(1.0, 0.0);
		(*t)[3] = mmlVector<2>::Fill(0.0f);
	}

	if (n != nullptr) {
		n->Create(6);
		(*n)[0] = mmlVector<3>( 1.0,  0.0,  0.0);
		(*n)[1] = mmlVector<3>(-1.0,  0.0,  0.0);
		(*n)[2] = mmlVector<3>( 0.0,  1.0,  0.0);
		(*n)[3] = mmlVector<3>( 0.0, -1.0,  0.0);
		(*n)[4] = mmlVector<3>( 0.0,  0.0,  1.0);
		(*n)[5] = mmlVector<3>( 0.0,  0.0, -1.0);
	}

	if (m != nullptr) {
		m->Create(1);

		(*m)[0].name = "Material";
		(*m)[0].cd = mmlVector<3>::Fill(1.0f);
		tiny3d::Image img;
		CreateDefaultImage(img);
		(*m)[0].td.New();
		(*m)[0].td->FromImage(img);
		(*m)[0].f.Create(6);
		(*m)[0].f[0].Create(4);
		(*m)[0].f[1].Create(4);
		(*m)[0].f[2].Create(4);
		(*m)[0].f[3].Create(4);
		(*m)[0].f[4].Create(4);
		(*m)[0].f[5].Create(4);

		(*m)[0].f[0][0] = retro3d::IndexVTN{ 1, 0, 0 };
		(*m)[0].f[0][1] = retro3d::IndexVTN{ 5, 1, 0 };
		(*m)[0].f[0][2] = retro3d::IndexVTN{ 6, 2, 0 };
		(*m)[0].f[0][3] = retro3d::IndexVTN{ 2, 3, 0 };

		(*m)[0].f[1][0] = retro3d::IndexVTN{ 3, 0, 1 };
		(*m)[0].f[1][1] = retro3d::IndexVTN{ 7, 1, 1 };
		(*m)[0].f[1][2] = retro3d::IndexVTN{ 4, 2, 1 };
		(*m)[0].f[1][3] = retro3d::IndexVTN{ 0, 3, 1 };

		(*m)[0].f[2][0] = retro3d::IndexVTN{ 0, 0, 2 };
		(*m)[0].f[2][1] = retro3d::IndexVTN{ 4, 1, 2 };
		(*m)[0].f[2][2] = retro3d::IndexVTN{ 5, 2, 2 };
		(*m)[0].f[2][3] = retro3d::IndexVTN{ 1, 3, 2 };

		(*m)[0].f[3][0] = retro3d::IndexVTN{ 2, 0, 3 };
		(*m)[0].f[3][1] = retro3d::IndexVTN{ 6, 1, 3 };
		(*m)[0].f[3][2] = retro3d::IndexVTN{ 7, 2, 3 };
		(*m)[0].f[3][3] = retro3d::IndexVTN{ 3, 3, 3 };

		(*m)[0].f[4][0] = retro3d::IndexVTN{ 5, 0, 4 };
		(*m)[0].f[4][1] = retro3d::IndexVTN{ 4, 1, 4 };
		(*m)[0].f[4][2] = retro3d::IndexVTN{ 7, 2, 4 };
		(*m)[0].f[4][3] = retro3d::IndexVTN{ 6, 3, 4 };

		(*m)[0].f[5][0] = retro3d::IndexVTN{ 0, 0, 5 };
		(*m)[0].f[5][1] = retro3d::IndexVTN{ 1, 1, 5 };
		(*m)[0].f[5][2] = retro3d::IndexVTN{ 2, 2, 5 };
		(*m)[0].f[5][3] = retro3d::IndexVTN{ 3, 3, 5 };
	}
}

void retro3d::CreateDefaultSound(retro3d::Sound &sfx)
{
	sfx.CreateDuration(1.0, retro3d::Sound::Format{ retro3d::Sound::Channels_Mono, 8000, retro3d::Sound::SampleFormat_UINT8 });
	// NOTE: C note - 261.63 Hz - We need 261.63 revolutions on the sine wave per second (i.e. the duration of the sample).
	const uint32_t NUM_SAMPLES = sfx.GetSampleCount();
	constexpr float Hz = 261.63f;
	constexpr float RAD = 2.0f * mmlPI * Hz;
	const float INC = RAD / NUM_SAMPLES;
	float r = 0.0f;
	for (uint32_t i = 0; i < NUM_SAMPLES; ++i) {
		retro3d::Sound::Sample s;
		s.channel.left = s.channel.right = mmlSin(r);
		sfx.SetSample(i, s);
		r += INC;
	}
}

bool retro3d::FileExists(const std::string &file)
{
	retro3d::Reader reader;
	reader.ReadFromFile(file);
	return reader.IsOpen();
}

bool retro3d::LoadOBJMeta(retro3d::Reader &reader, retro3d::OBJ_Meta &obj)
{
	retro3d::OBJ_Meta::Object *o = nullptr;
	obj.o.Free();
	obj.v_count = 0;
	obj.t_count = 0;
	obj.n_count = 0;
	obj.f_count = 0;

	if (reader.IsOpen() == false) {
		std::cout << "[LoadOBJMeta] Could not read, ";
		return false;
	}

	std::string str;
	while (reader.IsEnd() == false) {
		reader.ReadWord(str);
		if (str == "v") {
			++obj.v_count;
		}

		else if (str == "t") {
			++obj.t_count;
		}

		else if (str == "n") {
			++obj.n_count;
		}

		else if (str == "o") {
			o = &obj.o.Add();
			reader.ReadWord(o->name);
			o->f_count = 0;
		}

		else if (str == "f") {
			if (o == nullptr) {
				o = &obj.o.Add();
			}
			++o->f_count;
			++obj.f_count;
		}

		reader.ReadLine(str);
	}

	return true;
}

bool retro3d::LoadOBJMeta(const std::string &file, retro3d::OBJ_Meta &o)
{
	retro3d::Reader reader;
	reader.ReadFromFile(file);
	return retro3d::LoadOBJMeta(reader, o);
}

float ReadF(std::ifstream &fin)
{
	std::string in;
	if (!(fin >> in)) {
		in = 0.0f;
	}
	return std::stof(in);
}

FaceIndex ReadI(retro3d::Reader &reader)
{
	std::string line, arg;
	reader.ReadLine(line);
	std::istringstream sin(line);

	UInt i_count = 0;
	while (sin >> arg) {
		++i_count;
	}

	FaceIndex f;
	f.Create(int(i_count));
	i_count = 0;
	sin = std::istringstream(line);
	while (sin >> arg) {
		std::string v, t, n;
		size_t a = arg.find_first_of('/');
		size_t b = arg.find_last_of('/');
		v = arg.substr(0, a);
		t = (a < std::string::npos && b - a - 1 > 0) ? arg.substr(a + 1, b - a - 1) : "0";
		n = (b < std::string::npos) ? arg.substr(b + 1) : "0";

		f[i_count++] = {
			SInt(std::stol(v)) - 1,
			SInt(std::stol(t)) - 1,
			SInt(std::stol(n)) - 1
		};
	}

	return f;
}

template < typename type_t >
void ListToArray(const std::list<type_t> &l, retro3d::Array<type_t> &a)
{
	a.Create(int(l.size()));
	auto it = l.begin();
	UInt i = 0;
	while (it != l.end()) { a[i++] = *(it++); }
}

struct MDef
{
	std::string          name;
	std::list<FaceIndex> f;
	mmlVector<3>         cd;
	std::string          td_file;
};

bool LoadMTL(const std::string &file, std::list<MDef> &m)
{
	size_t dir_sep1 = file.find_last_of('\\');
	size_t dir_sep2 = file.find_last_of('/');
	size_t dir_sep = mmlMax(dir_sep1, dir_sep2);
	if (dir_sep == std::string::npos) {
		dir_sep = mmlMin(dir_sep1, dir_sep2);
	}
	const std::string work_dir = (dir_sep != std::string::npos) ? file.substr(0, dir_sep + 1) : "";

	bool ret_val = true;

	std::ifstream fin;
	fin.open(file.c_str());
	if (!fin.is_open()) {
		// NOTE: Load a box with default texture.
		return false;
	}
	std::string in;
	while (fin >> in) {
		if (in == "newmtl") {
			m.push_back(MDef());
			fin >> m.back().name;
			m.back().cd = mmlVector<3>::Fill(1.0f);
		} else if (in == "map_Kd") {
			std::getline(fin, in);
			in = in.substr(in.find_first_not_of(' '), in.find_last_not_of(' '));
			m.back().td_file = work_dir + in;
		} else if (in == "Kd") {
			double r = double(ReadF(fin));
			double g = double(ReadF(fin));
			double b = double(ReadF(fin));
			m.back().cd = mmlVector<3>(r, g, b);
		} else if (in == "Ns" || in == "Ni" || in == "d" || in == "illum") {
			ReadF(fin);
		} else if (in == "Ka" || in == "Ks" || in == "Ke") {
			ReadF(fin);
			ReadF(fin);
			ReadF(fin);
		} else if (in[0] == '#') {
			std::getline(fin, in);
		} else {
			std::cout << "[LoadMTL] Bad argument in MTL: \"" << in << "\", ";
			std::getline(fin, in); // NOTE: Skip the line.
			ret_val = false;
		}
	}
	return ret_val;
}

std::list<FaceIndex> *FindMaterial(const std::string &name, std::list<MDef> &m)
{
	auto i = m.begin();
	++i; // NOTE: Skip over error material so that it can not accidentally be accessed by a valid OBJ file.
	for (; i != m.end(); ++i) {
		if (i->name == name) { return &(i->f); }
	}
	return nullptr;
}

bool retro3d::LoadOBJ(retro3d::Reader &reader, retro3d::Array< mmlVector<3> > *v, retro3d::Array< mmlVector<2> > *t, retro3d::Array< mmlVector<3> > *n, retro3d::Array<retro3d::Material> *m)
{
	const std::string work_dir = reader.GetPath();

	std::list< mmlVector<3> >  vl;
	std::list< mmlVector<2> >  tl;
	std::list< mmlVector<3> >  nl;
	std::list<MDef>            ml;
	std::list<FaceIndex>      *fl = nullptr;

	if (m != nullptr) {
		// NOTE: Generate an error material.
		ml.push_back(MDef());
		ml.back().cd = mmlVector<3>::Fill(1.0f);
		fl = &ml.back().f;
	}

	bool ret_val = true;

	if (reader.IsOpen() == false) {
		std::cout << "[LoadOBJ] Could not open (return default), ";
		CreateDefaultModel(v, t, n, m);
		return false;
	}
	std::string in;
	while (reader.ReadWord(in) == true) {
		if (in == "v") {
			mmlVector<3> a;
			for (int i = 0; i < a.Dimension; ++i) { reader.Read<float>(a[i]); }
			if (v) { vl.push_back(a); }
		} else if (in == "vn") {
			mmlVector<3> a;
			for (int i = 0; i < a.Dimension; ++i) { reader.Read<float>(a[i]); }
			if (n) { nl.push_back(a); }
		} else if (in == "vt") {
			mmlVector<2> a;
			for (int i = 0; i < a.Dimension; ++i) { reader.Read<float>(a[i]); }
			if (t) { tl.push_back(a); }
		} else if (in == "f") {
			FaceIndex a = ReadI(reader);
			for (int i = 0; i < a.GetSize(); ++i) { // NOTE: Handle negative indices as relative offsets.
				if (a[i].v < 0) {
					if (v) { a[i].v = int32_t(vl.size()) - a[i].v; }
					else   { a[i].v = 0; }
				}
				if (a[i].t < 0) {
					if (t) { a[i].t = int32_t(tl.size()) - a[i].t; }
					else   { a[i].t = 0; }
				}
				if (a[i].n < 0) {
					if (n) { a[i].n = int32_t(nl.size()) - a[i].n; }
					else   { a[i].n = 0; }
				}
			}
			if (m && fl) { fl->push_back(a); }
		} else if (in == "usemtl") {
			reader.ReadWord(in);
			if (m) {
				fl = FindMaterial(in, ml);
				if (fl == nullptr) {
					std::cout << "[LoadOBJ] Bad material in OBJ: \"" << in << "\" (default error material), ";
					fl = &ml.begin()->f;
					ret_val = false;
				}
			}
		} else if (in == "mtllib") {
			reader.ReadLine(in);
			in = in.substr(in.find_first_not_of(' '), in.find_last_not_of(' '));
			if (m) {
				if (LoadMTL(work_dir + in, ml) == false) {
					ret_val = false;
				}
			}
		} else if (in == "o") {
			reader.ReadWord(in);
		} else if (in == "g") {
			reader.ReadWord(in);
		} else if (in == "s") {
			reader.ReadWord(in);
		} else if (in[0] == '#') {
			reader.ReadLine(in);
		} else {
			std::cout << "[LoadOBJ] Bad argument in OBJ: \"" << in << "\", ";
			reader.ReadLine(in); // NOTE: Skip the offending line.
			ret_val = false;
		}
	}

	if (v) {
		ListToArray(vl, *v);
	}
	if (t) { ListToArray(tl, *t); }
	if (n) { ListToArray(nl, *n); }
	if (m) {
		m->Create(int(ml.size()));

		tiny3d::Image texture_source;
		auto mit = ml.begin();
		UInt mi = 0;
		retro3d::Material *material = &(*m)[mi];

		material->name = "ERROR";
		material->cd = mmlVector<3>::Fill(1.0f);
		CreateDefaultImage(texture_source);
		material->td.New();
		material->td->FromImage(texture_source);
		material->f.Create(int(mit->f.size()));
		UInt fi = 0;
		for (auto fit = mit->f.begin(); fit != mit->f.end(); ++fit) {
			material->f[fi] = *fit;
			++fi;
		}

		++mit;
		++mi;
		for (; mit != ml.end(); ++mit) {
			material = &(*m)[mi];
			material->name = mit->name;
			material->cd = mit->cd;

			if (mit->td_file != "") { // load file

				mtlChars texture_file_name = mtlChars::FromDynamic(mit->td_file.c_str(), int(mit->td_file.size()));
				if (retro3d::Texture::Library.Exists(texture_file_name, material->td) == false) {
					// Load the file
					size_t i = mit->td_file.find_last_of('.');
					std::string ext = mit->td_file.substr(i, mit->td_file.size() - i);
					if (ext == ".bmp") {
						if (retro3d::LoadBMP(mit->td_file, texture_source) == false) { ret_val = false; }
					} else if (ext == ".tga") {
						if (retro3d::LoadTGA(mit->td_file, texture_source) == false) { ret_val = false; }
					} else {
						std::cout << "[LoadOBJ] Unknown texture extension: \"" << ext << "\", ";
						ret_val = false;
					}
					texture_source.SetColorKey(tiny3d::Color{ 255, 0, 255, tiny3d::Color::Solid });
					material->td.New();
					material->td->FromImage(texture_source);
					retro3d::Texture::Library.Store(material->td, texture_file_name);
				}

			} else { // NOTE: Just load a white texture, keep as a separate copy.
				texture_source.Create(tiny3d::Texture::MinDimension());
				texture_source.Fill(tiny3d::Color{255,255,255,tiny3d::Color::Solid});
				material->td.New();
				material->td->FromImage(texture_source);
			}

			material->f.Create(int(mit->f.size()));
			fi = 0;
			for (auto fit = mit->f.begin(); fit != mit->f.end(); ++fit) {
				material->f[fi] = *fit;
				++fi;
			}
			++mi;
		}
	}

	return ret_val;
}

bool retro3d::LoadOBJ(const std::string &file, retro3d::Array< mmlVector<3> > *v, retro3d::Array< mmlVector<2> > *t, retro3d::Array< mmlVector<3> > *n, retro3d::Array<retro3d::Material> *m)
{
	retro3d::Reader reader;
	reader.ReadFromFile(file);
	return retro3d::LoadOBJ(reader, v, t, n, m);
}

bool retro3d::SaveOBJ(const std::string &file, const retro3d::Array< mmlVector<3> > &v, const retro3d::Array< mmlVector<2> > *t, const retro3d::Array< mmlVector<3> > *n, const retro3d::Array<retro3d::Material> &m)
{
	const std::string path = retro3d::GetDirectory(file);
	const std::string filename = retro3d::GetFileName(file);

	// Write the OBJ file
	std::ofstream fout(file.c_str());
	if (!fout.is_open()) {
		std::cout << "[SaveOBJ] Could not create/open OBJ file, ";
		return false;
	}

	fout << "mtllib " << filename + ".mtl" << std::endl;

	for (int32_t i = 0; i < v.GetSize(); ++i) {
		fout << "v  " << v[i][0] << " " << v[i][1] << " " << v[i][2] << std::endl;
	}
	if (t != nullptr) {
		fout << std::endl;
		for (int32_t i = 0; i < t->GetSize(); ++i) {
			fout << "vt " << (*t)[i][0] << " " << (*t)[i][1] << std::endl;
		}
	}
	if (n != nullptr) {
		fout << std::endl;
		for (int32_t i = 0; i < n->GetSize(); ++i) {
			fout << "vn " << (*n)[i][0] << " " << (*n)[i][1] << " " << (*n)[i][2] << std::endl;
		}
	}
	fout << std::endl;

	for (int32_t i = 0; i < m.GetSize(); ++i) {
		fout << "usemtl " << m[i].name << std::endl;
		for (int32_t j = 0; j < m[i].f.GetSize(); ++j) {
			fout << "f ";
			for (int32_t k = 0; k < m[i].f[j].GetSize(); ++k) {
				fout << m[i].f[j][k].v + 1;
				if ((t != nullptr && m[i].f[j][k].t >= 0) && (n != nullptr && m[i].f[j][k].n >= 0)) {
					fout << "/" << m[i].f[j][k].t + 1;
					fout << "/" << m[i].f[j][k].n + 1;
				} else if (t != nullptr && m[i].f[j][k].t >= 0) {
					fout << "/" << m[i].f[j][k].t + 1;
				} else if (n != nullptr && m[i].f[j][k].n >= 0) {
					fout << "//" << m[i].f[j][k].n + 1;
				}
				fout << " ";
			}
			fout << std::endl;
		}
		fout << std::endl;
	}

	fout.close();

	// Write the MTL file
	fout.open(path + filename + ".mtl");
	if (!fout.is_open()) {
		std::cout << "[SaveOBJ] Could not create/open MTL file, ";
		return false;
	}

	for (int32_t i = 0; i < m.GetSize(); ++i) {
		fout << "newmtl " << m[i].name << std::endl;
		fout << "Kd " << m[i].cd[0] << " " << m[i].cd[1] << " " << m[i].cd[2] << std::endl;
		if (m[i].td.IsNull() == false) {
			fout << "map_Kd " << m[i].name << "_diffuse.tga" << std::endl;
		}
		fout << std::endl;
	}
	fout.close();

	// Write TGA files
	bool tga_write_status = true;
	for (int32_t i = 0; i < m.GetSize(); ++i) {
		if (m[i].td.IsNull() == false) {
			tiny3d::Image img;
			m[i].td.GetShared()->GetHighestQuality()->ToImage(img);
			if (retro3d::SaveTGA(path + m[i].name + "_diffuse.tga", img) == false) {
				tga_write_status = false;
			}
		}
	}

	return tga_write_status;
}

// http://sdl.beuc.net/sdl.wiki/Pixel_Access
Uint32 GetPixel(SDL_Surface *surface, UInt x, UInt y)
{
	UInt bpp = UInt(surface->format->BytesPerPixel);
	Uint8 *p = reinterpret_cast<Uint8*>(surface->pixels) + y * surface->pitch + x * bpp;
	switch(bpp) {
	case 1: return *p;
	case 2: return *reinterpret_cast<Uint16*>(p);
	case 3:
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			return Uint32((p[0] << 16) | (p[1] << 8) | p[2]);
		} else {
			return Uint32(p[0] | (p[1] << 8) | (p[2] << 16));
		}
	case 4: return *reinterpret_cast<Uint32*>(p);
	default: break;
	}
	return 0;
}

bool retro3d::LoadBMP(const std::string &file, tiny3d::Image &img, bool improve_gradients)
{
	SDL_Surface *surf = SDL_LoadBMP(file.c_str());
	if (surf == nullptr || surf->w > int(Image::MaxDimension()) || surf->h > int(Image::MaxDimension()) || !img.Create(UHInt(surf->w), UHInt(surf->h))) {
		CreateDefaultImage(img);
		std::cout << "[LoadBMP] Bad file in BMP (return default). (SDL: " << SDL_GetError() << "), ";
		return false;
	}
	for (UInt y = 0; y < img.GetWidth(); ++y) {
		for (UInt x = 0; x < img.GetHeight(); ++x) {
			Color c;
			SDL_GetRGBA(GetPixel(surf, x, y), surf->format, &c.r, &c.g, &c.b, &c.blend);
			c.blend = c.blend < 128 ? Color::Transparent : Color::Solid;
			if (improve_gradients == true) {
				img.SetColor(UPoint{ x, y }, tiny3d::Dither2x2(c, UPoint{ x, y }));
			} else {
				img.SetColor(UPoint{ x, y }, c);
			}
		}
	}
	SDL_FreeSurface(surf);
	return true;
}

enum TGA_RelevantHeaderField
{
	TGA_COLOR_MAP_TYPE = 1,
	TGA_IMAGE_TYPE     = 2,
	TGA_IMAGE_WIDTH    = 12,
	TGA_IMAGE_HEIGHT   = 14,
	TGA_PIXEL_DEPTH    = 16,
	TGA_HEADER_SIZE    = 18
};
enum TGA_SupportedColorMapType
{
	TGA_NO_COLOR_MAP
};
enum TGA_SupportedTextureType
{
	TGA_UNCOMPRESSED_TRUECOLOR_IMAGE = 2,
	TGA_COMPRESSED_TRUECOLOR_IMAGE   = 10
};
enum TGA_SupportedPixelDepth
{
	TGA_BGR888   = 24,
	TGA_BGRA8888 = 32
};

bool retro3d::LoadTGA(retro3d::Reader &reader, tiny3d::Image &img, bool improve_gradients)
{
	uint8_t m_header[TGA_HEADER_SIZE];

	if (reader.IsOpen() == false) {
		std::cout << "[LoadTGA] Could not open file (return default), ";
		CreateDefaultImage(img);
		return false;
	}

	if (reader.ReadBin(reinterpret_cast<char*>(m_header), TGA_HEADER_SIZE) < TGA_HEADER_SIZE) {
		std::cout << "[LoadTGA] Failed to read (1) (return default), ";
		CreateDefaultImage(img);
		return false;
	}

	if (m_header[TGA_COLOR_MAP_TYPE] != TGA_NO_COLOR_MAP) {
		std::cout << "[LoadTGA] TGA has color map (return default), ";
		CreateDefaultImage(img);
		return false;
	}

	uint32_t bytes_per_pixel = 0;
	if (m_header[TGA_PIXEL_DEPTH] == TGA_BGR888) {
		bytes_per_pixel = 3;
	} else if (m_header[TGA_PIXEL_DEPTH] == TGA_BGRA8888) {
		bytes_per_pixel = 4;
	} else {
		std::cout << "[LoadTGA] Pixel depth not BGR888 or BGRA8888 (return default), ";
		CreateDefaultImage(img);
		return false;
	}

	uint32_t          width     = uint32_t(*reinterpret_cast<int16_t*>(m_header + TGA_IMAGE_WIDTH));
	uint32_t          height    = uint32_t(*reinterpret_cast<int16_t*>(m_header + TGA_IMAGE_HEIGHT));
	uint32_t          NUM_BYTES = width * height * bytes_per_pixel;
	mtlArray<uint8_t> pixels    = mtlArray<uint8_t>(int(NUM_BYTES));

	if (m_header[TGA_IMAGE_TYPE] == TGA_UNCOMPRESSED_TRUECOLOR_IMAGE) {

		if (img.Create(width, height) == false) {
			std::cout << "[LoadTGA] Failed to create image (2) (return default), ";
			CreateDefaultImage(img);
			return false;
		}

		if (reader.ReadBin(reinterpret_cast<char*>(&pixels[0]), NUM_BYTES) == NUM_BYTES) { // NOTE: Inputs BGR(A).
			for (uint32_t i = 0; i < NUM_BYTES; i += bytes_per_pixel) {
				mmlSwap(pixels[i], pixels[i+2]); // to RGB(A)
			}
		} else {
			std::cout << "[LoadTGA] Failed to read (3) (return default), ";
			CreateDefaultImage(img);
			return false;
		}

	} else if (m_header[TGA_IMAGE_TYPE] == TGA_COMPRESSED_TRUECOLOR_IMAGE) {

		if (img.Create(width, height) == false) {
			std::cout << "[LoadTGA] Failed to create image (return default), ";
			CreateDefaultImage(img);
			return false;
		}

		uint8_t chunkSize;
		uint32_t currentByte = 0;
		while (currentByte < NUM_BYTES) {
			reader.ReadBin(reinterpret_cast<char*>(&chunkSize), 1);
			if (chunkSize < 128) {
				++chunkSize;
				for (uint32_t c = 0; c < uint32_t(chunkSize); ++c) {
					if (reader.ReadBin(reinterpret_cast<char*>(&pixels[currentByte]), bytes_per_pixel) < bytes_per_pixel) {
						std::cout << "[LoadTGA] Failed to read (4) (return default), ";
						CreateDefaultImage(img);
						return false;
					}
					mmlSwap(pixels[currentByte], pixels[currentByte + 2]); // NOTE: To RGB(A).
					currentByte += bytes_per_pixel;
				}
			} else {
				chunkSize -= 127;
				if (reader.ReadBin(reinterpret_cast<char*>(&pixels[currentByte]), bytes_per_pixel) < bytes_per_pixel) {
					std::cout << "[LoadTGA] Failed to read (5) (return default), ";
					CreateDefaultImage(img);
					return false;
				}
				uint8_t *copyPixel = &pixels[currentByte];
				mmlSwap(copyPixel[0], copyPixel[2]);
				currentByte += bytes_per_pixel;

				for (int i = 0; i < chunkSize-1; ++i) {
					for (uint32_t j = 0; j < bytes_per_pixel; ++j) {
						pixels[currentByte+j] = copyPixel[j];
					}
					currentByte += bytes_per_pixel;
				}
			}
		}
	} else {
		std::cout << "[LoadTGA] IMAGE_TYPE is not UNCOMPRESSED_TRUECOLOR_IMAGE or COMPRESSED_TRUECOLOR_IMAGE (return default), ";
		CreateDefaultImage(img);
		return false;
	}

	for (tiny3d::UInt y = 0; y < img.GetHeight(); ++y) {
		for (tiny3d::UInt x = 0; x < img.GetWidth(); ++x) {
			uint32_t i = (y * img.GetWidth() + x) * bytes_per_pixel;
			tiny3d::Color c;
			c.r = pixels[i+0];
			c.g = pixels[i+1];
			c.b = pixels[i+2];
			c.blend = bytes_per_pixel == 4 && pixels[i+3] < 128 ? tiny3d::Color::Transparent : tiny3d::Color::Solid;
			tiny3d::UPoint p = { x, y };
			if (improve_gradients == true) {
				img.SetColor(p, tiny3d::Dither2x2(c, p));
			} else {
				img.SetColor(p, c);
			}
		}
	}

	return true;
}

bool retro3d::LoadTGA(const std::string &file, tiny3d::Image &img, bool improve_gradients)
{
	retro3d::Reader reader;
	reader.ReadFromFile(file);
	return retro3d::LoadTGA(reader, img, improve_gradients);
}

/*bool retro3d::LoadTGA(const std::string &file, tiny3d::Image &img, bool improve_gradients)
{
	uint8_t m_header[TGA_HEADER_SIZE];

	std::ifstream fin(file.c_str(), std::ios::binary);
	if (!fin.is_open()) {
		std::cout << "[LoadTGA] Could not open file (return default), ";
		CreateDefaultImage(img);
		return false;
	}

	fin.read(reinterpret_cast<char*>(m_header), TGA_HEADER_SIZE);
	if (fin.fail()) {
		std::cout << "[LoadTGA] Failed to read (1) (return default), ";
		CreateDefaultImage(img);
		return false;
	}

	if (m_header[TGA_COLOR_MAP_TYPE] != TGA_NO_COLOR_MAP) {
		std::cout << "[LoadTGA] TGA has color map (return default), ";
		CreateDefaultImage(img);
		return false;
	}

	uint32_t bytes_per_pixel = 0;
	if (m_header[TGA_PIXEL_DEPTH] == TGA_BGR888) {
		bytes_per_pixel = 3;
	} else if (m_header[TGA_PIXEL_DEPTH] == TGA_BGRA8888) {
		bytes_per_pixel = 4;
	} else {
		std::cout << "[LoadTGA] Pixel depth not BGR888 or BGRA8888 (return default), ";
		CreateDefaultImage(img);
		return false;
	}

	uint32_t          width     = uint32_t(*reinterpret_cast<int16_t*>(m_header + TGA_IMAGE_WIDTH));
	uint32_t          height    = uint32_t(*reinterpret_cast<int16_t*>(m_header + TGA_IMAGE_HEIGHT));
	uint32_t          NUM_BYTES = width * height * bytes_per_pixel;
	mtlArray<uint8_t> pixels    = mtlArray<uint8_t>(int(NUM_BYTES));

	if (m_header[TGA_IMAGE_TYPE] == TGA_UNCOMPRESSED_TRUECOLOR_IMAGE) {

		if (img.Create(width, height) == false) {
			std::cout << "[LoadTGA] Failed to create image (2) (return default), ";
			CreateDefaultImage(img);
			return false;
		}

		if (!fin.read(reinterpret_cast<char*>(&pixels[0]), NUM_BYTES).fail()) { // inputs BGR(A)
			for (uint32_t i = 0; i < NUM_BYTES; i += bytes_per_pixel) {
				mmlSwap(pixels[i], pixels[i+2]); // to RGB(A)
			}
		} else {
			std::cout << "[LoadTGA] Failed to read (3) (return default), ";
			CreateDefaultImage(img);
			return false;
		}

	} else if (m_header[TGA_IMAGE_TYPE] == TGA_COMPRESSED_TRUECOLOR_IMAGE) {

		if (img.Create(width, height) == false) {
			std::cout << "[LoadTGA] Failed to create image (return default), ";
			CreateDefaultImage(img);
			return false;
		}

		uint8_t chunkSize;
		uint32_t currentByte = 0;
		while (currentByte < NUM_BYTES) {
			fin.read(reinterpret_cast<char*>(&chunkSize), 1);
			if (chunkSize < 128) {
				++chunkSize;
				for (uint32_t c = 0; c < uint32_t(chunkSize); ++c) {
					fin.read(reinterpret_cast<char*>(&pixels[currentByte]), bytes_per_pixel);
					if (fin.fail()) {
						std::cout << "[LoadTGA] Failed to read (4) (return default), ";
						CreateDefaultImage(img);
						return false;
					}
					mmlSwap(pixels[currentByte], pixels[currentByte + 2]); // to RGB(A)
					currentByte += bytes_per_pixel;
				}
			} else {
				chunkSize -= 127;
				fin.read(reinterpret_cast<char*>(&pixels[currentByte]), bytes_per_pixel);
				if (fin.fail()) {
					std::cout << "[LoadTGA] Failed to read (5) (return default), ";
					CreateDefaultImage(img);
					return false;
				}
				uint8_t *copyPixel = &pixels[currentByte];
				mmlSwap(copyPixel[0], copyPixel[2]);
				currentByte += bytes_per_pixel;

				for (int i = 0; i < chunkSize-1; ++i) {
					for (uint32_t j = 0; j < bytes_per_pixel; ++j) {
						pixels[currentByte+j] = copyPixel[j];
					}
					currentByte += bytes_per_pixel;
				}
			}
		}
	} else {
		std::cout << "[LoadTGA] IMAGE_TYPE is not UNCOMPRESSED_TRUECOLOR_IMAGE or COMPRESSED_TRUECOLOR_IMAGE (return default), ";
		CreateDefaultImage(img);
		return false;
	}

	for (tiny3d::UInt y = 0; y < img.GetHeight(); ++y) {
		for (tiny3d::UInt x = 0; x < img.GetWidth(); ++x) {
			uint32_t i = (y * img.GetWidth() + x) * bytes_per_pixel;
			tiny3d::Color c;
			c.r = pixels[i+0];
			c.g = pixels[i+1];
			c.b = pixels[i+2];
			c.blend = bytes_per_pixel == 4 && pixels[i+3] < 128 ? tiny3d::Color::Transparent : tiny3d::Color::Solid;
			tiny3d::UPoint p = { x, y };
			if (improve_gradients == true) {
				img.SetColor(p, tiny3d::Dither2x2(c, p));
			} else {
				img.SetColor(p, c);
			}
		}
	}

	return true;
}*/

bool retro3d::SaveTGA(const std::string &file, const tiny3d::Image &img)
{
	uint8_t m_header[TGA_HEADER_SIZE];
	for (int i = 0; i < TGA_HEADER_SIZE; ++i) {
		m_header[i] = 0;
	}
	m_header[TGA_COLOR_MAP_TYPE]                             = TGA_NO_COLOR_MAP;
	m_header[TGA_IMAGE_TYPE]                                 = TGA_UNCOMPRESSED_TRUECOLOR_IMAGE;
	*reinterpret_cast<int16_t*>(&m_header[TGA_IMAGE_WIDTH])  = int16_t(img.GetWidth());
	*reinterpret_cast<int16_t*>(&m_header[TGA_IMAGE_HEIGHT]) = int16_t(img.GetHeight());
	m_header[TGA_PIXEL_DEPTH]                                = TGA_BGRA8888;

	std::ofstream fout(file.c_str(), std::ios::binary);
	if (!fout.is_open()) {
		std::cout << "[SaveTGA] Could not open file, ";
		return false;
	}
	fout.write(reinterpret_cast<char*>(m_header), TGA_HEADER_SIZE);
	if (fout.fail()) {
		std::cout << "[SaveTGA] Failed to write (1), ";
		return false;
	}
	for (tiny3d::UInt y = 0; y < img.GetHeight(); ++y) {
		for (tiny3d::UInt x = 0; x < img.GetWidth(); ++x) {
			tiny3d::Color c = img.GetColor(tiny3d::UPoint{ x, y });
			uint8_t rgba[4] = { c.b, c.g, c.r, c.blend == tiny3d::Color::Solid ? uint8_t(255) : uint8_t(0) };
			fout.write(reinterpret_cast<char*>(rgba), 4);
		}
	}
	if (fout.fail()) {
		std::cout << "[SaveTGA] Failed to write (2), ";
		return false;
	}

	return true;
}

float EncodeU8(uint8_t channel)
{
	float f32 = (channel / float(std::numeric_limits<uint8_t>::max())) * 2.0f - 1.0f;
	return f32;
}

float EncodeI16(int16_t channel)
{
	float f32 = (channel < 0 ? (channel / mmlAbs(float(std::numeric_limits<int16_t>::min()))) : (channel / float(std::numeric_limits<int16_t>::max())));
	return f32;
}

bool retro3d::LoadWAVHeader(retro3d::Reader &reader, retro3d::WAV_Header &header)
{
	const uint64_t read_bytes = sizeof(header);
	if (reader.ReadBin(reinterpret_cast<char*>(&header), read_bytes) < read_bytes) {
		std::cout << "[LoadWAVHeader] Could not read, ";
		return false;
	}
	return true;
}

void Debug_PrintWAVHeader(const retro3d::WAV_Header &header)
{
#ifdef RETRO3D_DEBUG
	std::cout << "--WAV Header--" << std::endl;
	std::cout << "  [Descriptor]" << std::endl;
	std::cout << "    chunk_id=" << std::string(header.descriptor.chunk_id, sizeof(header.descriptor.chunk_id)) << std::endl;
	std::cout << "    chunk_size=" << header.descriptor.chunk_size << std::endl;
	std::cout << "    format=" << std::string(header.descriptor.format, sizeof(header.descriptor.format)) << std::endl;

	std::cout << "  [Format]" << std::endl;
	std::cout << "    sub_chunk_1_id=" << std::string(header.format.sub_chunk_1_id, sizeof(header.format.sub_chunk_1_id)) << std::endl;
	std::cout << "    sub_chunk_1_size=" << header.format.sub_chunk_1_size << std::endl;
	std::cout << "    audio_format=" << header.format.audio_format << std::endl;
	std::cout << "    num_channels=" << header.format.num_channels << std::endl;
	std::cout << "    sample_rate=" << header.format.sample_rate << std::endl;
	std::cout << "    byte_rate=" << header.format.byte_rate << std::endl;
	std::cout << "    block_align=" << header.format.block_align << std::endl;
	std::cout << "    bits_per_sample=" << header.format.bits_per_sample << std::endl;

	std::cout << "  [Data]" << std::endl;
	std::cout << "    sub_chunk_2_id=" << std::string(header.data.sub_chunk_2_id, sizeof(header.data.sub_chunk_2_id)) << std::endl;
	std::cout << "    sub_chunk_2_size=" << header.data.sub_chunk_2_size << std::endl;
#endif
}

int32_t retro3d::VerifyWAVHeader(const retro3d::WAV_Header &header)
{
	if (
//		header.descriptor.chunk_size != 4 + (8 + header.format.sub_chunk_1_size) + (8 + header.data.sub_chunk_2_size) || // I don't think checking this serves any purpose even if it does not add up
		header.format.byte_rate != header.format.sample_rate * header.format.num_channels * (header.format.bits_per_sample / 8) ||
		header.format.block_align != header.format.num_channels * (header.format.bits_per_sample / 8)
		) {
		Debug_PrintWAVHeader(header);
		return 1;
	}

	if (
		std::string(header.descriptor.chunk_id, sizeof(header.descriptor.chunk_id)) != "RIFF" ||
		std::string(header.descriptor.format, sizeof(header.descriptor.format)) != "WAVE" ||
		std::string(header.format.sub_chunk_1_id, sizeof(header.format.sub_chunk_1_id)) != "fmt " ||
		header.format.sub_chunk_1_size != 16 ||
		header.format.audio_format != 1 ||
		(header.format.num_channels != 1 && header.format.num_channels != 2) ||
		(header.format.bits_per_sample != 8 && header.format.bits_per_sample != 16) ||
		std::string(header.data.sub_chunk_2_id, sizeof(header.data.sub_chunk_2_id)) != "data"
		) {
		Debug_PrintWAVHeader(header);
		return 2;
	}

	if (header.data.sub_chunk_2_size > uint32_t(std::numeric_limits<int>::max())) {
		Debug_PrintWAVHeader(header);
		return 3;
	}
	return 0;
}

bool retro3d::LoadWAVBuffer(retro3d::Reader &reader, retro3d::Sound &sfx_buffer)
{
	const uint64_t buffer_size = sfx_buffer.GetRawByteCount();
	const uint64_t read_bytes = reader.ReadBin(reinterpret_cast<char*>(sfx_buffer.GetSampleData().u8), buffer_size);
	for (uint64_t i = read_bytes; i < buffer_size; ++i) {
		sfx_buffer.GetSampleData().u8[i] = 0;
	}
	return buffer_size == read_bytes;
}

/*bool retro3d::LoadWAV(const std::string &file, retro3d::Sound &sfx)
{
	// http://soundfile.sapp.org/doc/WaveFormat/

	std::ifstream fin(file.c_str(), std::ios::binary);
	if (fin.is_open() == true) {

//		WAV_Header header;
		retro3d::WAV_Header header;

		fin.read(reinterpret_cast<char*>(&header), sizeof(header));

		if (
//			header.descriptor.chunk_size != 4 + (8 + header.format.sub_chunk_1_size) + (8 + header.data.sub_chunk_2_size) || // I don't think checking this serves any purpose even if it does not add up
			header.format.byte_rate != header.format.sample_rate * header.format.num_channels * (header.format.bits_per_sample / 8) ||
			header.format.block_align != header.format.num_channels * (header.format.bits_per_sample / 8)
			) {
			std::cout << "[LoadWAV] Corruption in WAV (return default), ";
			CreateDefaultSound(sfx);
			return false;
		}

		if (
			std::string(header.descriptor.chunk_id, sizeof(header.descriptor.chunk_id)) != "RIFF" ||
			std::string(header.descriptor.format, sizeof(header.descriptor.format)) != "WAVE" ||
			std::string(header.format.sub_chunk_1_id, sizeof(header.format.sub_chunk_1_id)) != "fmt " ||
			header.format.sub_chunk_1_size != 16 ||
			header.format.audio_format != 1 ||
			(header.format.num_channels != 1 && header.format.num_channels != 2) ||
			(header.format.bits_per_sample != 8 && header.format.bits_per_sample != 16) ||
			std::string(header.data.sub_chunk_2_id, sizeof(header.data.sub_chunk_2_id)) != "data"
			) {
			std::cout << "[LoadWAV] Unsupported format in WAV (return default), ";
			CreateDefaultSound(sfx);
			return false;
		}

		if (header.data.sub_chunk_2_size > uint32_t(std::numeric_limits<int>::max())) {
			std::cout << "[LoadWAV] Too large data chunk in WAV (return default), ";
			CreateDefaultSound(sfx);
			return false;
		}

		retro3d::Sound::Format fmt;
		fmt.sample_rate = header.format.sample_rate;
		fmt.num_channels = header.format.num_channels == 1 ? retro3d::Sound::Channels_Mono : retro3d::Sound::Channels_Stereo;
		fmt.sample_format = header.format.bits_per_sample == 8 ? retro3d::Sound::SampleFormat_UINT8 : retro3d::Sound::SampleFormat_INT16;

		sfx.CreateSamples(header.data.sub_chunk_2_size / header.format.num_channels / (header.format.bits_per_sample / 8), fmt);

		if (header.data.sub_chunk_2_size > 0) {
			fin.read(reinterpret_cast<char*>(sfx.GetSampleData().u8), sfx.GetRawByteCount());
		}

		RETRO3D_ASSERT(header.data.sub_chunk_2_size == sfx.GetRawByteCount());

	} else {
		std::cout << "[LoadWAV] Bad file in WAV (return default), ";
		CreateDefaultSound(sfx);
		return false;
	}

	return true;
}*/

bool retro3d::LoadWAV(retro3d::Reader &reader, retro3d::Sound &sfx)
{
	// http://soundfile.sapp.org/doc/WaveFormat/

	if (reader.IsOpen() == false) {
		std::cout << "[LoadWAV] Bad file in WAV (return default), ";
		CreateDefaultSound(sfx);
		return false;
	}

	retro3d::WAV_Header header;
	if (retro3d::LoadWAVHeader(reader, header) == false) {
		std::cout << "[LoadWAV] Could not read (1) (return default), ";
		CreateDefaultSound(sfx);
		return false;
	}

	const int32_t VerifyCode = retro3d::VerifyWAVHeader(header);
	switch (VerifyCode) {
	case 1:
		std::cout << "[LoadWAV] Corruption in WAV (return default), ";
		break;
	case 2:
		std::cout << "[LoadWAV] Unsupported format in WAV (return default), ";
		break;
	case 3:
		std::cout << "[LoadWAV] Too large data chunk in WAV (return default), ";
		break;
	}
	if (VerifyCode != 0) {
		CreateDefaultSound(sfx);
		return false;
	}

	retro3d::Sound::Format fmt;
	fmt.sample_rate = header.format.sample_rate;
	fmt.num_channels = header.format.num_channels == 1 ? retro3d::Sound::Channels_Mono : retro3d::Sound::Channels_Stereo;
	fmt.sample_format = header.format.bits_per_sample == 8 ? retro3d::Sound::SampleFormat_UINT8 : retro3d::Sound::SampleFormat_INT16;

	sfx.CreateSamples(header.data.sub_chunk_2_size / header.format.num_channels / (header.format.bits_per_sample / 8), fmt);

	if (header.data.sub_chunk_2_size > 0) {
		const uint64_t read_bytes = sfx.GetRawByteCount();
		if (reader.ReadBin(reinterpret_cast<char*>(sfx.GetSampleData().u8), read_bytes) < read_bytes) {
			std::cout << "[LoadWAV] Could not read (2) (return default), ";
			CreateDefaultSound(sfx);
			return false;
		}
	}

	RETRO3D_ASSERT(header.data.sub_chunk_2_size == sfx.GetRawByteCount());

	return true;
}

bool retro3d::LoadWAV(const std::string &file, retro3d::Sound &sfx)
{
	retro3d::Reader reader;
	reader.ReadFromFile(file);
	return retro3d::LoadWAV(reader, sfx);
}
