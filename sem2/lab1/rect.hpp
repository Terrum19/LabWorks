#include <iostream>

#define RED_TEXT "\033[31m"
#define GREEN_TEXT "\033[32m"
#define YELLOW_TEXT "\033[33m"
#define COLOR_TEXT_FINISH "\033[0m"

class Rect{
    public:
        int leftBorderX;
        int rightBorderX;
        int upperBorderY;
        int lowerBorderY;
    Rect(int inpLeftBorderX, int inpRightBorderX, int inpUpperBorderX, int inpLowerBorderX)
    {
        leftBorderX = inpLeftBorderX;
        rightBorderX = inpRightBorderX;
        upperBorderY = inpUpperBorderX;
        lowerBorderY = inpLowerBorderX;
        std::cout << GREEN_TEXT << "Initialisation with given values constructor called. " << "Object address: " << this << " " << leftBorderX << " " << rightBorderX << " " << upperBorderY << " " << lowerBorderY << COLOR_TEXT_FINISH <<std::endl;
    }
    Rect(){
        leftBorderX = 0;
        rightBorderX = 0;
        upperBorderY = 0;
        lowerBorderY = 0;
        std::cout << YELLOW_TEXT << "Default Initialisation Constructor called. " << "Object address: " << this << " " << leftBorderX << " " << rightBorderX << " " << upperBorderY << " " << lowerBorderY << COLOR_TEXT_FINISH << std::endl;
    }
    ~Rect(){
        std::cout << RED_TEXT << "Destructor called. " << "Object address: " << this << " " << leftBorderX << " " << rightBorderX << " " << upperBorderY << " " << lowerBorderY << COLOR_TEXT_FINISH << std::endl;
    }
    Rect(const Rect & other) {
        leftBorderX = other.leftBorderX;
        rightBorderX = other.rightBorderX;
        upperBorderY = other.upperBorderY;
        lowerBorderY = other.lowerBorderY;
        std::cout << "Copying constructor called. Copying into " << this << " from " << &other << std::endl;
    }
};