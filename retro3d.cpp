#include <iostream>
#include "common/MiniLib/MGL/mglCollision.h"
#include "retro3d.h"
#include "common/retro_defs.h"
#include "common/retro_factory.h"
#include "backend/null_render_device.h"
#include "backend/null_input_device.h"
#include "backend/null_sound_device.h"
#include "backend/null_video_device.h"
#include "ecs/systems/retro_collision_system.h"
#include "ecs/systems/retro_light_system.h"
#include "ecs/systems/retro_render_system.h"
#include "ecs/systems/retro_transform_system.h"
#include "ecs/systems/retro_audio_system.h"

void retro3d::Engine::CreateBaseModels( void )
{
	std::cout << "Creating base content...";

	if (retro3d::Texture::Library.Exists("Default.Texture") == false) {
		tiny3d::Image img;
		retro3d::CreateDefaultImage(img);
		retro3d::Texture t(img);
		retro3d::Texture::Library.Store(t, "Default.Texture");
	}

	DefaultModel();
	DefaultTexture();

	if (retro3d::Model::Library.Exists("Default.Cube.Model") == false) {
		retro3d::Model m;
		m.v.Create(8);
		m.t.Create(4);
		m.n.Create(6);
		m.m.Create(6);

		m.v[0] = mmlVector<3>(-0.5,  0.5, -0.5);
		m.v[1] = mmlVector<3>( 0.5,  0.5, -0.5);
		m.v[2] = mmlVector<3>( 0.5, -0.5, -0.5);
		m.v[3] = mmlVector<3>(-0.5, -0.5, -0.5);
		m.v[4] = mmlVector<3>(-0.5,  0.5,  0.5);
		m.v[5] = mmlVector<3>( 0.5,  0.5,  0.5);
		m.v[6] = mmlVector<3>( 0.5, -0.5,  0.5);
		m.v[7] = mmlVector<3>(-0.5, -0.5,  0.5);
		retro3d::Array< mmlVector<3> >::Library.Store(m.v, "Default.Cube.Vertex");

		m.aabb.FromGeometry(m.v);

		m.t[0] = mmlVector<2>(0.0, 1.0);
		m.t[1] = mmlVector<2>::Fill(1.0);
		m.t[2] = mmlVector<2>(1.0, 0.0);
		m.t[3] = mmlVector<2>::Fill(0.0f);
		retro3d::Array< mmlVector<2> >::Library.Store(m.t, "Default.Cube.TCoord");

		m.n[0] = mmlVector<3>( 1.0,  0.0,  0.0);
		m.n[1] = mmlVector<3>(-1.0,  0.0,  0.0);
		m.n[2] = mmlVector<3>( 0.0,  1.0,  0.0);
		m.n[3] = mmlVector<3>( 0.0, -1.0,  0.0);
		m.n[4] = mmlVector<3>( 0.0,  0.0,  1.0);
		m.n[5] = mmlVector<3>( 0.0,  0.0, -1.0);
		retro3d::Array< mmlVector<3> >::Library.Store(m.n, "Default.Cube.Normal");

		m.m[0].name = "Right";
		m.m[1].name = "Left";
		m.m[2].name = "Up";
		m.m[3].name = "Down";
		m.m[4].name = "Back";
		m.m[5].name = "Front";

		m.m[0].cd = mmlVector<3>::Fill(1.0f);
		m.m[1].cd = mmlVector<3>::Fill(1.0f);
		m.m[2].cd = mmlVector<3>::Fill(1.0f);
		m.m[3].cd = mmlVector<3>::Fill(1.0f);
		m.m[4].cd = mmlVector<3>::Fill(1.0f);
		m.m[5].cd = mmlVector<3>::Fill(1.0f);

		tiny3d::Image img(tiny3d::Texture::MinDimension());

		img.Fill(tiny3d::Color{ 255,   0,   0, tiny3d::Color::Solid });
		m.m[0].td.New();
		m.m[0].td->FromImage(img);
		img.Fill(tiny3d::Color{   0, 255,   0, tiny3d::Color::Solid });
		m.m[1].td.New();
		m.m[1].td->FromImage(img);
		img.Fill(tiny3d::Color{   0,   0, 255, tiny3d::Color::Solid });
		m.m[2].td.New();
		m.m[2].td->FromImage(img);
		img.Fill(tiny3d::Color{ 255, 255,   0, tiny3d::Color::Solid });
		m.m[3].td.New();
		m.m[3].td->FromImage(img);
		img.Fill(tiny3d::Color{ 255,   0, 255, tiny3d::Color::Solid });
		m.m[4].td.New();
		m.m[4].td->FromImage(img);
		img.Fill(tiny3d::Color{   0, 255, 255, tiny3d::Color::Solid });
		m.m[5].td.New();
		m.m[5].td->FromImage(img);

		m.m[0].f.Create(1);
		m.m[1].f.Create(1);
		m.m[2].f.Create(1);
		m.m[3].f.Create(1);
		m.m[4].f.Create(1);
		m.m[5].f.Create(1);

		m.m[0].f[0].Create(4);
		m.m[1].f[0].Create(4);
		m.m[2].f[0].Create(4);
		m.m[3].f[0].Create(4);
		m.m[4].f[0].Create(4);
		m.m[5].f[0].Create(4);

		m.m[0].f[0][0] = retro3d::IndexVTN{ 1, 0, 0 }; // right
		m.m[0].f[0][1] = retro3d::IndexVTN{ 5, 1, 0 };
		m.m[0].f[0][2] = retro3d::IndexVTN{ 6, 2, 0 };
		m.m[0].f[0][3] = retro3d::IndexVTN{ 2, 3, 0 };

		m.m[1].f[0][0] = retro3d::IndexVTN{ 3, 2, 1 }; // left
		m.m[1].f[0][1] = retro3d::IndexVTN{ 7, 3, 1 };
		m.m[1].f[0][2] = retro3d::IndexVTN{ 4, 0, 1 };
		m.m[1].f[0][3] = retro3d::IndexVTN{ 0, 1, 1 };

		m.m[2].f[0][0] = retro3d::IndexVTN{ 0, 3, 2 }; // up
		m.m[2].f[0][1] = retro3d::IndexVTN{ 4, 0, 2 };
		m.m[2].f[0][2] = retro3d::IndexVTN{ 5, 1, 2 };
		m.m[2].f[0][3] = retro3d::IndexVTN{ 1, 2, 2 };

		m.m[3].f[0][0] = retro3d::IndexVTN{ 2, 1, 3 }; // down
		m.m[3].f[0][1] = retro3d::IndexVTN{ 6, 2, 3 };
		m.m[3].f[0][2] = retro3d::IndexVTN{ 7, 3, 3 };
		m.m[3].f[0][3] = retro3d::IndexVTN{ 3, 0, 3 };

		m.m[4].f[0][0] = retro3d::IndexVTN{ 5, 0, 4 }; // back
		m.m[4].f[0][1] = retro3d::IndexVTN{ 4, 1, 4 };
		m.m[4].f[0][2] = retro3d::IndexVTN{ 7, 2, 4 };
		m.m[4].f[0][3] = retro3d::IndexVTN{ 6, 3, 4 };

		m.m[5].f[0][0] = retro3d::IndexVTN{ 0, 0, 5 }; // front
		m.m[5].f[0][1] = retro3d::IndexVTN{ 1, 1, 5 };
		m.m[5].f[0][2] = retro3d::IndexVTN{ 2, 2, 5 };
		m.m[5].f[0][3] = retro3d::IndexVTN{ 3, 3, 5 };

		m.RefreshConnectivity();

		retro3d::Model::Library.Store(m, "Default.Cube.Model");
	}

	if (retro3d::Model::Library.Exists("Default.Plane.Model") == false) {
		retro3d::Model m;
		m.v.Create(4);
		m.t.Create(4); // retro3d::Array< mmlVector<2> >::Library.Fetch("Default.Plane.TCoord");
		m.n.Create(1);
		m.m.Create(1);

		m.v[0] = mmlVector<3>(-0.5,  0.5,  0.0);
		m.v[1] = mmlVector<3>( 0.5,  0.5,  0.0);
		m.v[2] = mmlVector<3>( 0.5, -0.5,  0.0);
		m.v[3] = mmlVector<3>(-0.5, -0.5,  0.0);
		retro3d::Array< mmlVector<3> >::Library.Store(m.v, "Default.Plane.Vertex");

		m.aabb.FromGeometry(m.v);

		m.t[0] = mmlVector<2>( 0.0,  1.0);
		m.t[1] = mmlVector<2>( 1.0,  1.0);
		m.t[2] = mmlVector<2>( 1.0,  0.0);
		m.t[3] = mmlVector<2>( 0.0,  0.0);
		retro3d::Array< mmlVector<2> >::Library.Store(m.t, "Default.Plane.TCoord");

		m.n[0] = mmlVector<3>( 0.0,  0.0, -1.0);
		retro3d::Array< mmlVector<3> >::Library.Store(m.n, "Default.Plane.Normal");

		m.m[0].name = "Front";
		m.m[0].cd = mmlVector<3>( 1.0,  1.0,  1.0);

		m.m[0].td.New();
		m.m[0].td->Copy(*retro3d::Texture::Library.Fetch("Default.Texture").GetShared());

		m.m[0].f.Create(1);
		m.m[0].f[0].Create(4);
		m.m[0].f[0][0] = { 0, 0, 0 };
		m.m[0].f[0][1] = { 1, 1, 0 };
		m.m[0].f[0][2] = { 2, 2, 0 };
		m.m[0].f[0][3] = { 3, 3, 0 };

		m.RefreshConnectivity();

		retro3d::Model::Library.Store(m, "Default.Plane.Model");
	}

	if (retro3d::Model::Library.Exists("Default.Interior.WallFront.Model") == false) {
		retro3d::Model m;
		m.v.Create(4);
		m.t.Create(4);
		m.n.Create(1);
		m.m.Create(1);

		m.v[0] = mmlVector<3>(-0.5,  1.0,  0.5);
		m.v[1] = mmlVector<3>( 0.5,  1.0,  0.5);
		m.v[2] = mmlVector<3>( 0.5,  0.0,  0.5);
		m.v[3] = mmlVector<3>(-0.5,  0.0,  0.5);

		m.aabb.FromGeometry(m.v);

		m.t[0] = mmlVector<2>( 0.0,  1.0);
		m.t[1] = mmlVector<2>( 1.0,  1.0);
		m.t[2] = mmlVector<2>( 1.0,  0.0);
		m.t[3] = mmlVector<2>( 0.0,  0.0);

		m.n[0] = mmlVector<3>( 0.0,  0.0, -1.0);

		m.m[0].name = "Material";
		m.m[0].cd = mmlVector<3>( 1.0,  1.0,  1.0);

		m.m[0].f.Create(1);
		m.m[0].f[0].Create(4);
		m.m[0].f[0][0] = { 0, 0, 0 };
		m.m[0].f[0][1] = { 1, 1, 0 };
		m.m[0].f[0][2] = { 2, 2, 0 };
		m.m[0].f[0][3] = { 3, 3, 0 };

		m.RefreshConnectivity();

		retro3d::Model::Library.Store(m, "Default.Interior.WallFront.Model");
	}

	if (retro3d::Model::Library.Exists("Default.Interior.WallBack.Model") == false) {
		retro3d::Model m;
		m.v.Create(4);
		m.t.Create(4);
		m.n.Create(1);
		m.m.Create(1);

		m.v[0] = mmlVector<3>(-0.5,  1.0,  -0.5);
		m.v[1] = mmlVector<3>( 0.5,  1.0,  -0.5);
		m.v[2] = mmlVector<3>( 0.5,  0.0,  -0.5);
		m.v[3] = mmlVector<3>(-0.5,  0.0,  -0.5);

		m.aabb.FromGeometry(m.v);

		m.t[0] = mmlVector<2>( 0.0,  1.0);
		m.t[1] = mmlVector<2>( 1.0,  1.0);
		m.t[2] = mmlVector<2>( 1.0,  0.0);
		m.t[3] = mmlVector<2>( 0.0,  0.0);

		m.n[0] = mmlVector<3>( 0.0,  0.0,  1.0);

		m.m[0].name = "Material";
		m.m[0].cd = mmlVector<3>( 1.0,  1.0,  1.0);

		m.m[0].f.Create(1);
		m.m[0].f[0].Create(4);
		m.m[0].f[0][3] = { 0, 0, 0 };
		m.m[0].f[0][2] = { 1, 1, 0 };
		m.m[0].f[0][1] = { 2, 2, 0 };
		m.m[0].f[0][0] = { 3, 3, 0 };

		m.RefreshConnectivity();

		retro3d::Model::Library.Store(m, "Default.Interior.WallBack.Model");
	}

	if (retro3d::Model::Library.Exists("Default.Interior.WallRight.Model") == false) {
		retro3d::Model m;
		m.v.Create(4);
		m.t.Create(4);
		m.n.Create(1);
		m.m.Create(1);

		m.v[0] = mmlVector<3>( 0.5,  1.0,  -0.5);
		m.v[1] = mmlVector<3>( 0.5,  1.0,   0.5);
		m.v[2] = mmlVector<3>( 0.5,  0.0,   0.5);
		m.v[3] = mmlVector<3>( 0.5,  0.0,  -0.5);

		m.aabb.FromGeometry(m.v);

		m.t[0] = mmlVector<2>( 0.0,  1.0);
		m.t[1] = mmlVector<2>( 1.0,  1.0);
		m.t[2] = mmlVector<2>( 1.0,  0.0);
		m.t[3] = mmlVector<2>( 0.0,  0.0);

		m.n[0] = mmlVector<3>(-1.0,  0.0,  0.0);

		m.m[0].name = "Material";
		m.m[0].cd = mmlVector<3>( 1.0,  1.0,  1.0);

		m.m[0].f.Create(1);
		m.m[0].f[0].Create(4);
		m.m[0].f[0][3] = { 0, 0, 0 };
		m.m[0].f[0][2] = { 1, 1, 0 };
		m.m[0].f[0][1] = { 2, 2, 0 };
		m.m[0].f[0][0] = { 3, 3, 0 };

		m.RefreshConnectivity();

		retro3d::Model::Library.Store(m, "Default.Interior.WallRight.Model");
	}

	if (retro3d::Model::Library.Exists("Default.Interior.WallLeft.Model") == false) {
		retro3d::Model m;
		m.v.Create(4);
		m.t.Create(4);
		m.n.Create(1);
		m.m.Create(1);

		m.v[0] = mmlVector<3>(-0.5,  1.0,  -0.5);
		m.v[1] = mmlVector<3>(-0.5,  1.0,   0.5);
		m.v[2] = mmlVector<3>(-0.5,  0.0,   0.5);
		m.v[3] = mmlVector<3>(-0.5,  0.0,  -0.5);

		m.aabb.FromGeometry(m.v);

		m.t[0] = mmlVector<2>( 0.0,  1.0);
		m.t[1] = mmlVector<2>( 1.0,  1.0);
		m.t[2] = mmlVector<2>( 1.0,  0.0);
		m.t[3] = mmlVector<2>( 0.0,  0.0);

		m.n[0] = mmlVector<3>( 1.0,  0.0,  0.0);

		m.m[0].name = "Material";
		m.m[0].cd = mmlVector<3>( 1.0,  1.0,  1.0);

		m.m[0].f.Create(1);
		m.m[0].f[0].Create(4);
		m.m[0].f[0][0] = { 0, 0, 0 };
		m.m[0].f[0][1] = { 1, 1, 0 };
		m.m[0].f[0][2] = { 2, 2, 0 };
		m.m[0].f[0][3] = { 3, 3, 0 };

		m.RefreshConnectivity();

		retro3d::Model::Library.Store(m, "Default.Interior.WallLeft.Model");
	}

	if (retro3d::Model::Library.Exists("Default.Interior.Floor.Model") == false) {
		retro3d::Model m;
		m.v.Create(4);
		m.t.Create(4);
		m.n.Create(1);
		m.m.Create(1);

		m.v[0] = mmlVector<3>(-0.5,  0.0,  0.5);
		m.v[1] = mmlVector<3>( 0.5,  0.0,  0.5);
		m.v[2] = mmlVector<3>( 0.5,  0.0, -0.5);
		m.v[3] = mmlVector<3>(-0.5,  0.0, -0.5);

		m.aabb.FromGeometry(m.v);

		m.t[0] = mmlVector<2>( 0.0,  1.0);
		m.t[1] = mmlVector<2>( 1.0,  1.0);
		m.t[2] = mmlVector<2>( 1.0,  0.0);
		m.t[3] = mmlVector<2>( 0.0,  0.0);

		m.n[0] = mmlVector<3>( 0.0,  1.0,  0.0);

		m.m[0].name = "Material";
		m.m[0].cd = mmlVector<3>( 1.0,  1.0,  1.0);

		m.m[0].f.Create(1);
		m.m[0].f[0].Create(4);
		m.m[0].f[0][0] = { 0, 0, 0 };
		m.m[0].f[0][1] = { 1, 1, 0 };
		m.m[0].f[0][2] = { 2, 2, 0 };
		m.m[0].f[0][3] = { 3, 3, 0 };

		m.RefreshConnectivity();

		retro3d::Model::Library.Store(m, "Default.Interior.Floor.Model");
	}

	if (retro3d::Model::Library.Exists("Default.Interior.Ceiling.Model") == false) {
		retro3d::Model m;
		m.v.Create(4);
		m.t.Create(4);
		m.n.Create(1);
		m.m.Create(1);

		m.v[0] = mmlVector<3>(-0.5,  1.0,  0.5);
		m.v[1] = mmlVector<3>( 0.5,  1.0,  0.5);
		m.v[2] = mmlVector<3>( 0.5,  1.0, -0.5);
		m.v[3] = mmlVector<3>(-0.5,  1.0, -0.5);

		m.aabb.FromGeometry(m.v);

		m.t[0] = mmlVector<2>( 0.0,  1.0);
		m.t[1] = mmlVector<2>( 1.0,  1.0);
		m.t[2] = mmlVector<2>( 1.0,  0.0);
		m.t[3] = mmlVector<2>( 0.0,  0.0);

		m.n[0] = mmlVector<3>( 0.0, -1.0,  0.0);

		m.m[0].name = "Material";
		m.m[0].cd = mmlVector<3>( 1.0,  1.0,  1.0);

		m.m[0].f.Create(1);
		m.m[0].f[0].Create(4);
		m.m[0].f[0][3] = { 0, 0, 0 };
		m.m[0].f[0][2] = { 1, 1, 0 };
		m.m[0].f[0][1] = { 2, 2, 0 };
		m.m[0].f[0][0] = { 3, 3, 0 };

		m.RefreshConnectivity();

		retro3d::Model::Library.Store(m, "Default.Interior.Ceiling.Model");
	}

	{
		const tiny3d::UInt size = tiny3d::UInt(mmlMax(tiny3d::SInt(tiny3d::Texture::MinDimension() * 2), TINY3D_CHAR_WIDTH, TINY3D_CHAR_HEIGHT));
		for (int i = 0; i <= (TINY3D_CHAR_LAST - TINY3D_CHAR_FIRST) + 1; ++i) {
			char ch = char(TINY3D_CHAR_FIRST + i);
			std::string id = "Text." + std::string(1, ch);
			const mtlChars c_id = mtlChars::FromDynamic(id.c_str(), int(id.size()));
			if (retro3d::Texture::Library.Exists(c_id) == false) {
				tiny3d::Image img(size);
				img.Fill(tiny3d::Color{ 0, 0, 0, tiny3d::Color::Solid });
				tiny3d::DrawChars(img, tiny3d::Point{ 0, 0 }, 0, &ch, 1, tiny3d::Color{ 255, 255, 255, tiny3d::Color::Solid }, 1);
				for (tiny3d::UInt y = 0; y < img.GetHeight(); ++y) {
					for (tiny3d::UInt x = 0; x < img.GetWidth(); ++x) {
						tiny3d::Color c = img.GetColor(tiny3d::UPoint{ x, y });
						if (c.r < 128 && c.g < 128 && c.b < 128) {
							c.blend = tiny3d::Color::Transparent;
						}
						img.SetColor(tiny3d::UPoint{ x, y }, c);
					}
				}
				retro3d::Texture texture(img);
				texture.SetBlendMode1(tiny3d::Color::Transparent);
				texture.SetBlendMode2(tiny3d::Color::Emissive);
				retro3d::Texture::Library.Store(texture, c_id);
			}
		}
		if (retro3d::Texture::Library.Exists("Text.Tofu") == false) {
			char ch = TINY3D_CHAR_LAST + 1;
			tiny3d::Image img(size);
			img.Fill(tiny3d::Color{ 0, 0, 0, tiny3d::Color::Solid });
			tiny3d::DrawChars(img, tiny3d::Point{ 0, 0 }, 0, &ch, 1, tiny3d::Color{ 255, 255, 255, tiny3d::Color::Solid }, 1);
			for (tiny3d::UInt y = 0; y < img.GetHeight(); ++y) {
				for (tiny3d::UInt x = 0; x < img.GetWidth(); ++x) {
					tiny3d::Color c = img.GetColor(tiny3d::UPoint{ x, y });
					if (c.r < 128 && c.g < 128 && c.b < 128) {
						c.blend = tiny3d::Color::Transparent;
					}
					img.SetColor(tiny3d::UPoint{ x, y }, c);
				}
			}
			retro3d::Texture texture(img);
			texture.SetBlendMode1(tiny3d::Color::Transparent);
			texture.SetBlendMode2(tiny3d::Color::Emissive);
			retro3d::Texture::Library.Store(texture, "Text.Tofu");
		}
	}

	{
		Model model = *retro3d::Model::Library.Fetch("Default.Plane.Model").GetShared();
		for (int i = 0; i <= (TINY3D_CHAR_LAST - TINY3D_CHAR_FIRST) + 1; ++i) {
			char ch = char(TINY3D_CHAR_FIRST + i);
			std::string id = "Text." + std::string(1, ch);
			const mtlChars c_id = mtlChars::FromDynamic(id.c_str(), int(id.size()));
			if (retro3d::Model::Library.Exists(c_id) == false) {
				mtlShared<retro3d::Texture> texture = retro3d::Texture::Library.Fetch(c_id);
				model.m[0].name = std::string(1, ch);
				model.m[0].td.New();
				model.m[0].td->Copy(*retro3d::Texture::Library.Fetch(c_id).GetShared());
				retro3d::Model::Library.Store(model, c_id);
			}
		}
		std::string id = "Text.Tofu";
		const mtlChars c_id = mtlChars::FromDynamic(id.c_str(), int(id.size()));
		if (retro3d::Model::Library.Exists(c_id) == false) {
			mtlShared<retro3d::Texture> texture = retro3d::Texture::Library.Fetch(c_id);
			model.m[0].name = "Tofu";
			model.m[0].td.New();
			model.m[0].td->Copy(*retro3d::Texture::Library.Fetch(c_id).GetShared());
			retro3d::Model::Library.Store(model, c_id);
		}
	}

	std::cout << "done" << std::endl;
}

