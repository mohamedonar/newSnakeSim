#include <snake.h>
#include <JoystickShield.h>

#define LENGTH_NCREASE  10
uint8_t increaseCounter = 0;
uint8_t speedDelay = 100;

Adafruit_PCD8544 display = Adafruit_PCD8544(9, 10, 11, 12, 13);
JoystickShield joystickShield;

pixel seed {0,0};

uint8_t const screenLeft = 0;
uint8_t const screenTop = 8;
uint8_t const screenRight = display.width();
uint8_t const screenBottom = display.height();

Snake snake = Snake(screenTop,screenLeft,screenRight-1,screenBottom-1,&display);
bool bCanMove = true;
uint8_t score = 0;
DIRECTION dir = dRIGHT;

void dispalyHeader(uint8_t info1, uint8_t info2 = 0)
{
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(0,0);
    display.print(info1);
    if(info2!=0)
    {
        display.print(' ');
        display.print(info2);
    }
    display.display();
}

bool PixelIsBlack(pixel p)
{
    return display.getPixel(p.x,p.y) == BLACK;
}

pixel getRandSeed(uint8_t avoidSnakeInit = 0)
{
    pixel p;

    do {
        p.x = screenLeft + (rand() % (screenRight-screenLeft));
        p.y = screenTop + avoidSnakeInit + (rand() % (screenBottom-screenTop));
    } while (PixelIsBlack(p));

    return p;
}

void JSup()
{
    dir = dUP;
}

void JSdown()
{
    dir = dDOWN;
}

void JSright()
{
    dir = dRIGHT;
}

void JSleft()
{
    dir = dLEFT;
}

void JSFbutton()
{
    snake.Init();
    bCanMove = true;
    increaseCounter = 0;
    speedDelay = 100;
    score = 0;
    dir = dRIGHT;
    seed = getRandSeed(1);
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);

    //Initialize Display
    display.begin();
    display.setContrast(57);

    // Joystick Init & Callbacks
    joystickShield.calibrateJoystick();
    joystickShield.onJSUp(&JSup);
    joystickShield.onJSRight(&JSright);
    joystickShield.onJSDown(&JSdown);
    joystickShield.onJSLeft(&JSleft);
    joystickShield.onUpButton(&JSup);
    joystickShield.onRightButton(&JSright);
    joystickShield.onDownButton(&JSdown);
    joystickShield.onLeftButton(&JSleft);
    joystickShield.onFButton(&JSFbutton);

    srand(0);
    seed = getRandSeed(1);
}

void loop() {
    joystickShield.processCallbacks();

    if(bCanMove)
    {
        display.clearDisplay();

        display.drawRect(screenLeft, screenTop, screenRight-screenLeft, screenBottom-screenTop, BLACK);
        dispalyHeader(score);
        snake.draw();

        display.drawPixel(seed.x, seed.y, BLACK);
        display.display();

        delay(speedDelay);

        snake.move(dir, increaseCounter!=0);

        pixel p = snake.getSnakeHead();
        if(p.x==seed.x && p.y==seed.y)
        {    
            score++;
            if(score % 7 == 0 && speedDelay > 25)
                speedDelay-=25; 

            increaseCounter = LENGTH_NCREASE;
            seed = getRandSeed();
         }
        else
        {
            if(increaseCounter!=0)
                increaseCounter--;

            bCanMove = !PixelIsBlack(p);
        }
    }
}
