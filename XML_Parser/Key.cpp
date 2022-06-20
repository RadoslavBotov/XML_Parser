#include<iostream>
#include "Key.h"

Key::Key(std::string nameParam, std::string valueParam, std::string idParam)
{
	name = nameParam;
	value = valueParam;
	id = idParam;
}

std::ostream& operator << (std::ostream& os, const Key& source)
{
	os << "<" << source.name;

	if (source.id != "")
		os << " id=\"" << source.id << "\">" << source.value << "</" << source.name << ">";
	else
		os << ">" << source.value << "</" << source.name << ">";

	return os;
}
