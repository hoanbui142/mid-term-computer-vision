#pragma once


#include <map>
#include <string>
#include <vector>

#include "opencv2/opencv.hpp"

class EstimateFootPrint
{
public:
    std::vector<cv::Point2f> left_side;
    std::vector<cv::Point2f> right_side;
    std::vector<cv::Point2f> a4_size;

    void readCoords();
    void warping();
    void merging();
    void exportSizeFootprint();
};