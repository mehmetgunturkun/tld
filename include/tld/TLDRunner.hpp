#ifndef TLD_RUNNER_H
#define TLD_RUNNER_H

#include "util/Dataset.hpp"
#include "core/Frame.hpp"
#include "core/Box.hpp"

#include "tld/TLD.hpp"

class TLDRunner {
private:
    Dataset* dataset;
    Frame* firstFrame;
    Box* firstBox;
    TLD* tld;
public:
     TLDRunner(string key);
     void run();
     void persistOutput(Option<Box>* maybeBox);
};
#endif
