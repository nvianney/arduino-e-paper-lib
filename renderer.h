#ifndef renderer_h
#define renderer_h

#include "eink_display.h"
#include "binary_matrix.h"

enum DisplayColor {
    BLACK, RED
};

class Renderer {

    BinaryMatrix redData;
    BinaryMatrix blackData;
    EInkDisplay display;

    DisplayColor color = DisplayColor::BLACK;
    int borderWidth = 1;

    // Bounds of rendering
    int _minX;
    int _minY;
    int _maxX;
    int _maxY;
    bool dirty = false;

public:
    Renderer(int width, int height);

    void drawRect(int x, int y, int width, int height);
    void fillRect(int x, int y, int width, int height);

    void setColor(DisplayColor color);

    void clearAll();
    void render();

private:
    void updateBounds(int minX, int minY, int maxX, int maxY);
    void clearBounds();

    BinaryMatrix &data();
};


#endif