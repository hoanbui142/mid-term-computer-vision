#include <iostream>
#include <fstream>
#include <sstream>

#include "midterm-m10815822.hpp"

void EstimateFootPrint::readCoords()
{
    ////Correspondences points of left-side images
    left_side.push_back(cv::Point2f(75, 45));   //Correspondences top-left corner
    left_side.push_back(cv::Point2f(88, 725));  //Correspondences bottom-left corner
    left_side.push_back(cv::Point2f(540, 702)); //Correspondences bottom-right corner
    left_side.push_back(cv::Point2f(490, 104)); //Correspondences top-right corner

    ////Correspondences points of right-side images
    right_side.push_back(cv::Point2f(106, 91));  //Correspondences top-left corner
    right_side.push_back(cv::Point2f(52, 686));  //Correspondences bottom-left corner
    right_side.push_back(cv::Point2f(498, 729)); //Correspondences bottom-right corner
    right_side.push_back(cv::Point2f(520, 26));  //Correspondences top-right corner

    ////Destination points - A4 72DPI pixel points (595 pixels x 842 pixels)
    a4_size.push_back(cv::Point2f(0, 0));
    a4_size.push_back(cv::Point2f(0, 842));
    a4_size.push_back(cv::Point2f(595, 842));
    a4_size.push_back(cv::Point2f(595, 0));
}

void EstimateFootPrint::warping()
{
    cv::Mat left_img = cv::imread("left-side-A4.jpg");
    cv::Mat right_img = cv::imread("right-side-A4.jpg");
    //cv::Mat h1 = cv::findHomography(left_side, a4_size);
    //cv::Mat h2 = cv::findHomography(right_side, a4_size);
    cv::Mat h1 = cv::getPerspectiveTransform(left_side, a4_size);
    cv::Mat h2 = cv::getPerspectiveTransform(right_side, a4_size);
    //std::cout <<"H1 = \n" << h1 << std::endl;
    //std::cout <<"H2 = \n" << h2 << std::endl;
    cv::Mat img_out1;
    cv::Mat img_out2;

    cv::warpPerspective(left_img, img_out1, h1, cv::Size(595, 842));
    cv::warpPerspective(right_img, img_out2, h2, cv::Size(595, 842));

    cv::imwrite("Warped1.jpg", img_out1);
    cv::imwrite("Warped2.jpg", img_out2);
    cv::waitKey();
}

void EstimateFootPrint::merging()
{
    cv::Mat warp1 = cv::imread("Warped1.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat warp2 = cv::imread("Warped2.jpg", cv::IMREAD_GRAYSCALE);
    double thresh = 127;   //threshold value with 127 is fixed threholding value.
    double maxValue = 255; //maximum value that can be assigned out to the output of threholding
    cv::Mat mask1;
    cv::threshold(warp1, mask1, thresh, maxValue, cv::THRESH_BINARY_INV);
    cv::imwrite("Mask1.jpg", mask1);
    cv::Mat mask2;
    cv::threshold(warp2, mask2, thresh, maxValue, cv::THRESH_BINARY_INV);
    cv::imwrite("Mask2.jpg", mask2);
    cv::Mat merge;
    cv::bitwise_and(mask1, mask2, merge);
    cv::imwrite("Merged.jpg", merge);
}

std::string toString(double number)
{
    std::ostringstream buff;
    buff << number;
    return buff.str();
}

void EstimateFootPrint::exportSizeFootprint()
{
    //Convert image to grayscale, blur it slightly to get rid of the noise.
    cv::Mat merge1 = cv::imread("Merged.jpg");
    cv::Mat gray, blur_image, threshold_output;
    cv::cvtColor(merge1, gray, cv::COLOR_BGR2GRAY);
    cv::blur(gray, blur_image, cv::Size(3, 3));
    cv::threshold(gray, threshold_output, 127, 255, cv::THRESH_BINARY);

    //Draw the contour, the bounding rectangle, remove noise contours
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(threshold_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
    std::vector<std::vector<cv::Point>> contours_poly(contours.size());
    std::vector<cv::Rect> boundRect(contours.size());
    cv::Mat drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
    double width, height;

    for (int i = 0; i < contours.size(); i++)
    {
        double area = cv::contourArea(contours[i]);
        //std::cout<<"Dien tich "<<i<<": "<<area<<std::endl;
        if (area > 100)
        {
            cv::approxPolyDP(contours[i], contours_poly[i], 3, true);
            boundRect[i] = cv::boundingRect(contours_poly[i]);
            cv::Scalar color_contours = cv::Scalar(255, 255, 255);
            cv::Scalar color = cv::Scalar(0, 255, 0);
            cv::drawContours(drawing, contours_poly, i, color_contours, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
            cv::rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 1);
            cv::Point sizeRect = boundRect[i].br() - boundRect[i].tl();
            width = sizeRect.x / 72.0 * 2.54;    
            height = sizeRect.y/ 72.0 * 2.54;    
            std::cout << "Width of Foot-print is: " << width << "cm" << std::endl;
            std::cout << "Height of Foot-print is: " << height << "cm" << std::endl;
            cv::putText(drawing, toString(width) + "cm", boundRect[i].tl(), cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 0, 255, 255));
            cv::putText(drawing, toString(height) + "cm", cv::Point(boundRect[i].tl().x - 160, boundRect[i].tl().y + sizeRect.y / 2), cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 0, 255, 255));
        }
    }
    cv::imshow("Merge", drawing);
    cv::imwrite("M10815822.jpg", drawing);
    cv::waitKey();
}