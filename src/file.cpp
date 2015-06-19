#include "file.hpp"

File::File() {
    path_delimiter = '/';
    setFilename("");
    setDirectory("");
}

File::File(string filepath) {
    path_delimiter = '/';

    // Split the filepath into a directory and a filename
    int beginIdx = filepath.rfind(path_delimiter);
    string filename = filepath.substr(beginIdx + 1);
    string directory = filepath.substr(0, beginIdx);

    setFilename(filename);
    setDirectory(directory);

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

void File::save() {
    // Overwrite the file that this was loaded from
    saveAs(getFilepath());
}

void File::saveAs(string filepath) {
    // Generic file, doesn't really know how to save
}
