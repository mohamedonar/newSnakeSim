#include <snake.h>
#include <JoystickShield.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(9, 10, 11, 12, 13);
JoystickShield joystickShield;

pixel seed {0,0};

uint8_t const screenLeft = 0;
uint8_t const screenTop = 8;
uint8_t const screenRight = display.width();
uint8_t const screenBottom = display.height();

Snake snake = Snake(screenTop,screenLeft,screenRight-1,screenBottom-1,&display);
bool bIncreaseLength = false;
uint8_t score = 0;
DIRECTION dir = dRIGHT;

void dispalyScore()
{
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(0,0);
    display.print(score);
}

bool PixelIsBlack(pixel p)
{
    return display.getPixel(p.x,p.y) == BLACK;
}

pixel getRandSeed()
{
    pixel p;

    do {
        p.x = screenLeft + (rand() % screenRight);
        p.y = screenTop + (rand() + screenBottom);
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

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);

    //Initialize Display
    display.begin();
    display.setContrast(57);

    seed = getRandSeed();

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

    srand(0);
}

void loop() {
    display.clearDisplay();

    joystickShield.processCallbacks();

    display.drawRect(screenLeft, screenTop, screenRight-screenLeft, screenBottom-screenTop, BLACK);
    dispalyScore();
    display.drawPixel(seed.x, seed.y, BLACK);
    snake.draw();

    delay(200);

    bIncreaseLength = rand() % 2;
    snake.move(dir,bIncreaseLength);
}
