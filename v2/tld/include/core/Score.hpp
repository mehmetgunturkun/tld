#ifndef SCORE_H
#define SCORE_H

class Score {
public:
    virtual ~Score();
    virtual double getValue(int modelId);
};
#endif