void retro3d::Engine::SetupTimers( void )
{
	m_real_timer.Pause();
	m_real_timer.SetTickRate(1, 1_s);
	m_real_timer.Reset();

	m_sim_timer.Pause();
	m_sim_timer.SetParent(&m_real_timer);
	m_sim_timer.SetTickRate(1, 1_s);
	m_sim_timer.Reset();

	m_game_timer.Pause();
	m_game_timer.SetParent(&m_sim_timer);
	m_game_timer.SetTickRate(1, 1_s);
	m_game_timer.Reset();

	m_real_timer.Start();
	m_sim_timer.Start();
	m_game_timer.Start();

	m_real_time = m_real_timer.UpdateTimer();
	m_sim_time = m_sim_timer.UpdateTimer();
	m_game_time = m_game_timer.UpdateTimer();
}

void retro3d::Engine::AddRequiredSystems( void )
{
	AddSystem<retro3d::CollisionSystem>();
	AddSystem<retro3d::LightSystem>();
	AddSystem<retro3d::RenderSystem>();
	AddSystem<retro3d::TransformSystem>();
	AddSystem<retro3d::AudioSystem>();
}

void retro3d::Engine::Reset( void )
{
	AddRequiredSystems();

	m_camera = &m_default_camera;

	m_rand.SetSeed(retro3d::Time::Now());
	m_is_running = true;
	m_quit = false;
	m_frame = 0;
	m_delta_time = m_min_delta_time.GetFloatSeconds();

	SetupTimers();

	m_frame_start_time = m_real_timer.GetScaledTime();
	m_frame_time = m_min_delta_time;
}

