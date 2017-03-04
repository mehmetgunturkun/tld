#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cmath>

#include "common/Arguments.hpp"
#include "testbase/Sequence.hpp"
#include "util/ImageBuilder.hpp"
#include "common/Logging.hpp"

#include <opencv2/contrib/contrib.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#define PI 3.14159265358979323846
#define SIGMA 50

class Part {
public:
    double p;
    double theta;
    string locationKey;

    Part(double p, double theta) {
        this->p = p;
        this->theta = theta;
    }

    Part(Box* root, Box* part, string location) {
        this->locationKey = location;
        println("%s", location.c_str());
        println("%s", root->toCharArr());
        println("%s", part->toCharArr());
        double centerX = (root->x1 + root->x2) / 2.0;
        double centerY = (root->y1 + root->y2) / 2.0;

        double partX = (part->x1 + part->x2) / 2.0;
        double partY = (part->y1 + part->y2) / 2.0;

        this->p = sqrt((centerX - partX) * (centerX - partX) + (centerY - partY) * (centerY - partY));
        this->theta = atan2(partY - centerY , partX- centerX) * 180.0 / PI;

        println("P = %4.3f, Theta: %4.3f", p, theta);
    }
};

class ShapeModel {
public:
    Part* topLeft;
    Part* topRight;
    Part* bottomLeft;
    Part* bottomRight;
    Part* center;

    ShapeModel(Box* root, Box* topLeft, Box* topRight, Box* center, Box* bottomLeft, Box* bottomRight) {
        this->topLeft = new Part(root, topLeft, "TL");
        this->topRight = new Part(root, topRight, "TR");
        this->center = new Part(root, center, "CR");
        this->bottomLeft = new Part(root, bottomLeft, "BL");
        this->bottomRight = new Part(root, bottomRight, "BR");
    }
};

class Shape {
public:
    Box* topLeft;
    Box* topRight;
    Box* bottomLeft;
    Box* bottomRight;

    Box* center;

    Shape() {
        this->topLeft = nullptr;
        this->topRight = nullptr;
        this->bottomLeft = nullptr;
        this->bottomRight = nullptr;
        this->center = nullptr;
    }

    Shape(Box* tl, Box* tr, Box* c, Box* bl, Box* br) {
        this->topLeft = tl;
        this->topRight = tr;
        this->bottomLeft = bl;
        this->bottomRight = br;
        this->center = c;
    }
};

using namespace std;
int run(Arguments* arguments);

int main(int argc, char** args) {
    Arguments* arguments = new Arguments(argc, args);
    try {
        run(arguments);
    } catch(exception& e) {
        cout << "Standard exception: " << e.what() << endl;
        return 1;
    }
}

vector<Shape*> loadShapes(Sequence* sequence);
ShapeModel* loadShapeModel(Box* root);

vector<Box*> divideIntoStarShape(Box* box);
void displayHeatMap(Frame* frame, ShapeModel* model, Shape* shape);
void contributeToHeatmap(Mat* img, Mat* heatMap, Part* part, Box* box);

int run(Arguments* arguments) {
    string sequenceKey = arguments->getString("sequence");
    println(GREEN("Replaying Sequence(%s)"), sequenceKey.c_str());
    Sequence* sequence = new Sequence(sequenceKey);

    ShapeModel* model = loadShapeModel(sequence->initBox);
    vector<Shape*> shapes = loadShapes(sequence);

    int nrOfFrames = sequence->nrOfFrames;
    for (int i = 0; i < nrOfFrames; i++) {
        Frame* frame = sequence->next();
        Shape* shape = shapes[i];

        ImageBuilder* builder = new ImageBuilder(frame);
        // if (shape->topLeft != nullptr) {
        //     builder->withBox(shape->topLeft, Colors::YELLOW);
        // }
        //
        // if (shape->topRight != nullptr) {
        //     builder->withBox(shape->topRight, Colors::YELLOW);
        // }
        //
        // if (shape->center != nullptr) {
        //     builder->withBox(shape->center, Colors::RED);
        // }
        //
        // if (shape->bottomLeft != nullptr) {
        //     builder->withBox(shape->bottomLeft, Colors::YELLOW);
        // }
        //
        // if (shape->bottomRight != nullptr) {
        //     builder->withBox(shape->bottomRight, Colors::YELLOW);
        // }

        displayHeatMap(frame, model, shape);
        builder->display(5);
    }
    return 0;
}

