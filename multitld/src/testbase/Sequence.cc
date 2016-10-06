#include "testbase/Sequence.hpp"

vector<string> Sequence::extensions = {".jpg", ".png", ".pgm"};

bool endsWith (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (fullString.compare (fullString.length() - ending.length(), ending.length(), ending) == 0);
    } else {
        return false;
    }
}

bool isImageFile(string fileName) {
    bool isImage = false;
    for(vector<string>::iterator extension = Sequence::extensions.begin(); extension != Sequence::extensions.end(); ++extension) {
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
                 fileList.push_back(directory + "/" + fileName);
             }
             entry = readdir(pDIR);
         }
         closedir(pDIR);
     }
     return fileList;
}

Sequence::Sequence(string key, int skip, int limit) {
    this->key = key;
    this->limit = limit;

    string resourceString(RESOURCE_DIR);
    string sampleDirectory = resourceString + "/" + key;
    this->dir = sampleDirectory;
    string sequenceDirectory = sampleDirectory + "/sequence";

    files = listImageFiles(sequenceDirectory);
    nrOfFrames =  (int) files.size();
    processedFrames = skip;

    string initDir = sampleDirectory + "/evaluations/init.txt";
    this->outputFile = sampleDirectory + "/evaluations/mTLD.txt";
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

bool Sequence::hasNext() {
    return processedFrames < nrOfFrames && processedFrames < limit;
}

Frame* Sequence::next() {
    string imageFile = files[processedFrames];

    Frame* frame = new Frame(imageFile);
    frame->id = processedFrames;

    processedFrames++;
    return frame;
}

Frame* Sequence::get(int frameNo) {
    string imageFile = files[frameNo];
    Frame* frame = new Frame(imageFile);
    return frame;
}
