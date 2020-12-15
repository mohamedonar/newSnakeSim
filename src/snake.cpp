#include <snake.h>

Snake::Snake(int tLimit, int lLimit, int rLimit, int dLimit, Adafruit_PCD8544* pdisplay)
{
    // Init snake
    pDisplay = pdisplay;
    topLimit = tLimit;
    leftLimit = lLimit;
    rightLimit = rLimit;
    downLimit = dLimit;

    Init();
}

Snake::~Snake()
{
}

void Snake::Init()
{
    Segments.clear();

    length = 5;
    segment firstSeg;
    firstSeg.dir = dRIGHT;
    firstSeg.nDirCoord = topLimit+1;
    firstSeg.nPmax = length;
    firstSeg.nPmin = 0;

    Segments.push_back(firstSeg);
}

pixel Snake::getSnakeHead()
{
    pixel p {0,0};
    int headSegPos = Segments.size()-1;
    switch (Segments[headSegPos].dir)
    {
    case dRIGHT:
        p.x = Segments[headSegPos].nPmax;
        p.y = Segments[headSegPos].nDirCoord;
        break;
    
    case dLEFT:
        p.x = Segments[headSegPos].nPmin;
        p.y = Segments[headSegPos].nDirCoord;
        break;
    
    case dUP:
        p.x = Segments[headSegPos].nDirCoord;
        p.y = Segments[headSegPos].nPmin;
        break;
    
    case dDOWN:
        p.x = Segments[headSegPos].nDirCoord;
        p.y = Segments[headSegPos].nPmax;
        break;   
    } 

    return p;
}

bool Snake::move(DIRECTION dir, bool bIncreaseLength)
{
//    bool bOkayToMove = true;
    int headSegPos = Segments.size()-1;
    DIRECTION curDir = Segments[headSegPos].dir;
    if(curDir==dir) {
        if(dir==dRIGHT || dir==dDOWN) {
//            bOkayToMove = (dir==dRIGHT) ? Segments[headSegPos].nPmax<rightLimit-1 : Segments[headSegPos].nPmax<downLimit-1;
//            if(bOkayToMove)
                Segments[headSegPos].nPmax++;
        } else {
//            bOkayToMove = (dir==dLEFT) ? Segments[headSegPos].nPmin>leftLimit+1 : Segments[headSegPos].nPmin>topLimit-1;
//            if(bOkayToMove)
                Segments[headSegPos].nPmin--;
        }
    } else {
        if((curDir==dRIGHT && dir==dLEFT) || (curDir==dLEFT && dir==dRIGHT) || (curDir==dUP && dir==dDOWN) || (curDir==dDOWN && dir==dUP))
            return false;

        segment newSeg;
        newSeg.dir = dir;
        newSeg.nDirCoord = (curDir==dRIGHT || curDir==dDOWN) ? Segments[headSegPos].nPmax : Segments[headSegPos].nPmin;
        newSeg.nPmax = newSeg.nPmin = Segments[headSegPos].nDirCoord; 

        if(dir==dRIGHT || dir==dDOWN) newSeg.nPmax++;
        if(dir==dLEFT || dir==dUP) newSeg.nPmin--;

        Segments.push_back(newSeg);
    }

    if(/*bOkayToMove && */ !bIncreaseLength) {
        if(Segments[0].dir==dRIGHT || Segments[0].dir==dDOWN) {
            Segments[0].nPmin++;
        } else {
            Segments[0].nPmax--;
        }

        if(Segments[0].nPmax==Segments[0].nPmin)
            Segments.erase(Segments.begin());            
    }

    return true; //bOkayToMove;
}

void Snake::draw()
{
    if(pDisplay==nullptr)
        return;

    for(size_t i=0; i<Segments.size(); i++)
    {
        auto segment=Segments[i];
        int x0,y0,x1,y1;
        if(segment.dir==dRIGHT || segment.dir==dLEFT) {
            y0=y1=segment.nDirCoord;
            x0=segment.nPmin;
            x1=segment.nPmax;
        } else {
            x0=x1=segment.nDirCoord;
            y0=segment.nPmin;
            y1=segment.nPmax;
        }

        pDisplay->drawLine(x0,y0,x1,y1,BLACK);
    }
}
