#pragma once
#include<string>
#include<vector>

struct Key
{
public:
	/// Constructor with parameters.
	/// 
	/// Sets name, value and id of a key instance.
	Key(std::string nameParam, std::string valueParam, std::string idParam = "");

	/// Operator <<.
	/// 
	/// Writes name, value and id to a stream.
	friend std::ostream& operator << (std::ostream& os, const Key& source);

// variables / fields
	/// Name of key.
	std::string name;

	/// Value or text of key.
	std::string value;

	/// Unique id of key.
	std::string id;
};