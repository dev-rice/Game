#include "file.hpp"

File::File() {
    path_delimiter = '/';
    setFilename("");
    setDirectory("");
}

File::File(string directory, string filename) {
    path_delimiter = '/';
    setFilename(filename);
    setDirectory(directory);
}

bool File::isBlank() {
    bool filename_blank = (filename == "");
    return filename_blank;
}

void File::setFilename(string filename) {
    this->filename = filename;
}

void File::setDirectory(string directory) {
    // If the directory doesn't end with a path delimiter then add it on
    if (directory.back() != path_delimiter) {
        directory += path_delimiter;
    }

    this->directory = directory;
}

string File::getFilename() {
    return filename;
}

string File::getDirectory() {
    return directory;
}

string File::getFilepath() {
    return directory + filename;
}
