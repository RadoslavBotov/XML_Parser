#include "Element.h"

Element::Element(std::string nameParam, std::string contentsParam)
{
	name = nameParam;
	contents = contentsParam;
}

std::string& Element::getName()
{
	return name;
}

std::string& Element::getContents()
{
	return contents;
}