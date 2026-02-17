#include <iostream>

#define RED_TEXT "\033[31m"
#define GREEN_TEXT "\033[32m"
#define YELLOW_TEXT "\033[33m"
#define COLOR_TEXT_FINISH "\033[0m"

class Rect{
    private:
        int leftBorderX;
        int rightBorderX;
        int upperBorderY;
        int lowerBorderY;
    public:
        Rect(int inpLeftBorderX, int inpRightBorderX, int inpLowerBorderX, int inpUpperBorderX)
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
        void printBorders(){
            std::cout << leftBorderX << " " << rightBorderX << " " << lowerBorderY << " " << upperBorderY << std::endl;
        }
        int getLeft(){
            return this->leftBorderX;
        }
        int getRight(){
            return this->rightBorderX;
        }
        int getTop(){
            return this->upperBorderY;
        }
        int getBottom(){
            return this->lowerBorderY;
        }
        void setAll(int rightBorderX, int leftBorderX, int lowerBorderY, int upperBorderY){
            this->rightBorderX = rightBorderX;
            this->leftBorderX = leftBorderX;
            this->lowerBorderY = lowerBorderY;
            this->upperBorderY = upperBorderY;
        }
        void inflate(int amount){
            rightBorderX += amount;
            leftBorderX -= amount;
            upperBorderY += amount;
            lowerBorderY -= amount;
        }
        void inflate(int dh, int dw){
            upperBorderY += dh;
            lowerBorderY -= dh;
            rightBorderX += dw;
            leftBorderX -= dw;
        }
        void inflate(int dLeft, int dRight, int dTop, int dBottom){
            upperBorderY += dTop;
            lowerBorderY -= dBottom;
            leftBorderX -= dLeft;
            rightBorderX += dRight;
        }
        void move(int dX) {
            rightBorderX += dX;
            leftBorderX += dX;
        }
        void move(int dX, int dY){
            rightBorderX += dX;
            leftBorderX += dX;
            lowerBorderY += dY;
            upperBorderY += dY;
        }
};