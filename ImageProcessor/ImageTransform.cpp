#include "ImageTransform.h"

ImageTransform::ImageTransform(const std::string& command)
{
    std::stringstream stream(command);

    std::getline(stream, filterName, ':');

    if (filterName == "crop")
    {
        char separator1, separator2, separator3;

        if (!(stream >> x >> separator1
            >> y >> separator2
            >> width >> separator3
            >> height)
            || separator1 != ':'
            || separator2 != ':'
            || separator3 != ':')
        {
            throw ip::FilterError("Invalid crop parameters");
        }
    }
    else if (filterName == "resize")
    {
        char separator;

        if (!(stream >> width >> separator >> height)
            || separator != ':')
        {
            throw ip::FilterError("Invalid resize parameters");
        }
    }

    handlers.emplace("crop", [this](ip::ImageBuffer& image)
        {
            Crop(image);
        });

    handlers.emplace("resize", [this](ip::ImageBuffer& image)
        {
            Resize(image);
        });

    handlers.emplace("flipHorizontal", [this](ip::ImageBuffer& image)
        {
            FlipHorizontal(image);
        });

    handlers.emplace("flipVertical", [this](ip::ImageBuffer& image)
        {
            FlipVertical(image);
        });
}

void ImageTransform::Apply(ip::ImageBuffer& image)
{
    auto it = handlers.find(filterName);

    if (it == handlers.end())
    {
        throw ip::FilterError("Unknown filter: " + filterName);
    }

    if (image.empty())
    {
        throw ip::FilterError("Image is empty");
    }

    it->second(image);
}
void ImageTransform::Crop(ip::ImageBuffer& image)
{
    if (x < 0 || y < 0 ||
        width <= 0 || height <= 0 ||
        x > image.width() - width ||
        y > image.height() - height)
    {
        throw ip::FilterError("Invalid crop area");
    }

    ip::ImageBuffer result(width, height);

    for (int row = 0; row < height; ++row)
    {
        const auto* source = image.rowPtr(y + row) + x * 3;

        auto* destination = result.rowPtr(row);

        std::memcpy(
            destination,
            source,
            result.rowStride()
        );
    }

    image = std::move(result);
}
void ImageTransform::Resize(ip::ImageBuffer& image)
{
    if (width <= 0 || height <= 0)
    {
        throw ip::FilterError("Invalid resize dimensions");
    }

    const int originalWidth = image.width();
    const int originalHeight = image.height();

    ip::ImageBuffer result(width, height);

    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            int sourceX = static_cast<int>(
                1LL * col * originalWidth / width
                );

            int sourceY = static_cast<int>(
                1LL * row * originalHeight / height
                );

            const auto* source =
                image.rowPtr(sourceY) + sourceX * 3;

            auto* destination =
                result.rowPtr(row) + col * 3;

            std::memcpy(destination, source, 3);
        }
    }

    image = std::move(result);
}
void ImageTransform::FlipHorizontal(ip::ImageBuffer& image)
{
    const int width = image.width();
    const int height = image.height();

    for (int y = 0; y < height; ++y)
    {
        auto* row = image.rowPtr(y);

        for (int x = 0; x < width / 2; ++x)
        {
            int left = x * 3;
            int right = (width - 1 - x) * 3;

            for (int c = 0; c < 3; ++c)
            {
                std::swap(
                    row[left + c],
                    row[right + c]
                );
            }
        }
    }
}
void ImageTransform::FlipVertical(ip::ImageBuffer& image)
{
    const int height = image.height();

    for (int y = 0; y < height / 2; ++y)
    {
        auto* top = image.rowPtr(y);

        auto* bottom = image.rowPtr(height - 1 - y);

        std::swap_ranges(
            top,
            top + image.rowStride(),
            bottom
        );
    }
}