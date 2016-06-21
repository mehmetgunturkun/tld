#include "tracker/FBFlow.hpp"

float FBFlow::FB_ERROR_LIMIT = 0.5;

float computeFBError(tld::Point* src, tld::Point* target, tld::Point* fb) {
    float dy = fb->underlying.y - src->underlying.y;
    float dx = fb->underlying.x - src->underlying.x;
    float fbe = sqrt(dx*dx + dy*dy);
    return fbe;
}

float computeNCC(Frame* srcFrame, tld::Point* srcPoint, Frame* targetFrame, tld::Point* targetPoint) {
    //TODO Might need to write own ncc computation
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
    Frame* srcFrame = forward->source;
    Frame* targetFrame = forward->target;

    for (int i = 0; i < displacementCount; i++) {
        Displacement* fwdDisplacement = forwardDisplacements[i];
        Displacement* bwdDisplacement = backwardDisplacements[i];

        if (fwdDisplacement->state && bwdDisplacement->state) {
            tld::Point* srcPoint = fwdDisplacement->source;
            tld::Point* targetPoint = fwdDisplacement->target;
            tld::Point* fbPoint = bwdDisplacement->target;

            float fbError = computeFBError(srcPoint, targetPoint, fbPoint);
            float ncc = computeNCC(srcFrame, srcPoint, targetFrame, targetPoint);

            fbErrors.push_back(fbError);
            nccErrors.push_back(ncc);

            FBDisplacement* fbDisp = new FBDisplacement(fwdDisplacement, bwdDisplacement, fbError, ncc);
            initialDisplacements.push_back(fbDisp);
        }
    }

    float medFBE = median(fbErrors);
    float medNCC = median(nccErrors);
    if (medFBE > FB_ERROR_LIMIT) {
        println("Invalid Box -- high fb error - %f", medFBE);
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

Option<Box>* FBFlow::estimate(Box* box) {
    if (!isValid) {
        Option<Box>* none = new Option<Box>();
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
        Option<Box>* maybeMovedBox = new Option<Box>(movedBox);
        return maybeMovedBox;
    }
}
