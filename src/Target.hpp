#ifndef TARGET_CLASS_H
#define TARGET_CLASS_H
#include <opencv2/opencv.hpp>
using namespace cv;

class Target
{
public:
    Target(std::vector<std::vector<cv::Point> > contour);
    Target(std::vector<cv::Point> contour);
    double getHeight();
    double getWidth();
    void printPoints();
    Point getCenter();
    void setTar(bool tar);
    bool getType();
    bool getTar();
    Point getTopPoint();
    Point getBottomPoint();
    Point getLeftPoint();
    Point getRightPoint();
    //bool isInitialized();
    //int crow;
private:
    Point getLeftRightPoint();
    Point getRightLeftPoint();
    std::vector<Point> edgeL;
    std::vector<Point> edgeR;
    bool Tar;
};

#endif
