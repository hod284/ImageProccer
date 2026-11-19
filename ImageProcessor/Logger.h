#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Logger
{
private:
    std::vector<std::string> logs;
public:
   void WriteInfo(const std::string& message);
   void WriteError(const std::string& message);
   void Save(const std::string& filePath) const;
};

