#ifndef SNAKE_H
#define SNAKE_H

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <vector>

#include <commonDefs.h>

class Snake
{
private:
    int length;
    int topLimit, leftLimit, rightLimit,downLimit;
    std::vector<segment> Segments;
    Adafruit_PCD8544* pDisplay;

public:
    Snake(int tLimit, int lLimit, int rLimit, int dLimit, Adafruit_PCD8544* pdisplay = nullptr);
    ~Snake();

    void Init();
    bool move(DIRECTION dir, bool bIncreaseLength=false);
    pixel getSnakeHead();
    void draw();
};

#endif
