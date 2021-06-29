#include "archivefilemake.h"
#include "archiveassetmanager.h"
#include "clock.h"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

bool autoExportArchive(const std::string& dir = ".\\", const std::string& dest = "archive.dat") {
    std::cout << "Find files...\n";
    std::vector<fs::path> paths;

    fs::directory_iterator fdir(dir);
    for(fs::path p : fdir){
        paths.push_back(p);
    }


    std::cout << "Init Archive...\n";
    ArchiveFileExporter archive;

    std::cout << "Sign Archive...\n";
    archive.SignArchive("test-archive-JMSCREATOR");

    std::cout << "Add Files To Archive...\n";
    size_t index = 0;
    for(fs::path& path : paths){
        std::string ext = path.extension().string(),
                    fname = path.filename().string(),
                    type = "file";
        if(fs::is_directory(path)) continue; // do not allow directories
        if(ext == ".exe") continue; // do not allow *.exe in archive
        if(ext == ".dat") continue; // do not allow *.dat in archive
        
        if(ext == ".png") type = "sprite";
        if(ext == ".mnu") type = "menu";
        if(ext == ".ttf") type = "font";
        if(ext == ".otf") type = "font";
        if(ext == ".ani") type = "animation";
        if(ext == ".ogg") type = "sound";

        std::cout << "\t" << fname << "\n";

        if(!archive.AddFile(path.string(), fname, type)){
            std::cout << "Archive Failed Operation\n";
            return false;
        }
    }

    std::cout << "Export Archive...\n";
    return archive.SaveArchive(dest, "Temp Archive");
}

int main(int argc, const char** argv) {
    fs::path path = fs::path(argv[0]);
    std::string wdir = path.root_path().string() + path.relative_path().replace_filename("").string();

    return (argc > 1 ? autoExportArchive(wdir, argv[1]) : autoExportArchive(wdir)) ? 0 : 1;
}