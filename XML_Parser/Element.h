#pragma once
#include<string>
#include<vector>

struct Element
{
public:
	Element(std::string nameParam, std::string contentsParam);

// variables / fields
	std::string name;
	std::string contents;
};