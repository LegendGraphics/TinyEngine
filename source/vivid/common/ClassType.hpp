#ifndef VIVID_COMMON_CLASS_TYPE_HPP
#define VIVID_COMMON_CLASS_TYPE_HPP

#include <cstddef>
#include "vivid/base/Types.h"

namespace vivid
{
    template <typename TBase>
    class ClassTypeId
    {
    public:

        template <typename T>
        static int getTypeId()
        {
            static const int id = _next_type_id++;      // initialize the id at the first time, then it's been skipped.
            return id;
        } ///< do not abuse this class, it will generate tons of functions.

        static int getNextTypeId()
        {
            return _next_type_id;
        }

    private:
        static int _next_type_id;
    };

    template <typename TBase>
    int ClassTypeId<TBase>::_next_type_id = 0;

    template <class T>
    int getComponentTypeId()
    {
        return ClassTypeId<Component>::getTypeId<T>();
    }

    template <class T>
    int getResManagerTypeId()
    {
        return ClassTypeId<ResourceManager>::getTypeId<T>();
    }
}



#endif // VIVID_COMMON_CLASS_TYPE_HPP
