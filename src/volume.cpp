#include "volume.h"

Volume::Volume(const int &size)
{
	resolution = glm::ivec3(size);
	int X = size * (int)ceil(sqrt((float)size));
	int Y = size * (int)ceil(sqrt((float)size));
	data.create(X, Y);
}

Volume::Volume(const int &x, const int &y, const int &z)
{
	resolution = glm::ivec3(x, y, z);
	int X = resolution.x * (int)ceil(sqrt((float)resolution.z));
	int Y = resolution.y * (int)ceil(sqrt((float)resolution.z));
	data.create(X, Y);
}

void Volume::bindTexture()
{
	data.bindTexture();
}

void Volume::bindBuffer()
{
	data.bindBuffer();
}

void Volume::drawData(const int &x, const int &y, const int &z, const float &v)
{
	std::vector<float> pixels(1);
	pixels[0] = v;

	// std::cout << "resz sqrt: " << (int) sqrt(resolution.z) << "\n";
	int X = x + (z % (int)ceil(sqrt((float)resolution.z))) * resolution.x;
	int Y = y + ((int)(z / ceil(sqrt((float)resolution.z)))) * resolution.y;
	glTexSubImage2D(GL_TEXTURE_2D, 0, X, Y, 1, 1, GL_RED, GL_FLOAT, pixels.data());
}

void Volume::drawData(const int &z, const std::vector<float> &pixels, const int &dimx, const int &dimy)
{
	int X = (z % (int)ceil(sqrt((float)resolution.z))) * resolution.x;
	int Y = ((int)(z / ceil(sqrt((float)resolution.z)))) * resolution.y;
	glTexSubImage2D(GL_TEXTURE_2D, 0, X, Y, resolution.x, resolution.y, GL_RED, GL_FLOAT, pixels.data());
}

glm::vec4 Volume::readData(const int &x, const int &y, const int &z) const
{
	std::vector<glm::vec4> pixels(1);
	glReadPixels(x + resolution.x * z, y, 1, 1, GL_RED, GL_FLOAT, pixels.data());
	return pixels[0];
}

glm::vec3 Volume::getResolution() const
{
	return resolution;
}

void Volume::loadTestData()
{

	float TOTAL_ITERATIONS = this->getResolution().x * this->getResolution().y;

	unsigned x = 0;
	unsigned y = 0;
	unsigned z = 0;
	for (x = 0; x < this->getResolution().x; x++)
	{
		std::vector<glm::vec4> pixels(resolution.x * resolution.y * resolution.z);
		for (y = 0; y < this->getResolution().y; y++)
		{
			for (z = 0; z < this->getResolution().z; z++)
			{
				this->drawData(x, y, z, 0.0);

				float v1 = rand() % 100;
				if (v1 > 0)
				{
					this->drawData(x, y, z, 0.01);
				}

				glm::vec3 pos = glm::vec3(x, y, z);
				glm::vec3 middle = glm::vec3((resolution.x - 1) / 2, (resolution.y - 1) / 2, (resolution.z - 1) / 2);
				if (length(middle - pos) < resolution.x * 0.3)
				{
					this->drawData(x, y, z, 0.1);
				}

				if (abs(middle.x - pos.x) > middle.x * 0.9 ||
					abs(middle.y - pos.y) > middle.y * 0.9 ||
					abs(middle.z - pos.z) > middle.z * 0.9)
				{
					this->drawData(x, y, z, 0.005);
				}
			}

			float percentage = 100.0 * (y + this->getResolution().y * x) / TOTAL_ITERATIONS;
			std::cout << (percentage) << "% \r";
		}
	}
	std::cout << "Loading complete." << std::endl;

	this->drawData(0, 0, 0, 1.0);
	this->drawData(resolution.x - 1, 0, 0, 1.0);
	this->drawData(0, 0, resolution.z - 1, 1.0);
	this->drawData(resolution.x - 1, 0, resolution.z - 1, 1.0);

	this->drawData(0, resolution.y - 1, resolution.z - 1, 1.0);
	this->drawData(resolution.x - 1, resolution.y - 1, resolution.z - 1, 1.0);
	this->drawData(0, resolution.x - 1, 0, 1.0);
	this->drawData(resolution.x - 1, resolution.y - 1, 0, 1.0);

	this->drawData(resolution.x / 2, resolution.y / 2, resolution.z / 2, 1.0);
}

std::vector<std::string> Volume::split(std::string input, const std::string &delimiter)
{

	std::vector<std::string> tokens;
	std::istringstream ss(input);
	std::string token;

	while (std::getline(ss, token, '='))
	{
		tokens.push_back(token);
	}

	return tokens;
}

