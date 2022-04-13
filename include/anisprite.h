#ifndef __ANISPRITE__
#define __ANISPRITE__

#include "olcPixelGameEngine.h"
#include "assettypes.h"
#include "element.h"

namespace olc {
    
    // Data struct for storing frame information

    class Animation : public Element {
    protected:
        uint32_t curFrame; // current image selected to display - must be between 0 and frame count
        std::string curRegion; // current image region of images - string name of region
        uint32_t curRegionFrame; // current image selected relative to the region - this number is mod the selected region size
        olc::vf2d curPosition; // used for current frame offset

        void translateFrame();
        void setRawFrame(uint32_t frame);
        inline uint32_t getRawFrame() const { return curFrame; }

    public:

        Frame frameLocation;

        // set animation frame (relative to region)
        void setFrame(uint32_t frame);

        // get animation frame (relative to region)
        inline uint32_t getFrame() const { return curRegionFrame; }

        // set animation sequence from index or sequence name (optionally set the relative frame)
        void setFrameRegion(uint32_t freg, uint32_t relativeFrame=0);
        void setFrameRegion(const std::string& freg, uint32_t relativeFrame=0);

        // get the current sequence name
        inline std::string getFrameRegion() const { return curRegion; }

        // get the number of sequences in the animation
        inline uint32_t getFrameRegionCount() const { return frameLocation.region.size(); }

        // get the number of frames in the current animation sequence
        inline uint32_t getFrameCount() const { return frameLocation.region.at(curRegion).length; }

        Animation(const Frame& animationData);
        virtual ~Animation();

        virtual void Draw(View* view=nullptr);
    };

}



#endif // __ANISPRITE__