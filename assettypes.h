#ifndef __ASSET_TYPES__
#define __ASSET_TYPES__

#include "olcPixelGameEngine.h"
#include <memory>
#include <string>

/*
    Important Asset Types And Structs Defined Here
*/

namespace olc {

    struct AssetReference {
        std::string name;
        enum AssetType {
            SPRITE, SOUND, RAW, ANIMATION
        } type;
    };

    struct ResourceData {
        const char* data;
        size_t length;
        AssetReference::AssetType type;
    };

    typedef ResourceData RawAsset;

    typedef std::shared_ptr<olc::Sprite> SpriteAsset;

    struct ResourceTexture {
        SpriteAsset sprite;
        olc::Decal texture;
    };

    typedef std::shared_ptr<ResourceTexture> TextureAsset;

    struct Frame {
        olc::vf2d offset; // frame source start position
        olc::vf2d size; // image width / height
        olc::vf2d spread; // pixel offset per image

        uint32_t count; // image total count
        uint32_t rowsize; // image count per row

        TextureAsset texture; // actual texture image used
    };

    typedef std::shared_ptr<Frame> AnimationAsset;



    typedef std::shared_ptr<void*> SoundAsset;

}

#endif // __ASSET_TYPES__