vector<Shape*> loadShapes(Sequence* sequence) {
    vector<Shape*> shapes;

    int nrOfFrames = sequence->nrOfFrames;
    string sequenceDir = sequence->dir;

    string tl = sequenceDir + "/evaluations/mpt/tl.txt";
    ifstream tlFile(tl);
    tlFile.is_open();
    string topLeft = "";

    string tr = sequenceDir + "/evaluations/mpt/tr.txt";
    ifstream trFile(tr);
    string topRight = "";

    string c = sequenceDir + "/evaluations/mpt/c.txt";
    ifstream cFile(c);
    string center = "";

    string bl = sequenceDir + "/evaluations/mpt/bl.txt";
    ifstream blFile(bl);
    string bottomLeft = "";

    string br = sequenceDir + "/evaluations/mpt/br.txt";
    ifstream brFile(br);
    string bottomRight = "";

    for (int i = 0; i < nrOfFrames; i++) {
        getline(tlFile, topLeft);
        Option<Box>* boxTopLeft = Box::parseFromLine(topLeft);

        getline(trFile, topRight);
        Option<Box>* boxTopRight = Box::parseFromLine(topRight);

        getline(cFile, center);
        Option<Box>* boxCenter = Box::parseFromLine(center);

        getline(blFile, bottomLeft);
        Option<Box>* boxBottomLeft = Box::parseFromLine(bottomLeft);

        getline(brFile, bottomRight);
        Option<Box>* boxBottomRight = Box::parseFromLine(bottomRight);

        // println("TL: %s", topLeft.c_str());
        // println("TR: %s", topRight.c_str());
        // println("RT: %s", center.c_str());
        // println("BL: %s", bottomLeft.c_str());
        // println("BR: %s", bottomRight.c_str());

        // Frame* frame = sequence->next();
        // ImageBuilder* builder = new ImageBuilder(frame);

        Shape* shape = new Shape();
        if (boxTopLeft->isDefined()) {
            shape->topLeft = boxTopLeft->get();
            // builder->withBox(boxTopLeft->get(), Colors::YELLOW);
        }

        if (boxTopRight->isDefined()) {
            shape->topRight = boxTopRight->get();
            // builder->withBox(boxTopRight->get(), Colors::YELLOW);
        }

        if (boxCenter->isDefined()) {
            shape->center = boxCenter->get();
            // builder->withBox(boxCenter->get(), Colors::YELLOW);
        }

        if (boxBottomLeft->isDefined()) {
            shape->bottomLeft = boxBottomLeft->get();
            // builder->withBox(boxBottomLeft->get(), Colors::YELLOW);
        }

        if (boxBottomRight->isDefined()) {
            shape->bottomRight = boxBottomRight->get();
            // builder->withBox(boxBottomRight->get(), Colors::YELLOW);
        }
        // builder->display(1);

        shapes.push_back(shape);
    }
    return shapes;
}

ShapeModel* loadShapeModel(Box* box) {
    double x1 = box->x1;
    double y1 = box->y1;

    double x2 = box->x2;
    double y2 = box->y2;

    double midX = (x1 + x2) / 2.0;
    double midY = (y1 + y2) / 2.0;

    double x11 = (x1 + midX) / 2.0;
    double x12 = (midX + x2) / 2.0;

    double y11 = (y1 + midY) / 2.0;
    double y12 = (midY + y2) / 2.0;

   ShapeModel* model = new ShapeModel(
              box,
              new Box(0, x1, y1, midX, midY),
              new Box(1, midX, y1, x2, midY),
              new Box(4, x11, y11, x12, y12),
              new Box(2, x1, midY, midX, y2),
              new Box(3, midX, midY, x2, y2)
          );

    return model;
}

void displayHeatMap(Frame* frame, ShapeModel* model, Shape* shape) {
    Mat* img = frame->displayImg;
    Mat* heatMap = new Mat(frame->height, frame->width, CV_64F, 0.0);
    // frame->grayscale->copyTo(*heatMap);

    if (shape->topLeft != nullptr) {
        // contribute to heatmap
        contributeToHeatmap(img, heatMap, model->topLeft, shape->topLeft);
    }

    if (shape->topRight != nullptr) {
        contributeToHeatmap(img, heatMap, model->topRight, shape->topRight);
    }

    if (shape->center != nullptr) {
        contributeToHeatmap(img, heatMap, model->center, shape->center);
    }

    if (shape->bottomLeft != nullptr) {
        contributeToHeatmap(img, heatMap, model->bottomLeft, shape->bottomLeft);
    }

    if (shape->bottomRight != nullptr) {
        contributeToHeatmap(img, heatMap, model->bottomRight, shape->bottomRight);
    }

    int width = heatMap->cols;
    int height = heatMap->rows;

    int maxX = 0;
    int maxY = 0;
    double maxPixelValue = 0.0;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double pixelValue = heatMap->at<double>(y, x);
            if (pixelValue > maxPixelValue) {
                maxX = x;
                maxY = y;
                maxPixelValue = pixelValue;
            }
        }
    }

    Mat colorMap;
    applyColorMap(*heatMap, colorMap, COLORMAP_JET);
    cv::imshow("HM", colorMap);
    cv::waitKey(5);
    println("Max(%d, %d) = %4.3f", maxX, maxY, maxPixelValue);
}

void contributeToHeatmap(Mat* img, Mat* heatMap, Part* part, Box* box) {

    Mat* realImg = new Mat();
    img->copyTo(*realImg);

    double centerX = (box->x1 + box->x2) / 2.0;
    double centerY = (box->y1 + box->y2) / 2.0;

    int x = (int) ceil(centerX + part->p * cos(part->theta));
    int y = (int) ceil(centerY + part->p * sin(part->theta));

    println("%s, P = %4.3f, Theta = %4.3f", part->locationKey.c_str(), part->p, part->theta);

    double constantContribution = 1.0 / (2 * PI * SIGMA * SIGMA);

    for (int i = y - 50; i < y + 50; i++) {
        for (int j = x - 50; j < x + 50; j++) {
            int dy = abs(i - y);
            int dx = abs(j - x);
            double distance = sqrt(dx * dx + dy * dy);
            double contribution = 10 / (distance + 1.0);

            if (i == y && j == x) {
                println("%4.3f", contribution);
            }


            heatMap->at<double>(i, j) = heatMap->at<double>(i, j) + contribution;
        }
    }

    // cv::rectangle(
    //     *realImg,
    //     Point2i(box->x1, box->y1),
    //     Point2i(box->x2 ,box->y2),
    //     Colors::RED, 1, 8, 0
    // );
    //
    // cv::rectangle(
    //     *realImg,
    //     Point2i(x - 50, y - 50),
    //     Point2i(x + 50 ,y + 50),
    //     Colors::BLUE, 1, 8, 0
    // );
    //
    // cv::imshow("heatmap" + part->locationKey, *realImg);
    // cv::waitKey(0);

}
