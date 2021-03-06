#include "image.h"


Pixel::Pixel(int red, int green, int blue, int bpp) :
    r(red), g(green), b(blue), bytes_per_pixel(bpp) { }

Image Image::asImage(const void *imageStruct) {

    Image img (
        derefType<unsigned int>(imageStruct, 0),
        derefType<unsigned int>(imageStruct, 1),
        derefType<unsigned int>(imageStruct, 2),
        ((unsigned char*) imageStruct) + sizeof(unsigned int) * 3
    );

    return img;
}

Image::Image(unsigned int w, unsigned int h, unsigned int bpp, unsigned char *data) : 
    width(w), height(h), bytes_per_pixel(bpp), pixel_data(data) { }

// The scaling is done by mapping the pixel coordinates (x, y) -> (original x, original y), where (x, y)
// are coordinates from (0, 0) to (original width * scale, original height * scale).
// It is up to the method that retrieves the pixel to determine this.
void Image::setScale(float scale) {
    this->scale = scale;
}

float Image::getScale() const {
    return scale;
}

Pixel Image::pixelAt(int x, int y) const {
    int offset;
    if (scale == 1.0f) {
        offset = (width * y * bytes_per_pixel) + (x * bytes_per_pixel);
    } else {
        int imgX = (float) x / scale;
        int imgY = (float) y / scale;
        offset = (width * imgY * bytes_per_pixel) + (imgX * bytes_per_pixel);
    }

    if (bytes_per_pixel == 2) {
        uint16_t color = derefByte<uint16_t>(pixel_data, offset);
        Pixel pixel(
            (color >> 11) & 0x1F,
            (color >> 5) & 0x3F,
            (color >> 0) & 0x1F,
            bytes_per_pixel
        );

        return pixel;
    }

    return Pixel(-1, -1, -1, -1);
}

