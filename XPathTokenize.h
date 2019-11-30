#pragma once
#include <vector>
#include <string>
/*!This is used to parse a string,that contains the XPath, and split it into tokens*/

enum XPath_token {
	element_name, slash, at_sign, square_brackets, name, compare_op, right_br, left_br, quot_text
};

struct XPath_type {
	std::string content;
	XPath_token type;

};


std::vector<XPath_type> XPath_tokenize(std::string input);
