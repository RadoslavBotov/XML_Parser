#pragma once
#include<string>
#include<vector>

class Element
{
public:
	Element(std::string nameParam, std::string contentsParam);

	std::string& accessName();
	std::string& accessContents();

private:
	std::string name;
	std::string contents;
};