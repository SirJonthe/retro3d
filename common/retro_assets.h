#ifndef RETRO_ASSETS_H
#define RETRO_ASSETS_H

#include <iostream>
#include "MiniLib/MTL/mtlAsset.h"
#include "MiniLib/MTL/mtlStringMap.h"
#include "../serial/retro_serialize.h"

namespace retro3d
{

template < typename type_t > class Asset;

template < typename type_t >
class AssetLib
{
private:
	mtlStringMap< mtlShared<type_t> > m_lib;
	mtlShared<type_t>                 m_fallback;

public:
	bool Exists(const mtlChars &id) const
	{
		return m_lib.GetEntry(id) != nullptr;
	}

	bool Exists(const mtlChars &id, mtlShared<type_t> &out)
	{
		mtlShared<type_t> *r = m_lib.GetEntry(id);
		if (r == nullptr) {
			out = mtlShared<type_t>();
			return false;
		}
		out = *r;
		return true;
	}

	void SetFallback(mtlShared<type_t> fallback)
	{
		m_fallback = fallback;
	}

	mtlShared<type_t> Fetch(const mtlChars &id)
	{
		mtlShared<type_t> *r = m_lib.GetEntry(id);
		if (r == nullptr) {
			std::cout << "Error: Asset \"";
			for (int i = 0; i < id.GetSize(); ++i) { std::cout << id[i]; }
			std::cout << "\" not available (return fallback). ";
			mtlShared<type_t> fallback;
			fallback.New();
			*fallback.GetShared() = *m_fallback.GetShared();
			return fallback;
		}
		return *r;
	}

	mtlShared<type_t> FetchCopy(const mtlChars &id)
	{
		mtlShared<type_t> *r = m_lib.GetEntry(id);
		if (r == nullptr) {
			std::cout << "Error: Asset \"";
			for (int i = 0; i < id.GetSize(); ++i) { std::cout << id[i]; }
			std::cout << "\" not available (return fallback). ";
			mtlShared<type_t> fallback;
			fallback.New();
			*fallback.GetShared() = *m_fallback.GetShared();
			return fallback;
		} else {
			mtlShared<type_t> c;
			c.New();
			*c.GetShared() = *r->GetShared();
			*r = c;
		}
		return *r;
	}

	void Store(const type_t &asset, const mtlChars &id)
	{
		mtlShared<type_t> *new_entry = m_lib.CreateEntry(id);
		if (new_entry != nullptr) {
			mtlShared<type_t> ref;
			ref.New();
			*ref.GetShared() = asset;
			*new_entry = ref;
		}
	}

	void Store(mtlShared<type_t> &asset, const mtlChars &id)
	{
		mtlShared<type_t> *new_entry = m_lib.CreateEntry(id);
		if (new_entry != nullptr) {
			*new_entry = asset;
		}
	}
};

template < typename type_t >
class Asset : public retro3d::Serializeable
{
public:
	static AssetLib<type_t> Library;
};

}

template < typename type_t > retro3d::AssetLib<type_t> retro3d::Asset<type_t>::Library;

#endif // RETRO_ASSETS_H
