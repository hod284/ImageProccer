#pragma once

#include <iostream>
#include "ImageBuffer.h"
class FilterBase
{
protected:
    /// <summary>
    // 중심 수치
    /// </summary>
    static constexpr double CentralTendency = 128.0;
public:
    virtual ~FilterBase() = default;

    virtual void Apply(ip::ImageBuffer& image) = 0;

};

