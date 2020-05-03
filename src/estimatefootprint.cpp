#include <iostream>
#include <fstream>

#include "midterm-m10815822.hpp"

void EstimateFootPrint::readCoords()
{
    left_side.push_back(cv::Point2f(75,45));
    left_side.push_back(cv::Point2f(88,725));
    left_side.push_back(cv::Point2f(540,702));
    left_side.push_back(cv::Point2f(490,104));
   
    right_side.push_back(cv::Point2f(106,91));
    right_side.push_back(cv::Point2f(52,686));
    right_side.push_back(cv::Point2f(498,729));
    right_side.push_back(cv::Point2f(520,26));
    /*left_side.push_back(cv::Point2f(873,162));
    left_side.push_back(cv::Point2f(928,2709));
    left_side.push_back(cv::Point2f(2621,2626));
    left_side.push_back(cv::Point2f(2434,386));
   
    right_side.push_back(cv::Point2f(732,541));
    right_side.push_back(cv::Point2f(530,2766));
    right_side.push_back(cv::Point2f(2203,2932));
    right_side.push_back(cv::Point2f(2289,294));*/

    a4_size.push_back(cv::Point2f(0,0));
    a4_size.push_back(cv::Point2f(0,842));
    a4_size.push_back(cv::Point2f(595,842));
    a4_size.push_back(cv::Point2f(595,0));
    
}

void EstimateFootPrint::warping()
{
    cv::Mat left_img = cv::imread("../img/left-side-crop.jpg");
    cv::Mat right_img = cv::imread("../img/right-side-crop.jpg");
    //cv::Mat h1 = cv::findHomography(left_side, a4_size);
    //cv::Mat h2 = cv::findHomography(right_side, a4_size);
    cv::Mat h1 = cv::getPerspectiveTransform(left_side,a4_size);
    cv::Mat h2 = cv::getPerspectiveTransform(right_side,a4_size);
    std::cout <<"H1 = \n" << h1 << std::endl;
    std::cout <<"H2 = \n" << h2 << std::endl;
    cv::Mat img_out1;
    cv::Mat img_out2;

    cv::warpPerspective(left_img, img_out1, h1, cv::Size(595,842));
    cv::warpPerspective(right_img, img_out2, h2, cv::Size(595,842));

    cv::imwrite("Warped1.jpg",img_out1);
    cv::imwrite("Warped2.jpg",img_out2);
    cv::waitKey();
}

void EstimateFootPrint::merging()
{
    cv::Mat warp1 = cv::imread("Warped1.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat warp2 = cv::imread("Warped2.jpg", cv::IMREAD_GRAYSCALE);
    double thresh = 127;                //threshold value with 127 is fixed threholding value. 
    double maxValue = 255;              //maximum value that can be assigned out to the output of threholding
    cv::Mat mask1;
    cv::threshold(warp1, mask1, thresh, maxValue, cv::THRESH_BINARY_INV);
    cv::imwrite("Mask1.jpg",mask1);
    cv::Mat mask2;
    cv::threshold(warp2, mask2, thresh, maxValue, cv::THRESH_BINARY_INV);
    cv::imwrite("Mask2.jpg",mask2);
    cv::Mat merge;
    cv::bitwise_and(mask1,mask2,merge);
    cv::imwrite("merge.jpg",merge);
}

void EstimateFootPrint::exportSizeFootprint()
{
    cv::Mat merge1 = cv::imread("merge.jpg");
    cv::Mat gray, blur_image, threshold_output;
    cv::cvtColor(merge1, gray, cv::COLOR_BGR2GRAY);
    cv::blur(gray, blur_image, cv::Size(3, 3));
    cv::threshold(gray, threshold_output, 127, 255, cv::THRESH_BINARY);
    std::vector< std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(threshold_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0,0));
    std::vector< std::vector<cv::Point> > hull(contours.size());
    std::vector< cv::Rect> boundRect(contours.size());
    for (int i = 0; i < contours.size(); i++)
    {
        cv::convexHull(cv::Mat(contours[i]), hull[i], false);
        boundRect[i] = cv::boundingRect(hull[i]);
    }
    cv::Mat drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
    
    for (int i = 0; i < contours.size(); i++)
    {
        cv::Scalar color_contours = cv::Scalar(0,255,0);
        cv::Scalar color = cv::Scalar(255,0,0);
        cv::drawContours(drawing, contours, i, color_contours, 1, 8, std::vector<cv::Vec4i>(),0, cv::Point());
        //cv::drawContours(drawing, hull, i, color, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
        cv::rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color,1);    
    }
    cv::imshow("Merge",drawing);
    cv::imwrite("drawing.jpg",drawing);
    cv::waitKey();
    
}