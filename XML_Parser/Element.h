#pragma once
#include<string>
#include<vector>

struct Element
{
public:
	Element(std::string nameParam, std::string contentsParam);

	std::string& accessName();
	std::string& accessContents();

// variables / fields
	std::string name;
	std::string contents;
};