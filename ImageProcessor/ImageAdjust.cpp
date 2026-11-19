#include "ImageAdjust.h"

ImageAdjust::ImageAdjust(const std::string& command)
{

    auto pos = command.find(':');

    filterName = command.substr(0, pos);

    if (pos != std::string::npos)
    {
        value = std::stod(command.substr(pos + 1));
    }

    handlers.emplace("grayscale", [this](ip::ImageBuffer& image)
     {
            GrayscaleConversion(image);
    });

    handlers.emplace("brightness", [this](ip::ImageBuffer& image)
        {
            BrightnessAdjustment(image, static_cast<int>(this->value));
        });

    handlers.emplace("contrast", [this](ip::ImageBuffer& image)
        {
            ContrastAdjustment(image, this->value);
        });

    handlers.emplace("threshold", [this](ip::ImageBuffer& image)
        {
            Thresholding(image, static_cast<int>(this->value));
        });

    handlers.emplace("blur", [this](ip::ImageBuffer& image)
        {
            Blur(image);
        });

    handlers.emplace("sharpen", [this](ip::ImageBuffer& image)
        {
            Sharpening(image);
        });

    handlers.emplace("histogram", [this](ip::ImageBuffer& image)
        {
            Historygram(image);
        });
}

void ImageAdjust::GrayscaleConversion(ip::ImageBuffer& image)
{
    auto data = image.data();
    for (std::size_t i = 0; i < image.dataSize(); i += 3)
    {
        auto gray = static_cast<std::uint8_t>(
            //ITU-R BT.601 표준에서 사용하는 휘도(Luma) 계산 가중치 blue
            data[i] * 0.114 +
            //ITU-R BT.601 표준에서 사용하는 휘도(Luma) 계산 가중치 green
            data[i + 1] * 0.587 +
            //ITU-R BT.601 표준에서 사용하는 휘도(Luma) 계산 가중치red
            data[i + 2] * 0.299
            );

        data[i] = gray;
        data[i+1] = gray;
        data[i+2] = gray;
    }
    GrayscaleConversionOn = true;
}

void ImageAdjust::Blur(ip::ImageBuffer& image)
{
    // 주변 9개 픽셀 평균값
    constexpr double weight = 1.0 / 9.0;

    const Kernel kernel = { {
        { weight, weight, weight },
        { weight, weight, weight },
        { weight, weight, weight }
    } };
    Convolution(image, kernel);

}

void ImageAdjust::Sharpening(ip::ImageBuffer& image)
{
    const Kernel kernel = { {
       { 0, -1,  0 },
       { -1,  5, -1 },
       { 0, -1,  0 }
   } };
   Convolution(image, kernel);
}

void ImageAdjust::Thresholding(ip::ImageBuffer& image, int value)
{
    if (!GrayscaleConversionOn)
    {
        GrayscaleConversion(image);
    }

    auto data = image.data();

    for (std::size_t i = 0; i < image.dataSize(); ++i)
    {
        data[i] = data[i] >= value ? 255 : 0;
    }
}


void ImageAdjust::BrightnessAdjustment(ip::ImageBuffer& image, int value)
{
    auto data = image.data();

    for (std::size_t i = 0; i < image.dataSize(); ++i)
    {
        int pixel = static_cast<int>(data[i]) + value;

        data[i] = static_cast<std::uint8_t>(
            std::clamp(pixel, 0, 255)
            );
    }
}

void ImageAdjust::ContrastAdjustment(ip::ImageBuffer& image, double factor)
{
    auto data = image.data();

    for (std::size_t i = 0; i < image.dataSize(); ++i)
    {
        double pixel = (data[i] - CentralTendency) * factor + CentralTendency;

        data[i] = static_cast<std::uint8_t>(
            std::clamp(pixel, 0.0, 255.0)
            );
    }
}

void ImageAdjust::Historygram(const ip::ImageBuffer& image)
{
    std::array<int, 256> blue{};
    std::array<int, 256> green{};
    std::array<int, 256> red{};

    auto data = image.data();

    for (std::size_t i = 0; i < image.dataSize(); i += 3)
    {
        ++blue[data[i]];
        ++green[data[i + 1]];
        ++red[data[i + 2]];
    }

    std::cout << "Value\tBlue\tGreen\tRed\n";

    for (int i = 0; i < 256; ++i)
    {
        std::cout << i << '\t'
            << blue[i] << '\t'
            << green[i] << '\t'
            << red[i] << '\n';
    }
}

void ImageAdjust::Apply(ip::ImageBuffer& image)
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

void ImageAdjust::Convolution(ip::ImageBuffer& image, const Kernel& kernel)
{
        if (image.empty())
            throw ip::FilterError("Image is empty");

        const int width = image.width();
        const int height = image.height();

        ip::ImageBuffer result(width, height);

        const std::uint8_t* source = image.data();
        auto destination = result.data();

        
        auto processRows = [&](int startRow, int endRow)
            {
                // 이미지 픽셀 순회
                for (int y = startRow; y < endRow; ++y)
                {
                    for (int x = 0; x < width; ++x)
                    {
                        for (int c = 0; c < 3; ++c)
                        {
                            double sum = 0.0;
                            // 주변 픽셀 탐색
                            for (int ky = -1; ky <= 1; ++ky)
                            {
                                for (int kx = -1; kx <= 1; ++kx)
                                {
                                    // 이미지 경계 처리
                                    int px = std::clamp(
                                        x + kx, 0, width - 1);

                                    int py = std::clamp(
                                        y + ky, 0, height - 1);
                                    // 실제 합성곱 계산
                                    auto index = (py * width + px) * 3 + c;

                                    sum += source[index] *
                                        kernel[ky + 1][kx + 1];
                                }
                            }

                            auto index = (y * width + x) * 3 + c;
                            // 결과값저장
                            destination[index] =
                                static_cast<std::uint8_t>(
                                    std::clamp(std::round(sum), 0.0, 255.0)
                                    );
                        }
                    }
                }
            };

        // 이미지 크기에 따라 병렬 처리 여부 결정
        constexpr std::size_t PARALLEL_THRESHOLD = 1'000'000;

        const auto pixelCount =
            static_cast<std::size_t>(width) * height;

        if (pixelCount < PARALLEL_THRESHOLD)
        {
            processRows(0, height);
        }
        else
        {
            unsigned int threadCount =
                std::thread::hardware_concurrency();

            threadCount = std::max(1u, threadCount);

            threadCount = std::min(
                threadCount, static_cast<unsigned int>(height));

            std::vector<std::thread> threads;
            threads.reserve(threadCount);

            try
            {
                for (unsigned int i = 0; i < threadCount; ++i)
                {
                    int start = static_cast<int>(
                        1LL * height * i / threadCount);

                    int end = static_cast<int>(
                        1LL * height * (i + 1) / threadCount);

                    threads.emplace_back(processRows, start, end);
                }
            }
            catch (...)
            {
                for (auto& thread : threads)
                    thread.join();

                throw;
            }

            for (auto& thread : threads)
            {
                thread.join();
            }
        }

        // 처리 완료된 버퍼로 교체
        image = std::move(result);
    
}