#include <cstdio>
#include <fstream>
#include <string>
#include <iostream>

static void DumpFile(const char * file)
{
	std::ifstream in(file);
	std::string line;
	if (!in)
	{
		std::cout << "cannot open file " << file << "." << std::endl;
	}
	else
	{
		while (std::getline(in, line))
		{
			std::cout << line << std::endl;
		}
	}

}

int main()
{

	DumpFile("/etc/os-release");
    return 0;
}