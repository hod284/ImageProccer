#pragma once

#include <iostream>
#include <algorithm>
#include <array>
#include <functional>
#include <unordered_map>
#include <thread>
#include <vector>
#include <utility>
#include <cmath>
#include "FilterBase.h"
#include "Exceptions.h"

class ImageAdjust : public FilterBase 
{
public:
	
	ImageAdjust(const std::string& command);

	void Apply(ip::ImageBuffer& image) override;

private:

	/// <summary>
    // 흑백 변환
    /// </summary>
	void GrayscaleConversion(ip::ImageBuffer& image);
	/// <summary>
	// 흐림 처리
	/// </summary>
	void Blur(ip::ImageBuffer& image);
	/// <summary>
	//선명도 개선
	/// </summary>
	void Sharpening(ip::ImageBuffer& image);
	/// <summary>
	//이진화 단독 실행
	/// </summary>
	void Thresholding(ip::ImageBuffer& image, int value);
	/// <summary>
	//밝기조절
	/// </summary>
	void BrightnessAdjustment(ip::ImageBuffer& image, int value);
	/// <summary>
	// 대비조절
	/// </summary>
	void ContrastAdjustment(ip::ImageBuffer& image, double factor);
	/// <summary>
	///  픽셀값을 보여주기 
	/// </summary>
	void Historygram(const ip::ImageBuffer& image);
	/// <summary>
	// 함수중 어느것을 실행시키는지 확인하는 문자열
	/// </summary>
	std::string filterName;
	/// <summary>
     // 파라미터에 들어가는 변수
	/// </summary>
	double value = 0.0;
	/// <summary>
	// 흑백변환 처리 여부
	/// </summary>
	bool GrayscaleConversionOn = false;
	using Kernel = std::array<std::array<double, 3>, 3>;
	/// <summary>
	// Convolution은 현재 픽셀과 주변 픽셀의 값을 합께 계산해서 새로운 픽셀값을 만드는 알고리즘
	/// </summary>
	void Convolution(ip::ImageBuffer& image, const Kernel& kernel);
	/// <summary>
	/// 함수를 포인터로 모아놓음 딕셔너리
	/// </summary>
	std::unordered_map<std::string, std::function<void(ip::ImageBuffer&)>> handlers;
};

