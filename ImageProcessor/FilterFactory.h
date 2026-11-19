#pragma once
#include <memory>
#include <string>
#include "FilterBase.h"
#include "ImageAdjust.h"
#include "ImageTransform.h"
#include "Exceptions.h"
#include <unordered_set>

class FilterFactory
{
public:
	FilterFactory() = delete;

 	static std::unique_ptr<FilterBase> create(const std::string& command);
};

