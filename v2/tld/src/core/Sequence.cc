#include "core/Sequence.hpp"

vector<string> Sequence::extensions = {".jpg", ".png", ".pgm"};

vector<string> Sequence::tldSequences = {
    "01_david",
    "02_jumping",
    "03_pedestrian1",
    "04_pedestrian2",
    "05_pedestrian3",
    "06_car",
    "07_motocross",
    "08_volkswagen",
    "09_carchase"
 };

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
    struct dirent **namelist;
    int n,i;
    const char* directoryStr = directory.c_str();
    n = scandir(directoryStr, &namelist, 0, versionsort);

    vector<string> fileList;
    for(i = 0 ; i < n; ++i) {
        struct dirent *namelist_i = namelist[i];
        string fileName(namelist_i->d_name);
        if (isImageFile(fileName)) {
          fileList.push_back(directory + "/" + fileName);
        }
        //free(namelist[i]);
        delete[] namelist_i;
    }

    delete[] namelist;
    return fileList;
}

Sequence::Sequence(string key, int skip, int limit) {
    this->key = key;
    this->limit = limit;

    string resourceString(RESOURCE_DIR);
    string sampleDirectory = resourceString + "/" + key;

    struct stat info;
    const char* pathname = sampleDirectory.c_str();
    if( stat( pathname, &info ) != 0 ) {
        printf( "cannot access %s\n", pathname );
        throw invalid_argument(key + " does not exist under " + resourceString);
    } else if( info.st_mode & S_IFDIR ) { // S_ISDIR() doesn't exist on my windows
        //printf( "%s is a directory\n", pathname );
    } else {
        //printf( "%s is no directory\n", pathname );
    }

    this->dir = sampleDirectory;
    string sequenceDirectory = sampleDirectory + "/sequence";

    files = listImageFiles(sequenceDirectory);
    nrOfFrames =  (int) files.size();

    if (limit == NO_LIMIT) {
        //println("Setting limit to nrOfFrames: %d", nrOfFrames);
        this->limit = nrOfFrames;
    }

    processedFrames = skip;

    string initDir = sampleDirectory + "/evaluations/init.txt";
    this->outputFile = sampleDirectory + "/evaluations/mTLD.txt";
    ifstream initFile(initDir);
    if (initFile.is_open()) {
        string line;
        getline(initFile, line);
        Option<Box*> maybeBox = Box::fromLine(line);
        if (maybeBox.isEmpty()) {
            throw "Failed to parse box from line" + line;
        } else {
            initBox = maybeBox.get();
        }
    }
}

Sequence::~Sequence() {
    delete initBox;
}

bool Sequence::hasNext() {
    bool hasMoreFrames = processedFrames < nrOfFrames;
    if (!hasMoreFrames) {
        println("There are no frames");
    }

    bool belowTheLimit = processedFrames < limit;
    if (!belowTheLimit) {
        println("Exceeded The Limit: %d >= %d", processedFrames, limit);
    }
    return hasMoreFrames && belowTheLimit;
}

Frame* Sequence::next() {
    string imageFile = files[processedFrames];

    Option<Frame*> maybeFrame = Frame::fromFile(processedFrames, imageFile);
    processedFrames++;

    if (maybeFrame.isDefined()) {
        return maybeFrame.get();
    } else {
        throw runtime_error("no frame left");
    }
}

Frame* Sequence::get(int frameNo) {
    string imageFile = files[frameNo];
    Option<Frame*> maybeFrame = Frame::fromFile(processedFrames, imageFile);

    if (maybeFrame.isDefined()) {
        return maybeFrame.get();
    } else {
        throw runtime_error("no frame left");
    }
}

vector<Box*> Sequence::loadGroundTruth() {
    vector<Box*> gt;

    string groundTruthFile = this->dir + "/evaluations/gt.txt";
    ifstream gtFile(groundTruthFile);
    if (gtFile.is_open()) {
        string line = "";
        while (getline(gtFile, line)) {
            Option<Box*> maybeBox = Box::fromLine(line);
            if (maybeBox.isEmpty()) {
                throw "Failed to parse box from line" + line;
            } else {
                gt.push_back(maybeBox.get());
            }
        }
    }

    return gt;
}
