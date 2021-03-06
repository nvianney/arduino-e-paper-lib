#include "binary_matrix.h"

BinaryMatrix::BinaryMatrix(uint16_t width, uint16_t height) : width(width), height(height) {
    // buffer = new uint8_t[width * height / 8];
    buffer = (uint8_t*) malloc(width * height / 8);
    clear();
}

BinaryMatrix::~BinaryMatrix() {
    free(buffer);
}

void BinaryMatrix::setPixel(uint16_t x, uint16_t y, bool value) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        Serial.printf("Error: location not in screen. Location is (%d,%d), screen size is (%d,%d)\n", x, y, width, height);
    };

    uint8_t &b = buffer[loc(x, y)];
    uint8_t offset = x % 8;
    uint8_t mask = ~(0x80 >> offset); // take all bits except the one containing the pixel
    b &= mask;
    b |= value << (7 - offset);
}

bool BinaryMatrix::getPixel(uint16_t x, uint16_t y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        Serial.printf("Error: location not in screen. Location is (%d,%d), screen size is (%d,%d)\n", x, y, width, height);
        return 0;
    }

    uint8_t b = buffer[loc(x, y)];
    uint8_t offset = x % 8;
    uint8_t mask = 0x80 >> offset; // take only the bit w/ the pixel value
    return (b & mask) > 0; // masked val > 0 => bit is active
}

void BinaryMatrix::setRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, bool value) {
    if (x < 0 || x+width >= this->width || y < 0 || y+height >= this->height) {
        Serial.printf("Error: rect not in bounds of screen. Rect is (%d,%d,%d,%d), screen size is (%d,%d)", x, y, width, height, this->width, this->height);
        return;
    }

    const uint8_t word = value ? 0xFF : 0x00;

    // To take into account partial first/last byte
    const int startX = (x-1) / 8 + 1; // ceil(x/8)
    const int endX = (x + width) / 8; // floor( (x+width)/8 )

    for (int ys = y; ys < y + height; ys++) { // in unit of dots

        if (startX > endX) { // Fill is only within one byte
            for (int i = x; i < x + width; i++) setPixel(i, y, value);
            continue;
        }

        // Fill partial first byte
        for (int i = x; i < startX * 8; i++) { // unit of dots
            setPixel(i, ys, value);
        }

        // Fill center
        for (int xs = startX; xs < endX; xs++) { // unit of bytes
            buffer[ys * this->width /  8 + xs] = word;
        }

        // Fill partial last byte
        for (int i = endX * 8; i < x + width; i++) { // unit of dots
            setPixel(i, ys, value);
        }
    }
}

void BinaryMatrix::clear() {
    memset(buffer, 0, width * height / 8);
}

inline int BinaryMatrix::loc(uint16_t x, uint16_t y) const {
    return (int) width * y / 8 + x / 8;;
}