void retro3d::Engine::UpdateDevices( void )
{
	m_input->Update();
}

void retro3d::Engine::InitSystems( void )
{
	Systems pending = m_systems_pending_init; // copy the pending list in order to handle pending objects that create new pending objects
	m_systems_pending_init.clear(); // all currently pending objects will be initialized via 'pending', so clear this map

	// Initialize pending systems
	for (Systems::iterator i = pending.begin(); i != pending.end(); ++i) {
		i->second->OnSpawn();
		m_systems[i->second->GetInstanceType()] = i->second;
	}
}

void retro3d::Engine::InitComponents( void )
{
	Components pending = m_components_pending_init; // copy the pending list in order to handle pending objects that create new pending objects
	m_components_pending_init.clear(); // all currently pending objects will be initialized via 'pending', so clear this map

	// Hand over pending components to main component list
	for (Components::iterator i = pending.begin(); i != pending.end(); ++i) {
		for (ComponentClass::iterator j = i->second.begin(); j != i->second.end(); ++j) {
			m_components[i->first][j->first] = j->second;
		}
	}

	// Initialize pending components
	for (Components::iterator i = pending.begin(); i != pending.end(); ++i) {
		for (ComponentClass::iterator j = i->second.begin(); j != i->second.end(); ++j) {
			j->second->OnSpawn();
		}
	}

	// Call systems
	for (Systems::iterator system = m_systems.begin(); system != m_systems.end(); ++system) {
		Components::iterator component_class = pending.find(system->second->GetComponentClassType());
		if (component_class != pending.end()) {
			for (ComponentClass::iterator component = component_class->second.begin(); component != component_class->second.end(); ++component) {
				system->second->OnSpawn(component->second);
			}
		}
	}
}

