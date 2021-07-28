// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_GFX.h>

// This #include statement was automatically added by the Particle IDE.
#include <neomatrix.h>

// This #include statement was automatically added by the Particle IDE.
#include <neopixel.h>

#include <math.h>

#define PIXEL_PIN D6
//  1553
//  1  3
//  1  3
//  1  3
//  1  3
//  1  3
//  1  3
//  B66B    <-- each number line defines the number value of the digit
//  2  4
//  2  4
//  2  4
//  2  4
//  2  4
//  2  4
//  2774
int nbrSegs[10][7] = {
    //for each number value, define what segments from above
    {1, 2, 3, 4, 5, 7}, //are required to create a number
    {3, 4},
    {2, 3, 5, 6, 7}, //this allows for scalable numbers and pixel grid sizes
    {3, 4, 5, 6, 7},
    {1, 3, 4, 6},
    {1, 4, 5, 6, 7},
    {1, 2, 4, 5, 6, 7},
    {3, 4, 5},
    {1, 2, 3, 4, 5, 6, 7},
    {1, 3, 4, 5, 6, 7},
};

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(20, 15, 1, 1, PIXEL_PIN,
                                               NEO_TILE_TOP + NEO_TILE_LEFT + NEO_TILE_ROWS + NEO_TILE_PROGRESSIVE +
                                                   NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG); //setup your neopixel grid here

int maxDisplacement;
int pixelPerChar = 6;
bool regularClock = false;
bool partyClock = true;
bool alarmFlash = false;

const uint16_t colors[] = {
    matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255)};

void setup()
{
    Time.zone(-6);
    Time.beginDST();
    matrix.begin();
    matrix.setTextWrap(false);
    matrix.setBrightness(255);
    matrix.setTextColor(matrix.Color(0, 0, 0));
    Particle.function("showRegularClock", showRegularClock); //expose functions so they can be used by IFTTT or web request
    Particle.function("showPartyClock", showPartyClock);
    Particle.function("showNothing", showNothing);
    Particle.function("showAlarm", showAlarm);
}

int x = matrix.width();
int y = matrix.height();

int wheelPos = 0;
void loop()
{
    if (partyClock)
    {
        //make whatever background you want here
        matrix.fillScreen(Wheel(wheelPos));
        wheelPos++;
        if (wheelPos > 256)
        {
            wheelPos = 0;
        }

        //show the time
        int hr = Time.hour();
        int min = Time.minute();

        int hr1 = 0;
        if (hr > 9)
        {
            hr1 = ExtractDigit(hr, 2);
        }
        int hr2 = ExtractDigit(hr, 1);
        int min1 = 0;
        if (min > 9)
        {
            min1 = ExtractDigit(min, 2);
        }
        int min2 = ExtractDigit(min, 1);

        //draw each digit
        DrawDigit(1, 1, 2, 12, hr1, matrix.Color(0, 0, 0));
        DrawDigit(5, 1, 3, 12, hr2, matrix.Color(0, 0, 0));
        DrawDigit(10, 1, 3, 7, min1, matrix.Color(0, 0, 0));
        DrawDigit(15, 1, 3, 7, min2, matrix.Color(0, 0, 0));
    }
    else if (regularClock)
    {
        //make whatever background you want here
        matrix.fillScreen(matrix.Color(0, 0, 0));

        //show the time
        int hr = Time.hour();
        int min = Time.minute();

        int hr1 = 0;
        if (hr > 9)
        {
            hr1 = ExtractDigit(hr, 2);
        }
        int hr2 = ExtractDigit(hr, 1);
        int min1 = 0;
        if (min > 9)
        {
            min1 = ExtractDigit(min, 2);
        }
        int min2 = ExtractDigit(min, 1);

        //draw each digit
        DrawDigit(1, 1, 2, 12, hr1, matrix.Color(255, 255, 255));
        DrawDigit(5, 1, 3, 12, hr2, matrix.Color(255, 255, 255));
        DrawDigit(10, 1, 3, 7, min1, matrix.Color(255, 255, 255));
        DrawDigit(15, 1, 3, 7, min2, matrix.Color(255, 255, 255));

        matrix.show();
    }
    else if (alarmFlash)
    {
        wheelPos++;
        if (wheelPos > 256)
        {
            wheelPos = 0;
        }
        matrix.fillScreen(matrix.Color(0, 0, 0));
        if (wheelPos < 32 || (wheelPos > 64 && wheelPos < 96) || (wheelPos > 128 && wheelPos < 160) || (wheelPos > 192 && wheelPos < 224))
        {
            matrix.fillScreen(matrix.Color(255, 0, 0));
        }
    }
    else
    {
        matrix.fillScreen(matrix.Color(0, 0, 0));
    }

    matrix.show();
    delay(10);
}

void DrawDigit(int startX, int startY, int width, int height, int nbr, uint16_t color)
{
    for (int i = 0; i < 7; ++i)
    {
        int seg = nbrSegs[nbr][i];
        switch (seg)
        {
        case 0:
            break;
        case 1:
            for (int p = 0; p <= height / 2; p++)
            {
                matrix.drawPixel(0 + startX, p + startY, color);
            }
            break;
        case 2:
            for (int p = height / 2; p <= height; p++)
            {
                matrix.drawPixel(0 + startX, p + startY, color);
            }
            break;
        case 3:
            for (int p = 0; p <= height / 2; p++)
            {
                matrix.drawPixel(width + startX, p + startY, color);
            }
            break;
        case 4:
            for (int p = height / 2; p <= height; p++)
            {
                matrix.drawPixel(width + startX, p + startY, color);
            }
            break;
        case 5:
            for (int p = 0; p <= width; p++)
            {
                matrix.drawPixel(p + startX, 0 + startY, color);
            }
            break;
        case 6:
            for (int p = 0; p <= width; p++)
            {
                matrix.drawPixel(p + startX, (height / 2) + startY, color);
            }
            break;
        case 7:
            for (int p = 0; p <= width; p++)
            {
                matrix.drawPixel(p + startX, height + startY, color);
            }
            break;
        default:
            break;
        }
    }
}

int showRegularClock(String args)
{
    regularClock = true;
    partyClock = false;
    alarmFlash = false;

    return 1;
}

int showPartyClock(String args)
{
    regularClock = false;
    partyClock = true;
    alarmFlash = false;

    return 1;
}

int showAlarm(String args)
{
    regularClock = false;
    partyClock = false;
    alarmFlash = true;

    return 1;
}

int showNothing(String args)
{
    regularClock = false;
    partyClock = false;
    alarmFlash = false;

    return 1;
}

int ExtractDigit(int V, int P)
{
    return int(V / (pow(10, P - 1))) - int(V / (pow(10, P))) * 10;
}

uint32_t Wheel(byte WheelPos)
{
    if (WheelPos < 85)
    {
        return matrix.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
    else if (WheelPos < 170)
    {
        WheelPos -= 85;
        return matrix.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    else
    {
        WheelPos -= 170;
        return matrix.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
}