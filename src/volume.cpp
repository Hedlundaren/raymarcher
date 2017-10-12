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

void Volume::loadTestData()
{

	float TOTAL_ITERATIONS = this->getResolution().x * this->getResolution().y;
	for (int i = 0; i < this->getResolution().x; i++)
	{
		for (int j = 0; j < this->getResolution().y; j++)
		{
			for (int k = 0; k < this->getResolution().z; k++)
			{
				this->drawData(i, j, k, glm::vec4(0, 0, 0, 0));

				float v1 = rand() % 100;
				if (v1 > 0)
				{
					this->drawData(i, j, k, glm::vec4(v1 / 100, 0, 0, 0.01));
				}

				glm::vec3 pos = glm::vec3(i, j, k);
				glm::vec3 middle = glm::vec3((resolution.x - 1) / 2, (resolution.y - 1) / 2, (resolution.z - 1) / 2);
				if (length(middle - pos) < resolution.x * 0.3)
				{
					this->drawData(i, j, k, glm::vec4(0, 1, 0, 0));
				}

				if (abs(middle.x - pos.x) > middle.x * 0.9 ||
					abs(middle.y - pos.y) > middle.y * 0.9 ||
					abs(middle.z - pos.z) > middle.z * 0.9)
				{
					this->drawData(i, j, k, glm::vec4(0, 0, 1.0, 0.005));
				}
			}

			float percentage = 100.0 * (j + this->getResolution().y * i) / TOTAL_ITERATIONS;
			std::cout << (percentage) << "% \r";
		}
	}
	std::cout << "Loading complete." << std::endl;

	this->drawData(0, 0, 0, glm::vec4(0.0, 0, 1, 1));
	this->drawData(resolution.x - 1, 0, 0, glm::vec4(0.3, 0, 1, 1));
	this->drawData(0, 0, resolution.z - 1, glm::vec4(0.6, 0, 1, 1));
	this->drawData(resolution.x - 1, 0, resolution.z - 1, glm::vec4(0.9, 0, 1, 1));

	this->drawData(0, resolution.y - 1, resolution.z - 1, glm::vec4(0.0, 1, 0, 1));
	this->drawData(resolution.x - 1, resolution.y - 1, resolution.z - 1, glm::vec4(0.3, 1, 0, 1));
	this->drawData(0, resolution.x - 1, 0, glm::vec4(0.6, 1, 0, 1));
	this->drawData(resolution.x - 1, resolution.y - 1, 0, glm::vec4(0.9, 1, 0, 1));

	this->drawData(resolution.x / 2, resolution.y / 2, resolution.z / 2, glm::vec4(1, 1, 1, 1));
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

	float bytes = (float)bytesPerVoxel;
	float size = dimx * dimy * dimz * bytes;

	swapbytes(chardata, static_cast<unsigned int>(size));

	std::cout << chardata << '\n';
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
	std::cout << "chardata: " << chardata << '\n';

	// if (dim == size3_t(0))
	// {
	// 	throw DataReaderException("Error: Unable to find dimensions in .pvm file: " + filePath,
	// 							  IvwContextCustom("PVMVolumeReader"));
	// }

	// auto volume = std::make_shared<Volume>();

	// if (format == DataUInt16::get())
	// {
	// 	size_t bytes = format->getSize();
	// 	size_t size = dim.x * dim.y * dim.z * bytes;
	// 	swapbytes(data, static_cast<unsigned int>(size));
	// 	// This format does not contain information about data range
	// 	// so we need to compute it for correct results
	// 	auto minmax = std::minmax_element(reinterpret_cast<DataUInt16::type *>(data),
	// 									  reinterpret_cast<DataUInt16::type *>(data + size));
	// 	volume->dataMap_.dataRange = dvec2(*minmax.first, *minmax.second);
	// }

	// // Additional information
	// std::stringstream ss;

	// if (description)
	// {
	// 	ss << description;
	// 	volume->setMetaData<StringMetaData>("description", ss.str());
	// }
}

void Volume::loadDataSAV(std::string filename)
{
	std::ifstream file;
	file.open(filename);

	if (file.is_open())
	{
		std::string line;

		int count = 0;
		int re_count = 0;
		int ra_count = 0;
		while (getline(file, line))
		{
			std::stringstream ss;
			ss << line;

			// std::cout << line << std::endl;

			std::vector<std::string> tokens = split(line, "=");

			if (tokens.size() > 1)
			{
				std::string first = tokens[0];
				std::string second = tokens[1];

				if (first == "res")
				{
					std::string::size_type sz; // alias of size_t
					int isecond = std::stoi(second, &sz);
					originalResolution = isecond;
				}
				if (first == "re")
				{
					re_count++;

					std::string::size_type sz;
					float fsecond = std::stof(second, &sz);
					ve.push_back(glm::vec3());
					ve[ve.size() - 1].r = fsecond;
				}
				if (first == "ge")
				{
					std::string::size_type sz;
					float fsecond = std::stof(second, &sz);
					ve[ve.size() - 1].g = fsecond;
				}
				if (first == "be")
				{
					std::string::size_type sz;
					float fsecond = std::stof(second, &sz);
					ve[ve.size() - 1].b = fsecond;
				}
				if (first == "ra")
				{
					ra_count++;

					std::string::size_type sz;
					float fsecond = std::stof(second, &sz);
					va.push_back(glm::vec3());
					va[va.size() - 1].r = fsecond;
				}
				if (first == "ga")
				{
					std::string::size_type sz;
					float fsecond = std::stof(second, &sz);
					va[va.size() - 1].g = fsecond;
				}
				if (first == "ba")
				{
					std::string::size_type sz;
					float fsecond = std::stof(second, &sz);
					va[va.size() - 1].b = fsecond;
				}
			}

			count++;
			if (count > 165000)
				break;
		}
		std::cout << "line_count: " << count << "\n";
		std::cout << "line_count: " << count - 98869 << "\n";
		std::cout << "re_count: " << re_count << "\n";
		std::cout << "ra_count: " << ra_count << "\n";
		std::cout << "res: " << originalResolution << "\n";
		std::cout << "ve_size: " << ve.size() << "\n";
	}

	// Send data to texture
	unsigned i = 0;
	for (glm::vec3 data : ve)
	{
		int x = (int)floor(data.x * resolution.x);
		int y = (int)floor(data.y * resolution.y);
		int z = (int)floor(data.z * resolution.z);
		drawData(x, y, z, glm::vec4(va[i].x));
		i++;
	}
}