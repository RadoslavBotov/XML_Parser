#include "Element.h"

Element::Element(std::string nameParam, std::string contentsParam)
{
	name = nameParam;
	contents = contentsParam;
}

std::string& Element::accessName()
{
	return name;
}

std::string& Element::accessContents()
{
	return contents;
}