void retro3d::Engine::TickComponents( void )
{
	// Update components
	for (Components::iterator i = m_components.begin(); i != m_components.end(); ++i) {
		for (ComponentClass::iterator j = i->second.begin(); j != i->second.end(); ++j) {
			if (j->second->IsActive() == true) {
				j->second->OnUpdate();
			}
		}
	}
}

void retro3d::Engine::TickSystems( void )
{
	// Let the system iterate through all components of requested type
	for (Systems::iterator system = m_systems.begin(); system != m_systems.end(); ++system) {
		if (system->second->IsActive() == true) {
			Components::iterator component_class = m_components.find(system->second->GetComponentClassType());
			if (component_class != m_components.end()) {
				for (ComponentClass::iterator component = component_class->second.begin(); component != component_class->second.end(); ++component) {
					if (component->second->IsActive() == true) {
						system->second->OnUpdate(component->second);
					}
				}
				system->second->OnUpdate();
			}
		}
	}
}

void retro3d::Engine::TickEntities( void )
{
	mtlItem<retro3d::Entity*> *i = m_entities.GetFirst();
	while (i != nullptr) {
		i->GetItem()->OnUpdate();
		i = i->GetNext();
	}
}

void retro3d::Engine::DestroySystems( void )
{
	// Delete and remove destroyed systems
	for (Systems::iterator i = m_systems.begin(); i != m_systems.end();) {
		if (i->second->IsDestroyed() == true) {
			i->second->OnDestroy();
			delete i->second;
			i = m_systems.erase(i);
		} else {
			++i;
		}
	}
}

