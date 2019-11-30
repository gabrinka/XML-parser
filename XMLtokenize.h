#pragma once
#include <vector>
#include <string>
/*!These functions take a string and splits it into small tokens,used later on to create tags and then the final XML doc. */
enum token_type { left_bracket, right_bracket, closing_s, quotation_text, identifier, equals_mark };

struct token {
	std::string content;
	token_type type;


};

struct tag {
	std::string text; /// <tag>Gabi</tag> gabi is text
	std::string id;
	std::string tag_name;
	std::vector<std::pair<std::string, std::string>> attributes;
	bool isOpening; ///check whether the tag is closing 
};

///A function to set unique IDs.
void setIds(std::vector<tag>& tags, int id_counter); 

///A function to create tokens.
std::vector<token> tokenArray(std::string xmlText);

///A function to create tags from tokens ,ex: <head>Hello is an opening tag,</head> is a closing one.
std::vector<tag> createTags(std::string xmlText);


