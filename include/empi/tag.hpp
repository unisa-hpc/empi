//
// Created by luigi on 21/09/22.
//

#ifndef EMPI_PROJECT_TAG_HPP
#define EMPI_PROJECT_TAG_HPP

namespace empi{
    struct Tag{
    public:
        explicit constexpr Tag(int value) : value(value) {}
        const int value;
    };
}

#endif //EMPI_PROJECT_TAG_HPP
