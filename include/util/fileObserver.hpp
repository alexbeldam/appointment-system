#ifndef FILE_OBSERVER_HPP
#define FILE_OBSERVER_HPP

#include <filesystem>
#include <map>
#include <string>
#include <vector>

namespace fs = std::filesystem;
using FileTime = fs::file_time_type;

class FileObserver {
   private:
    static constexpr const char* BASE_DIR = "data/";
    static constexpr const char* EXTENSION = ".csv";

    std::map<std::string, FileTime> fileTimestamps;

   public:
    FileObserver(const std::vector<std::string>& filenames);

    bool hasFileChanged();
};

#endif