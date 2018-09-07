#include "highlighter.h"

pcrecpp::RE Highlighter::HIGHLIGHTER("_(.*?)_");
pcrecpp::RE Highlighter::STRIPPER("[ \n]");

Highlighter::Highlighter(const std::string& text)
{
	std::string stripped = text;
	STRIPPER.GlobalReplace(std::string(""), &stripped);

	mask.resize(stripped.size());

	pcrecpp::StringPiece input(stripped);
	const char* lastMatch = input.data();
	int pos = 0;
	std::string match;
	while (HIGHLIGHTER.Consume(&input, &match))
	{
		//pos += input.data() - lastMatch;
		//for (int i = pos; i < pos + match.size(); i++) 
		//{
		//	mask[i] = true;
		//}
		//pos += match.size();
		//lastMatch = input.data() + match.size();
	}
	
	
	this->text = text;
}

bool Highlighter::isHighlighted()
{
	for (int i = 0; i < mask.size(); i++) 
	{
		if (mask[i]) 
		{
			return true;
		}
	}
	return false;
}

void Highlighter::inverted(std::vector<bool> &out)
{
	out.resize(mask.size());
	for (int i = 0; i < out.size(); i++) 
	{
		out[i] = !mask[i];
	}
}