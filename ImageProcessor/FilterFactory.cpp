#include "FilterFactory.h"
std::unique_ptr<FilterBase> FilterFactory::create(
    const std::string& command)
{
    auto pos = command.find(':');
    std::string filterName = command.substr(0, pos);

    static const std::unordered_set<std::string> adjustFilters = {
        "grayscale",
        "brightness",
        "contrast",
        "threshold",
        "blur",
        "sharpen",
        "histogram"
    };

    static const std::unordered_set<std::string> transformFilters = {
        "crop",
        "resize",
        "flipHorizontal",
        "flipVertical"
    };

    if (adjustFilters.count(filterName))
    {
    
        return std::make_unique<ImageAdjust>(command);
    }

    if (transformFilters.count(filterName))
    {
        return std::make_unique<ImageTransform>(command);
    }

    throw ip::FilterError("Unknown filter: " + filterName);
}