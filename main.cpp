#include <iostream>
#include <fstream>
#include "parser.h"

int main()
{
    std::ifstream file("sample_auth.log");

    if (!file.is_open())
    {
        std::cerr << "File not found" << std::endl;
        return 1;
    }

    std::string line;

    while (std::getline(file, line))
    {
        std::cout << line << std::endl;
    }
    Parse();
    std::cout << "Qoru your website!" << std::endl;
}