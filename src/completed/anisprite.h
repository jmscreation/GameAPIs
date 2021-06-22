#ifndef __ANISPRITE__
#define __ANISPRITE__

#include "olcPixelGameEngine.h"
#include "assettypes.h"

namespace olc {
    
    // Data struct for storing frame information

    class Animation {
    protected:
        static olc::PixelGameEngine* pge;
        uint32_t curFrame; // current image selected to display - must be between 0 and frame count
        olc::vf2d curPosition; // used for current frame offset
    public:
        olc::vf2d position;
        olc::vf2d scale;
        olc::vf2d origin;
        float rotation; // radians
        olc::Pixel blend;
        
        Frame frameLocation;

        void setFrame(uint32_t frame);
        inline uint32_t getFrame() { return curFrame; }

        static inline void SetPGE(olc::PixelGameEngine* pge) { Animation::pge = pge; } // update pge pointer

        Animation(const Frame& animationData);
        virtual ~Animation();

        virtual void Draw();
    };

}



#endif // __ANISPRITE__