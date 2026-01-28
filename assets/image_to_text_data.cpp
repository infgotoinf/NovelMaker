#include <iostream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: main [PATH_TO_IMAGE] [NUMBER_OF_CHANNELS]" << std::endl;
        return 1;
    }

    std::string image_name = argv[1];
    int number_of_channels = std::stoi(argv[2]);
    //std::cout << image_name << '\n';

    std::string name = "";
    for (int i = image_name.size() - 1, counter = 0; (image_name[i] != '/') || counter < 1; --i)
    {
        if (image_name[i] == '.')
        {
            name = "";
            continue;
        }
        else if (image_name[i] == '/')
        {
            ++counter;
            name = "_" + name;
            continue;
        }

        name = image_name[i] + name;
    }
    std::string h_file_name = name + ".h";
    std::string cpp_file_name = name + ".cpp";

    int x;
    int y;
    int channels;

    uint8_t* data = stbi_load(image_name.c_str(), &x, &y, &channels, number_of_channels);

    size_t data_size = x*y*number_of_channels;


    std::cout << image_name << '\t' << h_file_name << '\n';

    std::ofstream file(h_file_name);

    file << "const unsigned long long " << name << "_size = " << data_size << ';' << std::endl;
    file << "const unsigned char " << name << "[" << data_size << "] = { ";
    for (size_t i = 0, end = data_size - 1; i <= end; ++i)
    {
        file << static_cast<int>(data[i]);
        if (i != end)
        {
            file << ",";
        }
        if (i % 128 == 0)
        {
            file << std::endl;
        }
    }
    file << "};" << std::endl;

    file.close();

    stbi_image_free(data);

    return 0;
}
