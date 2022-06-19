#pragma once
#include<string>
#include<vector>

struct Key
{
public:
	Key(std::string nameParam, std::string valueParam);

// variables / fields
	std::string name;
	std::string value;
};