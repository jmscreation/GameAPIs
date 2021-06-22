#include "anisprite.h"

namespace olc {

    // PGE Connector
    olc::PixelGameEngine* Animation::pge = nullptr;

    // Construct
    Animation::Animation(const Frame& animationData):
        position({0.0f, 0.0f}), scale({1.0f, 1.0f}), rotation(0.0f), origin({0.0f, 0.0f}), blend(olc::WHITE),
        frameLocation(animationData)
    {
        if(pge == nullptr) std::cout << "Did you forget to call: olc::Animation::SetPGE( pge pointer ); ?\n"; // warn developer if they forgot the init call
    }

    // Destruct
    Animation::~Animation() {}

    // Update a frame
    void Animation::setFrame(uint32_t frame) {
        curFrame = frame; // for reading purpose

        uint32_t xframe = frame % frameLocation.rowsize, // x frame number
                 yframe = frame / frameLocation.rowsize; // y frame number
        
        // calculate:     offset  +  image offset   +  image spread
        curPosition.x = frameLocation.offset.x + xframe * frameLocation.size.x + xframe * frameLocation.spread.x;
        curPosition.y = frameLocation.offset.y  + yframe * frameLocation.size.y + yframe * frameLocation.spread.y;
    }

    // Draw
    void Animation::Draw() {
        if(pge == nullptr || frameLocation.texture == nullptr) return;

        pge->DrawPartialRotatedDecal(position, &frameLocation.texture->texture, rotation, origin, curPosition, frameLocation.size, scale, blend);
    }

}