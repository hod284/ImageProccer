#pragma once

#include "FilterBase.h"
#include "ImageBuffer.h"

#include <string>
#include <unordered_map>
#include <functional>
#include "Exceptions.h"
#include <algorithm>
#include <sstream>
#include <utility>
#include <cstring>

class ImageTransform : public FilterBase
{
private:
    /// <summary>
    // 함수중 어느것을 실행시키는지 확인하는 문자열
    /// </summary>
    std::string filterName;
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    /// <summary>
    /// 함수를 포인터로 모아놓음 딕셔너리
    /// </summary>
    std::unordered_map<std::string, std::function<void(ip::ImageBuffer&)>> handlers;
    /// <summary>
    /// 이미지 영역 자르기
    /// </summary>
    void Crop(ip::ImageBuffer& image);
    /// <summary>
    /// 이미지 크기 변경
    /// </summary>
    void Resize(ip::ImageBuffer& image);
    /// <summary>
    /// 이미지 좌우 반전
     /// </summary>
    void FlipHorizontal(ip::ImageBuffer& image);
    /// <summary>
    /// 이미지 상하 반전
     /// </summary>
    void FlipVertical(ip::ImageBuffer& image);

public:
    ImageTransform(const std::string& command);

    void Apply(ip::ImageBuffer& image) override;
};

