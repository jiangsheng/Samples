// OrganizeFilesByDate.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>

using namespace std;
using namespace filesystem;

int wmain(int argc, wchar_t* argv[])
{
    std::wstring inputFolder;
    std::wstring outputFolder;
    std::wstring exePath = argv[0];
    if (argc == 1)
    {
        inputFolder= outputFolder= std::filesystem::current_path();
    }
    else if (argc == 2)
    {
        inputFolder = outputFolder = argv[1];
    }
    else if (argc == 3)
    {
        inputFolder = argv[1];
        outputFolder = argv[2];
    }
    else
        std::cout << "Too many parameters! expected [Source] [Target]!\n";

    for (auto& p : directory_iterator(inputFolder))
    {
        auto filePath=p.path();
        if (filePath.wstring() == exePath) continue;
        auto fileTime = p.last_write_time();
        auto formatted_time = std::format(L"{:%Y-%m-%d}", fileTime);
        path targetPath = outputFolder;
        targetPath /= formatted_time;
        create_directories(targetPath);
        targetPath /= filePath.filename();
        rename(filePath, targetPath);
    }
}
