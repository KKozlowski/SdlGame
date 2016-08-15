#pragma once
#include <string>
#include <vector>

class file_reader_line_by_line
{
public:
	std::vector<std::string> content;
	bool is_empty() { return content.size() == 0; }
	file_reader_line_by_line(std::string filename);
};
