#include "file_reader_line_by_line.h"
#include <fstream>
#include <iostream>

file_reader_line_by_line::file_reader_line_by_line(std::string filename)
{
	std::ifstream read(filename);
	std::string line;

	while (std::getline(read, line))
	{
		content.push_back(line);
	}
}
