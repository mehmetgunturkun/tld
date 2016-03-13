#include "util/Dataset.hpp"

vector<string> Dataset::extensions = {".jpg", ".png"};

bool endsWith (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

bool isImageFile(string fileName) {
    bool isImage = false;
    for(vector<string>::iterator extension = Dataset::extensions.begin(); extension != Dataset::extensions.end(); ++extension) {
        if (endsWith(fileName, *extension)) {
            isImage = true;
            break;
        }
    }
    return isImage;
}

vector<string> listImageFiles(string directory) {
    struct dirent *entry;
    DIR* pDIR = opendir(directory.c_str());
    vector<string> fileList;
    if( pDIR != NULL) {
        entry = readdir(pDIR);
        while(entry != NULL) {
            string fileName = string(entry->d_name);
             if (isImageFile(fileName)) {
                 fileList.push_back(fileName);
             }
             entry = readdir(pDIR);
         }
         closedir(pDIR);
     }
     return fileList;
}

Dataset::Dataset(string key) {
    string resourceString(RESOURCE_DIR);
    string sampleDirectory = resourceString + "/" + key;
    string sequenceDirectory = sampleDirectory + "/sequence";

    files = listImageFiles(sequenceDirectory);
    size =  (int) files.size();

    string initDir = sampleDirectory + "/evaluations/init.txt";
    ifstream initFile(initDir);
    if (initFile.is_open()) {
        string line;
        getline(initFile, line);
        Option<Box>* maybeBox = Box::parseFromLine(line);
        if (maybeBox->isEmpty()) {
            throw "Failed to parse box from line" + line;
        } else {
            initBox = maybeBox->get();
        }
    }
}

Dataset::Dataset(string key, int skip) {

}

bool Dataset::hasNext() {
    throw "NotImplemented!";
}

Frame* Dataset::next() {
    throw "NotImplemented!";
}

void Dataset::reset() {

}
