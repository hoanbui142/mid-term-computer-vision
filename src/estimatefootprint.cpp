#include <iostream>
#include <fstream>

#include "midterm-m10815822.hpp"

void EstimateFootPrint::readCoords()
{
    left_side.push_back(cv::Point2f(73,44));
    left_side.push_back(cv::Point2f(88,725));
    left_side.push_back(cv::Point2f(540,702));
    left_side.push_back(cv::Point2f(490,104));
   
    right_side.push_back(cv::Point2f(106,91));
    right_side.push_back(cv::Point2f(52,686));
    right_side.push_back(cv::Point2f(499,730));
    right_side.push_back(cv::Point2f(522,25));
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
    //cv::Mat h = cv::findHomography(left_side,right_side);
    //cv::Mat h1 = cv::findHomography(left_side, a4_size);
    //cv::Mat h2 = cv::findHomography(right_side, a4_size);
    //cv::Mat h = cv::getPerspectiveTransform(left_side,right_side);
    cv::Mat h1 = cv::getPerspectiveTransform(left_side,a4_size);
    cv::Mat h2 = cv::getPerspectiveTransform(right_side,a4_size);
    //std::cout <<"H = \n" << h << std::endl;
    //std::cout <<"H1 = \n" << h1 << std::endl;
    //std::cout <<"H2 = \n" << h2 << std::endl;
    //cv::Mat img_out;
    cv::Mat img_out1;
    cv::Mat img_out2;

    //cv::warpPerspective(left_img, img_out, h, right_img.size());
    cv::warpPerspective(left_img, img_out1, h1, left_img.size());
    cv::warpPerspective(right_img, img_out2, h2, right_img.size());

    //cv::imshow("Warped Source Image:", img_out1);
    //cv::imwrite("Warped.jpg",img_out);
    cv::imwrite("Warped1.jpg",img_out1);
    cv::imwrite("Warped2.jpg",img_out2);
    cv::waitKey();
}

void EstimateFootPrint::merging()
{
    cv::Mat mask1 = cv::imread("mask-left-side.jpg");
    cv::Mat mask2 = cv::imread("mask-right-side.jpg");
    cv::Mat merge;
    cv::bitwise_and(mask1,mask2,merge);
    cv::imwrite("merge.jpg",merge);
    
}