void retro3d::Engine::DestroyComponents( void )
{
	// Build a map for components that need to be destroyed
	/*Components pending;
	for (Components::iterator i = m_components.begin(); i != m_components.end();) {
		for (ComponentClass::iterator j = i->second.begin(); j != i->second.end();) {
			if (j->second->IsDestroyed() == true) {
				pending[i->first][j->first] = j->second;
				j = m_components[i->first].erase(j);
			} else {
				++j;
			}
		}
		if (i->second.size() == 0) {
			i = m_components.erase(i);
		} else {
			++i;
		}
	}

	// Call systems
	for (Systems::iterator system = m_systems.begin(); system != m_systems.end(); ++system) {
		Components::iterator component_class = pending.find(system->second->GetComponentClassType());
		if (component_class != pending.end()) {
			for (ComponentClass::iterator component = component_class->second.begin(); component != component_class->second.end(); ++component) {
				system->second->OnDestroy(component->second);
			}
		}
	}

	// Delete and remove destroyed components
	for (Components::iterator i = pending.begin(); i != pending.end(); ++i) {
		for (ComponentClass::iterator j = i->second.begin(); j != i->second.end(); ++j) {
			j->second->OnDestroy();
			delete j->second;
		}
	}*/

	ComponentIters pending;

	// 1) Construct a list containing components to be removed and tell components they are about to be removed.
	for (Components::iterator i = m_components.begin(); i != m_components.end(); ++i) {
		for (ComponentClass::iterator j = i->second.begin(); j != i->second.end(); ++j) {
			if (j->second->IsDestroyed() == true) {
				pending[i->first][j->first] = j;
				// 1.a) Tell components they are about to be removed.
				j->second->OnDestroy();
			}
		}
	}

	// 2) Tell systems that component is to be removed.
	for (Systems::iterator system = m_systems.begin(); system != m_systems.end(); ++system) {
		ComponentIters::iterator component_class = pending.find(system->second->GetComponentClassType());
		if (component_class != pending.end()) {
			for (ComponentClassIters::iterator component = component_class->second.begin(); component != component_class->second.end(); ++component) {
				system->second->OnDestroy(component->second->second);
			}
		}
	}

	// 3) Delete component memory and remove component from component list.
	for (ComponentIters::iterator i = pending.begin(); i != pending.end(); ++i) {
		for (ComponentClassIters::iterator j = i->second.begin(); j != i->second.end(); ++j) {
			// 3.a) Delete component memory.
			delete j->second->second;
			// 3.b) Remove component from component list.
			m_components[i->first].erase(j->second);
		}
	}
}

