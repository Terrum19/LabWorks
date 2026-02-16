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
    }
    Rect(){
        leftBorderX = 0;
        rightBorderX = 0;
        upperBorderY = 0;
        lowerBorderY = 0;
    }
};