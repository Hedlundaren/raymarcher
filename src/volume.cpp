#include "volume.h"

Volume::Volume(const int &size)
{
    data.create(size * size, size);
}

Volume::Volume(const int &x, const int &y, const int &z)
{
    data.create(x * z, y);
}

void Volume::bindTexture()
{
    data.bindTexture();
}

void Volume::bindBuffer()
{
    data.bindBuffer();
}

void Volume::drawData(const int &x, const int &y, const int &z, const glm::vec4 &v)
{
    std::vector<glm::vec4> pixels(1);
    pixels[0] = v;
    glTexSubImage2D(GL_TEXTURE_2D, 0, x * z, y, 1, 1, GL_RGBA, GL_FLOAT, pixels.data());
}

glm::vec4 Volume::readData(const int &x, const int &y, const int &z) const
{
    std::vector<glm::vec4> pixels(1);
    glReadPixels(x*z, y, 1, 1, GL_RGBA, GL_FLOAT, pixels.data());
    return pixels[0];
}