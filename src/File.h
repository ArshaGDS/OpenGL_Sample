#pragma once

#include <filesystem>

namespace fs = std::filesystem;

class File
{
public:
    static fs::path FindResource(const fs::path& relative)
    {
        fs::path dir = fs::current_path();
        while (true)
        {
            fs::path candidate = dir / relative;
            if (fs::exists(candidate))
                return candidate;
            if (!dir.has_parent_path() || dir == dir.parent_path())
                return {};
            dir = dir.parent_path();
        }
    }
};
