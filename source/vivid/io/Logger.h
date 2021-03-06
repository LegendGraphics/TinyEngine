#ifndef VIVID_IO_LOGGER_H
#define VIVID_IO_LOGGER_H

#include <ostream>
#include <fstream>

#include "vivid/base/Singleton.hpp"
#include "vivid/base/String.h"
#include "vivid/math/Vector3.h"
#include "vivid/math/Matrix.h"
#include "vivid/math/Quaternion.h"

#define cLog *(vivid::ConsoleLogger::getInstance())
#define fLog *(vivid::FileLogger::getInstance())

namespace vivid
{
    class FileLogger: public Singleton<FileLogger>
    {
    public:
        virtual FileLogger& operator<<(const String& msg);
        virtual FileLogger& operator<<(const Vector3& v);
        virtual FileLogger& operator<<(const Vector4& v);
        virtual FileLogger& operator<<(const Matrix& m);
        virtual FileLogger& operator<<(const Quaternion& q);
    private:
        std::ofstream   _fout;
    };

    class ConsoleLogger: public Singleton<ConsoleLogger>
    {
    public:
        ConsoleLogger();
    public:
        virtual ConsoleLogger& operator<<(const String& msg);
        virtual ConsoleLogger& operator<<(const Vector3& v);
        virtual ConsoleLogger& operator<<(const Vector4& v);
        virtual ConsoleLogger& operator<<(const Matrix& m);
        virtual ConsoleLogger& operator<<(const Quaternion& q);
    private:
        std::ostream   _cout;
    };
}

#endif // VIVID_IO_LOGGER_H