void retro3d::Engine::DestroyEntities( void )
{
	// Delete and remove destroyed objects
	for (mtlItem< Entity* > *i = m_entities.GetFirst(); i != nullptr;) {
		if (i->GetItem()->IsDestroyed() == true) {
			i->GetItem()->OnDestroy();
			delete i->GetItem();
			i = i->Remove();
		} else {
			i = i->GetNext();
		}
	}
}

void retro3d::Engine::DetectTermination( void )
{
	if (m_entities.GetSize() <= 0) {
		m_quit = true;
		std::cout << "No entities - ";
	}

	if (m_components_pending_init.size() <= 0 && m_components.size() <= 0) {
		m_quit = true;
		std::cout << "No components - ";
	}

	if (m_input->UserQuit()) {
		m_quit = true;
		std::cout << "User quit - ";
	}

	if (m_quit == true) {
		std::cout << "Termination." << std::endl;
	}
}

void retro3d::Engine::TickTime( void )
{
	m_sim_timer.SetTickRate(m_real_timer, false); // Set 1t:1s tick rate by default.

	const retro3d::Time now = m_real_timer.GetScaledTime();
	m_frame_time = now - m_frame_start_time;
	if (m_frame_time < m_min_delta_time) {
		m_input->Sleep(m_min_delta_time - m_frame_time);
	} else if (m_frame_time > m_max_delta_time) {
		// The game does not achieve minimal acceptable target frame rate. Rather than making the time delta larger (as we might break stuff like physics), we slow down execution of the game.
		// We make sure not to update the timer as that will push the current unscaled time delta to the accumulated
		m_sim_timer.SetTimeScale(m_max_delta_time / m_frame_time, false);
	}

	m_real_time = m_real_timer.UpdateTimer();
	m_sim_time = m_sim_timer.UpdateTimer();

	retro3d::Time game_now = m_game_timer.UpdateTimer();
	m_delta_time = (game_now - m_game_time).GetFloatSeconds();
	m_game_time = game_now;

	auto i = m_entities.GetFirst();
	while (i != nullptr) {
		retro3d::Entity *e = i->GetItem();

		game_now = e->m_game_timer.UpdateTimer();
		e->m_delta_time = (game_now - e->m_game_time).GetFloatSeconds();
		e->m_game_time = e->m_game_timer.UpdateTimer();

		i = i->GetNext();
	}

	m_frame_start_time = m_real_time; // Do not use 'now' since that is before sleeping
}

void retro3d::Engine::Tick( void )
{
	m_rand.GetUint();

	m_input->Update();

	InitSystems();

	InitComponents();

	TickComponents();

	TickSystems();

	TickEntities();

	DestroySystems();

	DestroyComponents();

	DestroyEntities();

	m_video->Display();

	DetectTermination();

	TickTime();

	++m_frame;
}

void retro3d::Engine::Cleanup( void )
{
	// Destroy all objects without calling OnDestroy

	for (Systems::iterator system = m_systems.begin(); system != m_systems.end(); ++system) {
		delete system->second;
	}
	m_systems.clear();

	for (Components::iterator i = m_components_pending_init.begin(); i != m_components_pending_init.end(); ++i) {
		for (ComponentClass::iterator j = i->second.begin(); j != i->second.end(); ++j) {
			delete j->second;
		}
	}
	m_components_pending_init.clear();

	for (Components::iterator i = m_components.begin(); i != m_components.end(); ++i) {
		for (ComponentClass::iterator j = i->second.begin(); j != i->second.end(); ++j) {
			delete j->second;
		}
	}
	m_components.clear();

	for (mtlItem< Entity* > *i = m_entities.GetFirst(); i != nullptr;) {
		delete i->GetItem();
		i = i->GetNext();
	}
	m_entities.RemoveAll();
}

