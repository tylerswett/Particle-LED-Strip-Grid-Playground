
// This #include statement was automatically added by the Particle IDE.
#include <neomatrix.h>

// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_GFX.h>

// This #include statement was automatically added by the Particle IDE.
#include <neopixel.h>

#define PIXEL_PIN D6

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(20, 15, 1, 1, PIXEL_PIN,
                                               NEO_TILE_TOP + NEO_TILE_LEFT + NEO_TILE_ROWS + NEO_TILE_PROGRESSIVE +
                                                   NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG); //setup your neopixel grid here

const uint16_t colors[] = {
    matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255)};

void setup()
{
    matrix.begin();
    matrix.setTextWrap(false);
    matrix.setBrightness(255);
    matrix.setTextColor(matrix.Color(0, 0, 0));
    Particle.function("setPixel", setPixel); //expose functions so they can be used by IFTTT or web request
    Particle.function("clearBoard", clearBoard);
}

void loop()
{
}

int setPixel(String args)
{
    int pixelX = args.substring(0, 3).toInt();
    int pixelY = args.substring(3, 6).toInt();
    int pixelR = args.substring(6, 9).toInt();
    int pixelG = args.substring(9, 12).toInt();
    int pixelB = args.substring(12, 15).toInt();

    matrix.drawPixel(pixelX, pixelY, matrix.Color(pixelR, pixelG, pixelB));
    matrix.show();

    return 1;
}

int clearBoard(String args)
{
    matrix.fillScreen(matrix.Color(0, 0, 0));
    matrix.show();

    return 1;
}