#include <cstdlib>
 #include <cstdio>
 #include "core/Frame.hpp"
 #include "core/FrameView.hpp"
 #include "core/Box.hpp"
 #include "detector/common/BoxIterator.hpp"
 #include "detector/ensemble/EnsembleClassifier.hpp"

 #include "util/Util.h"
 #include "util/Dataset.hpp"

 int main(int argc, char** argv) {
     Dataset* dataset = new Dataset("car");
     Frame* f = dataset->next();
     Box* b = dataset->initBox;

     EnsembleClassifier* eClassifier = new EnsembleClassifier();
     println("%s", eClassifier->toString().c_str());
     return EXIT_SUCCESS;
 }
