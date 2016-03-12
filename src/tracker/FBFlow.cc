#include "tracker/FBFlow.hpp"

float FBFlow::FB_ERROR_LIMIT = 0.0;

float computeFBError(tld::Point* src, tld::Point* target, tld::Point* fb) {
    float dy = fb->underlying.y - src->underlying.y;
    float dx = fb->underlying.x - src->underlying.x;
    float fbe = sqrt(dx*dx + dy*dy);
    return fbe;
}

float computeNCC(Frame* srcFrame, tld::Point* srcPoint, Frame* targetFrame, tld::Point* targetPoint) {
    Size windowSize(4, 4);
    Mat rec0 = Mat::zeros(windowSize, CV_8U);
    Mat rec1 = Mat::zeros(windowSize, CV_8U);
    Mat res = Mat::zeros(Size(1, 1), CV_32F);

    getRectSubPix(*(targetFrame->grayscale), windowSize, targetPoint->underlying, rec0);
    getRectSubPix(*(srcFrame->grayscale), windowSize, srcPoint->underlying, rec1);
    matchTemplate(rec0, rec1, res, CV_TM_CCOEFF_NORMED );
    float ncc = ((float *)(res.data))[0];
    return ncc;
}

float median(vector<float> floatVector) {
    int size = (int) floatVector.size();
    sort(floatVector.begin(), floatVector.end(), [](float a, float b) { return a > b; });
    return floatVector[size / 2];
}

FBFlow::FBFlow(Flow* forward, Flow* backward) {
    int displacementCount = forward->displacementCount;
    vector<Displacement*> forwardDisplacements = forward->displacementList;
    vector<Displacement*> backwardDisplacements = backward->displacementList;

    vector<float> fbErrors;
    vector<float> nccErrors;

    vector<FBDisplacement*> initialDisplacements;

    for (int i = 0; i < displacementCount; i++) {
        Frame* srcFrame = forward->source;
        Frame* targetFrame = forward->target;

        Displacement* fwdDisplacement = forwardDisplacements[i];
        Displacement* bwdDisplacement = backwardDisplacements[i];

        if (fwdDisplacement->state && bwdDisplacement->state) {
            tld::Point* srcPoint = fwdDisplacement->source;
            tld::Point* targetPoint = fwdDisplacement->target;
            tld::Point* fbPoint = bwdDisplacement->target;

            float fbError = computeFBError(srcPoint, targetPoint, fbPoint);
            float ncc = computeNCC(srcFrame, srcPoint, targetFrame, targetPoint);
            FBDisplacement* fbDisp = new FBDisplacement(fwdDisplacement, bwdDisplacement, fbError, ncc);
            initialDisplacements.push_back(fbDisp);
        }
    }

    float medFBE = median(fbErrors);
    float medNCC = median(nccErrors);

    if (medFBE > FB_ERROR_LIMIT) {
        isValid = false;
    } else {
        displacementCount = (int) initialDisplacements.size();
        for (int i = 0; i < displacementCount; i++) {
            FBDisplacement* fbDisp = initialDisplacements[i];
            if (fbDisp->fbError <= medFBE && fbDisp->nccError >= medNCC) {
                displacementList.push_back(fbDisp->forward);
            }
        }

        displacementSize = (int) displacementList.size();

        if (displacementList.size() > 0) {
            isValid = true;
        } else {
            isValid = false;
        }
    }
}

Option<ScoredBox>* FBFlow::estimate(Box* box) {
    if (isValid) {
        Option<ScoredBox>* none = new Option<ScoredBox>();
        return none;
    } else {
        vector<float> dxList(displacementSize);
        vector<float> dyList(displacementSize);

        for (int i = 0; i < displacementSize; i++) {
            Displacement* disp = displacementList[i];
            dxList[i] = disp->dx;
            dyList[i] = disp->dy;
        }

        float medX = median(dxList);
        float medY = median(dyList);
        Box* movedBox = box->move(medX, medY);
        ScoredBox* scoredBox = new ScoredBox(movedBox);
        Option<ScoredBox>* maybeMovedBox = new Option<ScoredBox>(scoredBox);
        return maybeMovedBox;
    }
}
