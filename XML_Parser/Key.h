#pragma once
#include<string>
#include<vector>

struct Key
{
public:
	Key(std::string nameParam, std::string valueParam, std::string idParam = "");

	friend std::ostream& operator << (std::ostream& os, const Key& source);

// variables / fields
	std::string name;
	std::string value;
	std::string id;
};