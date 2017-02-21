#include "io/FileUtils.h"

void streamFromBinaryFile(const std::string& filename, char*& data, int& size)
{
    // Reading size of file
    FILE * file = fopen(filename.c_str(), "rb");
    if (file == NULL) return;
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fclose(file);
    // Reading data to array of chars
    file = fopen(filename.c_str(), "rb");
    data = (char *)malloc(sizeof(char)*size);
    int ii = fread(data, sizeof(char), size, file);
    fclose(file);
}