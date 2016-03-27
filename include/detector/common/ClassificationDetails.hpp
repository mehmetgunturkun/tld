#ifndef CLASSIFICATION_DETAILS_H
#define CLASSIFICATION_DETAILS_H

class ClassificationDetails {
public:
    double score;
    ClassificationDetails* merge(ClassificationDetails* other);
};
#endif