retro3d::Engine::Engine( void ) :
	m_renderer(new platform::NullRenderDevice),
	m_sound(new platform::NullSoundDevice),
	m_input(new platform::NullInputDevice),
	m_video(new platform::NullVideoDevice),
	m_camera(&m_default_camera),
	m_frame(0),
	m_rand(),
	m_delta_time(1.0/60.0), m_min_delta_time(1000/120), m_max_delta_time(1000/20),
	m_is_running(false), m_quit(true)
{
	AddRequiredSystems();
	CreateBaseModels();
	SetupTimers();
}

retro3d::Engine::~Engine( void )
{
	// NOTE: Set ptrs to null immediately after being destroyed since the remaining devices might try to access destroyed devices in their respective destructors.
	delete m_video;
	m_video = nullptr;
	delete m_renderer;
	m_renderer = nullptr;
	delete m_input;
	m_input = nullptr;
	delete m_sound;
	m_sound = nullptr;
}

void retro3d::Engine::SetCamera(const retro3d::Camera *camera)
{
	if (camera != nullptr) {
		m_camera = camera;
	} else {
		DefaultCamera();
	}
}

void retro3d::Engine::DefaultCamera( void )
{
	m_camera = &m_default_camera;
}

const retro3d::Camera *retro3d::Engine::GetCamera( void ) const
{
	return m_camera;
}

retro3d::RenderDevice *retro3d::Engine::GetRenderer( void )
{
	return m_renderer;
}

const retro3d::RenderDevice *retro3d::Engine::GetRenderer( void ) const
{
	return m_renderer;
}

retro3d::SoundDevice *retro3d::Engine::GetSoundDevice( void )
{
	return m_sound;
}

const retro3d::SoundDevice *retro3d::Engine::GetSoundDevice( void ) const
{
	return m_sound;
}

retro3d::InputDevice *retro3d::Engine::GetInput( void )
{
	return m_input;
}

const retro3d::InputDevice *retro3d::Engine::GetInput( void ) const
{
	return m_input;
}

retro3d::VideoDevice *retro3d::Engine::GetVideo( void )
{
	return m_video;
}

const retro3d::VideoDevice *retro3d::Engine::GetVideo( void ) const
{
	return m_video;
}

void retro3d::Engine::SetMaxUpdateFrequency(uint32_t max_hz)
{
	m_min_delta_time = retro3d::Time(1000 / max_hz);
}

void retro3d::Engine::SetMaxSimulationTimeDelta(retro3d::Time time_delta)
{
	m_max_delta_time = time_delta;
}

retro3d::Time retro3d::Engine::RealTime( void ) const
{
	return m_real_timer.GetScaledTime();
}

retro3d::Time retro3d::Engine::SimulationTime( void ) const
{
	return m_sim_timer.GetScaledTime();
}

retro3d::Time retro3d::Engine::GameTime( void ) const
{
	return m_game_timer.GetScaledTime();
}

retro3d::Time retro3d::Engine::Time(retro3d::TimerType type) const
{
	switch (type) {
	case retro3d::TIMER_REAL: return RealTime();
	case retro3d::TIMER_SIM:  return SimulationTime();
	case retro3d::TIMER_GAME: return GameTime();
	}
	return retro3d::Time();
}

retro3d::Time retro3d::Engine::GetFrameTime( void ) const
{
	return m_frame_time;
}

retro3d::Time retro3d::Engine::GetLockedFrameTime( void ) const
{
	return mmlMax(m_min_delta_time, m_frame_time);
}

float retro3d::Engine::GetFramesPerSecond( void ) const
{
	return 1.0f / m_frame_time.GetFloatSeconds();
}

float retro3d::Engine::GetLockedFramesPerSecond( void ) const
{
	return 1.0f / GetLockedFrameTime().GetFloatSeconds();
}

uint64_t retro3d::Engine::GetFrameCount( void ) const
{
	return m_frame;
}

retro3d::RealTimeTimer retro3d::Engine::CreateSimulationTimer(uint32_t num_ticks, retro3d::Time over_time) const
{
	return m_sim_timer.SpawnChild(num_ticks, over_time);
}

retro3d::RealTimeTimer retro3d::Engine::CreateGameTimer(uint32_t num_ticks, retro3d::Time over_time) const
{
	return m_game_timer.SpawnChild(num_ticks, over_time);
}

retro3d::RealTimeTimer retro3d::Engine::CreateTimer(retro3d::TimerType type, uint32_t num_ticks, retro3d::Time over_time) const
{
	switch (type) {
	case retro3d::TIMER_REAL: return retro3d::RealTimeTimer(num_ticks, over_time);
	case retro3d::TIMER_SIM:  return CreateSimulationTimer(num_ticks, over_time);
	case retro3d::TIMER_GAME: return CreateGameTimer(num_ticks, over_time);
	}
	return retro3d::RealTimeTimer(num_ticks, over_time);
}

uint32_t retro3d::Engine::GetEntityCount( void ) const
{
	return uint32_t(m_entities.GetSize());
}

double retro3d::Engine::GetTimeScale( void ) const
{
	return 1.0 / m_game_timer.GetTimePerTick().GetFloatSeconds();
}

void retro3d::Engine::SetTimeScale(double time_scale)
{
	m_game_timer.SetTickRate(1, retro3d::Time::FloatSeconds(1.0 / time_scale), false);
}

// NOTE: KEEP THIS FOR REFERENCE!
/*mtlShared<retro3d::Object> retro3d::Engine::CastRay(const retro3d::Ray &world_ray, retro3d::Ray::Contact &out) const
{
	mtlShared<Object> collision_detected;
	float closest_dist = 0.0f;
	auto obj = m_objects.GetFirst();
	while (obj != nullptr) {
		retro3d::Ray::Contact c;
		if (obj->GetItem()->m_is_collideable == true && obj->GetItem()->m_collider->IsColliding(world_ray, &c) == true) {
			if (collision_detected.GetShared() == nullptr || c.entry.distance < closest_dist) {
				collision_detected = obj->GetItem();
				closest_dist = c.entry.distance;
				out = c;
			}
		}
		obj = obj->GetNext();
	}
	return collision_detected;
}*/

