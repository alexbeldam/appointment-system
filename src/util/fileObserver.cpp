#include "util/fileObserver.hpp"

using std::string;
using std::vector;

FileObserver::FileObserver(const vector<string>& filenames) {
    for (const auto& filename : filenames) {
        string fullPath = BASE_DIR + filename + EXTENSION;

        fileTimestamps[fullPath] = fs::last_write_time(fullPath);
    }
}

bool FileObserver::hasFileChanged() {
    bool changed = false;

    for (auto& pair : fileTimestamps) {
        const string& path = pair.first;
        FileTime& lastKnownTime = pair.second;

        try {
            FileTime currentWriteTime = fs::last_write_time(path);

            if (currentWriteTime > lastKnownTime) {
                lastKnownTime = currentWriteTime;
                changed = true;
            }
        } catch (const fs::filesystem_error& ignore) {
        }
    }

    return changed;
}