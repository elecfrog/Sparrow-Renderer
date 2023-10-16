#pragma once

#include "Asset.h"

class AssetManager
{
public:
    template<typename AssetType>
    bool loadAsset(const std::string& asset_url, AssetType& out_asset) const
	{
		return false;
	}
};