uint32_t retro3d::Engine::GetRandomUint( void )
{
	return m_rand.GetUint();
}

uint32_t retro3d::Engine::GetRandomUint(uint32_t min, uint32_t max)
{
	return m_rand.GetUint(min, max);
}

int32_t retro3d::Engine::GetRandomInt( void )
{
	return m_rand.GetInt();
}

int32_t retro3d::Engine::GetRandomInt(int min, int max)
{
	return m_rand.GetInt(min, max);
}

float retro3d::Engine::GetRandomFloat( void )
{
	return m_rand.GetFloat();
}

float retro3d::Engine::GetRandomFloat(float min, float max)
{
	return m_rand.GetFloat(min, max);
}

void retro3d::Engine::Run( void )
{
	if (m_is_running == true) { return; }

	Reset();

	while (m_quit == false) {
		Tick();
	}

	Cleanup();

	m_is_running = false;
}

void retro3d::Engine::Quit( void )
{
	m_quit = true;
}

retro3d::Entity *retro3d::Engine::SpawnEntity(const std::string &name)
{
	// FIXME: Not gonna work if the schematic requires the entity to have an engine connected.
	retro3d::Entity *e = retro3d::Singleton< retro3d::Factory< retro3d::Entity > >::Instance().Assemble(name);
	e->m_engine = this;
	e->m_game_timer.SetParent(&m_game_timer);
	e->m_delta_time = m_delta_time;
	m_entities.AddLast(e);
	e->OnSpawn();
	return e;
}

bool retro3d::Engine::FindEntityByTag(const std::string &tag, mtlList<retro3d::Entity*> &results, bool search_inactive)
{
	mtlItem<retro3d::Entity*> *i = m_entities.GetFirst();
	if (search_inactive == false) {
		while (i != nullptr) {
			if (tag == i->GetItem()->GetTag() && i->GetItem()->IsActive()) {
				results.AddLast(i->GetItem());
			}
			i = i->GetNext();
		}
	} else {
		while (i != nullptr) {
			if (tag == i->GetItem()->GetTag()) {
				results.AddLast(i->GetItem());
			}
			i = i->GetNext();
		}
	}
	return results.GetSize() > 0;
}

bool retro3d::Engine::FindEntityByFilter(uint64_t filter_flags, mtlList<retro3d::Entity*> &results, retro3d::Engine::FilterSearchMode filter_search, bool search_inactive)
{
	mtlItem<retro3d::Entity*> *i = m_entities.GetFirst();
	if (search_inactive == false) {
		while (i != nullptr) {
			if (i->GetItem()->IsActive() == true) {
				switch (filter_search) {
				case FILTERSEARCH_ANY:
					if ((i->GetItem()->GetFilterFlags() & filter_flags) != 0) {
						results.AddLast(i->GetItem());
					}
					break;
				case FILTERSEARCH_EXACT:
					if (i->GetItem()->GetFilterFlags() == filter_flags) {
						results.AddLast(i->GetItem());
					}
					break;
				case FILTERSEARCH_INCLUSIVE:
					if ((i->GetItem()->GetFilterFlags() & filter_flags) == filter_flags) {
						results.AddLast(i->GetItem());
					}
					break;
				}
			}
			i = i->GetNext();
		}
	} else {
		while (i != nullptr) {
			if (i->GetItem()->IsActive() == true) {
				switch (filter_search) {
				case FILTERSEARCH_ANY:
					if ((i->GetItem()->GetFilterFlags() & filter_flags) != 0) {
						results.AddLast(i->GetItem());
					}
					break;
				case FILTERSEARCH_EXACT:
					if (i->GetItem()->GetFilterFlags() == filter_flags) {
						results.AddLast(i->GetItem());
					}
					break;
				case FILTERSEARCH_INCLUSIVE:
					if ((i->GetItem()->GetFilterFlags() & filter_flags) == filter_flags) {
						results.AddLast(i->GetItem());
					}
					break;
				}
			}
			i = i->GetNext();
		}
	}
	return results.GetSize() > 0;
}

retro3d::Entity *retro3d::Engine::FindEntityByUUID(uint64_t uuid, bool search_inactive)
{
	mtlItem<retro3d::Entity*> *i = m_entities.GetFirst();
	while (i != nullptr) {
		if (i->GetItem()->GetUUID() == uuid) {
			if (search_inactive == false && i->GetItem()->IsActive() == false) {
				return nullptr; // We found the entity, but it is not active and we do not include inactive results.
			}
			return i->GetItem();
		}
		i = i->GetNext();
	}

	return nullptr;
}

mtlShared<retro3d::Model> retro3d::Engine::DefaultModel( void )
{
	mtlShared<retro3d::Model> s;
	if (retro3d::Model::Library.Exists("Default", s) == false) {
		s.New();
		CreateDefaultModel(&s->v, &s->t, &s->n, &s->m);
		s->aabb.FromGeometry(s->v);
		s->RefreshConnectivity();
		retro3d::Model::Library.Store(s, "Default");
		retro3d::Model::Library.SetFallback(s);
	}
	return s;
}

mtlShared<retro3d::Texture> retro3d::Engine::DefaultTexture( void )
{
	mtlShared<retro3d::Texture> s;
	if (retro3d::Texture::Library.Exists("Default", s) == false) {
		tiny3d::Image img;
		retro3d::CreateDefaultImage(img);
		s.New();
		s->FromImage(img);
		retro3d::Texture::Library.Store(s, "Default");
		retro3d::Texture::Library.SetFallback(s);
	}
	return s;
}
