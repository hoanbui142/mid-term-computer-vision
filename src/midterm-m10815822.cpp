#include "midterm-m10815822.hpp"

int main(int argc, char const *argv[])
{
    EstimateFootPrint twoviews;
    twoviews.readCoords();
    twoviews.warping();
    twoviews.merging();

    return 0;
}