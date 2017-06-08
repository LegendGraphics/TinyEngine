#ifndef IO_FILE_UTILS_H
#define IO_FILE_UTILS_H

#include "base/String.h"

namespace te
{
    class FileUtils
    {
    public:
        static void streamFromBinaryFile(const String& filename, char*& data, int& size);
        
        static int getFileLength(std::ifstream& file);
        static int loadShader(const String& filename, char*& data, int& size);

    };
}

#endif // IO_FILE_UTILS_H