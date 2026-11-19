#include "Logger.h"


void Logger::WriteInfo(const std::string& message)
{
    logs.emplace_back("[INFO] " + message);
}

void Logger::WriteError(const std::string& message)
{
    logs.emplace_back("[ERROR] " + message);
}

void Logger::Save(const std::string& filePath) const
{
    std::ofstream file(filePath, std::ios::app);

    if (!file.is_open())
    {
        std::cerr << "Failed to open log file: "
            << filePath << '\n';

        return;
    }

    for (const auto& log : logs)
    {
        file << log << '\n';
    }

    if (!file)
    {
        std::cerr << "Failed to write log file\n";
    }
}