void Volume::InitTextures3D()
{

	glm::mat3 basis(2.0f);
	glm::vec3 spacing(0.0f);

	// Reading MPVM volume
	unsigned char *chardata = nullptr;
	unsigned int bytesPerVoxel = 0;
	unsigned char *description = nullptr;
	unsigned char *courtesy = nullptr;
	unsigned char *parameter = nullptr;
	unsigned char *comment = nullptr;

	glm::ivec3 udim{0};

	unsigned int dimx = 0;
	unsigned int dimy = 0;
	unsigned int dimz = 0;

	// Holds the luminance buffer
	chardata = readPVMvolume("data/Bruce.pvm", &dimx, &dimy, &dimz, &bytesPerVoxel, &spacing.x,
							 &spacing.y, &spacing.z, &description, &courtesy, &parameter, &comment);

	float *floatBuffer = new float[dimy * dimz * dimx];

	unsigned short *ptr = (unsigned short *)chardata;
	for (unsigned nIndx = 0; nIndx < dimx * dimy * dimz; nIndx++)
	{
		unsigned short voxelData = *ptr;
		float value = (float)voxelData / (8.0 * 65000.0f);
		floatBuffer[nIndx] = value;
		ptr++;
	}

	GLuint mu3DTex;
	glGenTextures(1, &mu3DTex);

	glEnable(GL_TEXTURE_3D);
	glBindTexture(GL_TEXTURE_3D, mu3DTex);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage3D(GL_TEXTURE_3D,
				 0,
				 GL_RED,
				 dimy,
				 dimz,
				 dimx,
				 0,
				 GL_RED,
				 GL_FLOAT,
				 (GLvoid *)floatBuffer);

	glBindTexture(GL_TEXTURE_3D, mu3DTex);

	delete[] floatBuffer;
}

// =========== Load different sorts of data =================

void Volume::loadDataPVM(std::string filePath)
{

	glm::mat3 basis(2.0f);
	glm::vec3 spacing(0.0f);

	// Reading MPVM volume
	unsigned char *chardata = nullptr;
	unsigned int bytesPerVoxel = 0;
	unsigned char *description = nullptr;
	unsigned char *courtesy = nullptr;
	unsigned char *parameter = nullptr;
	unsigned char *comment = nullptr;

	glm::ivec3 udim{0};

	unsigned int dimx = 0;
	unsigned int dimy = 0;
	unsigned int dimz = 0;

	chardata = readPVMvolume(filePath.c_str(), &dimx, &dimy, &dimz, &bytesPerVoxel, &spacing.x,
							 &spacing.y, &spacing.z, &description, &courtesy, &parameter, &comment);

	if (chardata == nullptr)
	{
		std::cout << "Could not read data.\n";
	}

	// std::cout << chardata << '\n';
	std::cout << "dimx: " << dimx << '\n';
	std::cout << "dimy: " << dimy << '\n';
	std::cout << "dimz: " << dimz << '\n';
	std::cout << "bytesPerVoxelbytes: " << bytesPerVoxel << '\n';
	std::cout << "spacing.x: " << spacing.x << '\n';
	std::cout << "spacing.y: " << spacing.y << '\n';
	std::cout << "spacing.z: " << spacing.z << '\n';
	std::cout << "description: " << description << '\n';
	std::cout << "courtesy: " << courtesy << '\n';
	std::cout << "parameter: " << parameter << '\n';
	std::cout << "comment: " << comment << '\n';

	unsigned x = 0;
	unsigned y = 0;
	unsigned z = 0;
	float maxDataValue = 0.0f;

	switch (bytesPerVoxel)
	{
	case 1:
		maxDataValue = 255.0f;
		break;
	case 2:
		maxDataValue = 65535.0f;
		break;
	default:
		maxDataValue = 255.0f;
		break;
	}

	auto *ptr = (unsigned char *)chardata;

	for (z = 0; z < dimz; z++)
	{

		std::vector<float> pixels(resolution.x * resolution.y * resolution.z);

		for (y = 0; y < dimy; y++)
		{
			for (x = 0; x < dimx; x++)
			{
				auto voxelData = *ptr;
				float value = (float)voxelData / maxDataValue;
				pixels[x + y * resolution.x] = value;
				ptr++;
			}

			float percentage = 100.0 * z / (float)dimz;
			std::cout << (percentage) << "% \r";
		}

		const int k = (int)((z / (float)dimz) * resolution.z);
		this->drawData(k, pixels, dimx, dimy);
	}
	std::cout << "Loading complete." << std::endl;
}
