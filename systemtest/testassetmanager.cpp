#include "resources.h"
#include "anisprite.h"
#include "archivefilemake.h"
#include "archiveassetmanager.h"
#include "olcPixelGameEngine.h"
#include "clock.h"

#include <filesystem>

namespace fs = std::filesystem;

std::vector<std::string> arguments;

bool testImportArchive() {
    std::cout << "Init Archive...\n";

    ArchiveFile archive;

    std::string data;
    {
        std::stringstream sdata;
        std::ifstream file("archive.dat", std::ios::binary | std::ios::in);
        if(!file.is_open()){
            std::cout << "Failed to load archive.dat\n";
            return false;
        }
        sdata << file.rdbuf();
        file.close();

        data = sdata.str();
    }

    std::cout << "Load Archive...\n";
    archive.LoadArchive(data.data(), data.size());

    std::cout << "List Archive Files...\n";
    auto list = archive.ListFiles();
    for(auto& f : list){
        std::cout << "Resource: " << f.name << "\n"
                    << "\tfilename: " << f.filename << "\n"
                    << "\ttype: " << f.type << "\n"
                    << "\tlength: " << f.size << "\n"
                    << "\toffset: " << f.offset << "\n"
                    << "\tin memory: " << std::hex << reinterpret_cast<uint64_t>(f.buffer) << std::dec << "\n";
    }

    return true;
}

bool testExtractArchive() {
    ArchiveFile archive;

    std::cout << "Load Internal Archive...\n";
    archive.LoadArchive(archive_data, sizeof(archive_data));

    std::cout << "Extract Internal Archive Files...\n";
    auto list = archive.ListFiles();

    for(auto& f : list){
        std::ofstream file("extra\\" + f.filename, std::ios::out | std::ios::binary);
        if(!file.is_open()) continue;

        file.write(f.buffer, f.size);

        file.close();
    }

    return true;
}

class Window : public olc::PixelGameEngine {
    olc::ArchiveAssetManager& manager;
    Clock runtime;
public:
    Window(olc::ArchiveAssetManager& mgr): manager(mgr)  {
        sAppName = "Test Asset Manager";

        if(Construct(500, 500, 1, 1)) {
            Start();
        }
    }

    virtual ~Window() {
        decals.clear();
        for(auto& ptr : objs) delete ptr;
    }

    std::vector<olc::TextureAsset> decals;
    std::vector<olc::Animation*> objs;
    std::vector<olc::SoundAsset> sounds;

    olc::Animation* player;

    bool OnUserCreate() {
        olc::Animation::SetPGE(this);
        player = nullptr;

        bool success = false;
        for(auto& asset : manager.GetResourceList()){
            switch(asset.type){
                case olc::AssetReference::SPRITE: {
                    olc::TextureAsset decal = manager.GetTexture(asset.name);

                    if(decal != nullptr){
                        success = true;
                        decals.push_back(decal);
                    } else {
                        std::cout << "Failed to load sprite: " << asset.name << "\n";
                    }
                    break;
                }

                case olc::AssetReference::ANIMATION: {
                    olc::AnimationAsset frameData = manager.GetAnimation(asset.name);

                    if(frameData != nullptr){
                        success = true;
                        std::cout << "Name: " << asset.name << "\n"
                                << "Count: " << frameData->count << "\n"
                                << "Rowsize: " << frameData->rowsize << "\n"
                                << "Offset: " << frameData->offset.str() << "\n"
                                << "Size: " << frameData->size.str() << "\n"
                                << "Spread: " << frameData->spread.str() << "\n"
                                << "Regions: " << frameData->region.size() << "\n";
                        
                        for(auto& rg : frameData->region) std::cout << "\t" << rg.first << " : {" << rg.second.start << ", " << rg.second.length << "}\n";

                        olc::Animation* s = new olc::Animation(*frameData);
                        objs.push_back(s);
                        if(asset.name == "player.ani") player = s;
                        s->origin = s->frameLocation.size / 2.0f;
                        s->position = {float(objs.size()) * 90.0f, ScreenHeight() / 2.0f};
                        s->scale /= 2.0f;
                    } else {
                        std::cout << "Failed to load animation: " << asset.name << "\n";
                    }
                    break;
                }
                
                case olc::AssetReference::SOUND: {
                    olc::SoundAsset sound = manager.GetSound(asset.name);

                    if(sound != nullptr){
                        sounds.push_back(sound);
                        sound->defaultDestroy(true);
                    } else {
                        std::cout << "Failed to load sound: " << asset.name << "\n";
                    }
                    break;
                }
            }
        }
        
        return success; // whether sprites loaded or not
    }

    bool OnUserUpdate(float delta) {
        const float width = 128, height = 128;
        Clear(olc::BLANK);

        {
            static Clock clockSound;
            if(clockSound.getSeconds() > 5){
                sounds[rand() % sounds.size()]->play();
                if(player != nullptr){
                    player->setFrameRegion((rand() % (player->getFrameRegionCount() - 1)) + 1);
                }
                clockSound.restart();
            }
        }

        int e=0,
            mw = ScreenWidth() / width, mh = ScreenHeight() / height;
		for(auto &s : decals){
			s->texture.Update();
			float scale = std::min(width / s->sprite->width, height / s->sprite->height);

			DrawDecal({ float(e % mw) * width , float(e / mh) * height}, &s->texture, {scale, scale});
			++e;
		}

        bool nextFrame = false;
        static Clock nf;
        if(nf.getSeconds() > 0.20f){
            nextFrame = true;
            nf.restart();
        }

        for(auto &s : objs){

            if(s->frameLocation.count > 1){
                float sin = std::sin(runtime.getMilliseconds() / 800.0f) / 15.0f;
                s->scale = { 1.3f + sin, 1.3f + sin };
                s->position.x += sin;
                
                if(nextFrame) s->setFrame(s->getFrame() + 1);
                
                s->Draw();
            }
        }
        
        return true;
    }
};

bool testAssetManager() {

    std::cout << "Init Asset Manager...\n";
    olc::ArchiveAssetManager manager;

    manager.LoadArchive(archive_data, sizeof(archive_data));

    std::cout << "Check Resources...\n";
    auto list = manager.GetResourceList();

    for(auto& r : list){
        std::cout << "Resource: " << r.name << "\n"
                  << "  Type: " << manager.GetAssetType(r.type) << "\n";
    }

    // spawn window and test
    Window win(manager);

    return true;
}

int main(int argc, char** argv){
    // load all paths
    for(int i=1; i < argc; ++i){
        arguments.push_back(argv[i]);
    }

    if(arguments.size()){
        if(arguments[0] == "extract"){
            testExtractArchive();
        }
        if(arguments[0] == "import"){
            testImportArchive();
        }
    } else {
        testAssetManager();
    }

    return 0;
}