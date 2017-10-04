#include "volume.h"

Volume::Volume(const int &size)
{
    data.create(size * size, size);
    resolution = glm::vec3(size);
}

Volume::Volume(const int &x, const int &y, const int &z)
{
    data.create(x * z, y);
    resolution = glm::vec3(x, y, z);
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
    glTexSubImage2D(GL_TEXTURE_2D, 0, x + resolution.x * z, y, 1, 1, GL_RGBA, GL_FLOAT, pixels.data());
}

glm::vec4 Volume::readData(const int &x, const int &y, const int &z) const
{
    std::vector<glm::vec4> pixels(1);
    glReadPixels(x + resolution.x * z, y, 1, 1, GL_RGBA, GL_FLOAT, pixels.data());
    return pixels[0];
}

glm::vec3 Volume::getResolution() const
{
    return resolution;
}

void Volume::loadTestData(){

	float TOTAL_ITERATIONS = this->getResolution().x * this->getResolution().y;
	for(int i = 0; i < this->getResolution().x; i++){
		for(int j = 0; j < this->getResolution().y; j++){
			for(int k = 0; k < this->getResolution().z; k++){
				this->drawData(i, j, k, glm::vec4(0,0,0,0));
				
				float v1 = rand() % 100;
				if(v1 > 0){
					this->drawData(i, j, k, glm::vec4(v1/100,0,0,0.6));
				}

				
				glm::vec3 pos = glm::vec3(i,j,k);
				glm::vec3 middle = glm::vec3(50.0);
				if(length(middle - pos) < 30.0){
					this->drawData(i, j, k, glm::vec4(0,1,0,1));
				}

				if(abs(middle.x - pos.x) > middle.x * 0.9 || 
					abs(middle.y - pos.y) > middle.y * 0.9 || 
					abs(middle.z - pos.z) > middle.z * 0.9){
					this->drawData(i, j, k, glm::vec4(0,0,1,1));
				}
			}

			float percentage = 100.0 * (j + this->getResolution().y * i) / TOTAL_ITERATIONS;
			std::cout << (percentage) <<  "% \r";
		}
	}
	std::cout << "Loading complete." << std::endl;
	
	this->drawData(0, 0, 0, glm::vec4(0.0,0,1,1));
	this->drawData(99, 0, 0, glm::vec4(0.3,0,1,1));
	this->drawData(0, 0, 99, glm::vec4(0.6,0,1,1));
	this->drawData(99, 0, 99, glm::vec4(0.9,0,1,1));
	
	this->drawData(0, 99, 99, glm::vec4(0.0,1,0,1));
	this->drawData(99, 99, 99, glm::vec4(0.3,1,0,1));
	this->drawData(0, 99, 0, glm::vec4(0.6,1,0,1));
	this->drawData(99, 99, 0, glm::vec4(0.9,1,0,1));

	this->drawData(50, 50, 50, glm::vec4(1,1,1,1));
}