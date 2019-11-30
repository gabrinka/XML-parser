#include "XPathTokenize.h"
#include <vector>
#include <string>
std::vector<XPath_type> XPath_tokenize(std::string input) {//dividing the text
	//XPath_type XP_array[200]; // simple XPath
	std::vector<XPath_type> result;
	int counter = 0;
	for (size_t i = 0; i <= input.size(); i++) {
		if (input[i] == '/') {
			XPath_type op;
			op.type = slash;
			op.content += input[i];
			result.push_back(op);


		}
		else if (input[i] == '@') {
			XPath_type op;
			op.type = at_sign;
			i++;
			op.content += input[i]; //keeping the whole @something in one xpath element
			result.push_back(op);
		}
		else if (input[i] == '[') {
			XPath_type op;
			op.type = square_brackets;
			op.content += input[++i]; //we will keep the index in the content
			i++;//skipping the ] bracket
			result.push_back(op);

		}
		else if (input[i] == '=') {
			XPath_type op;
			op.type = compare_op;
			op.content += input[i];
			result.push_back(op);
		}
		else if (input[i] == '(') {
			XPath_type op;
			op.type = left_br;
			op.content += input[i];
			result.push_back(op);
		}
		else if (input[i] == ')') {
			XPath_type op;
			op.type = right_br;
			op.content += input[i];
			result.push_back(op);
		}
		else if (input[i] == '"') {
			XPath_type op;
			op.type = quot_text;
			i++;
			while (input[i] != '"') {
				op.content += input[i];
				i++;
			}
			result.push_back(op);


		}
		else {
			XPath_type op;
			op.type = element_name;
			while (input[i] != '/' && input[i] != '"' && input[i] != '@' && input[i] != '[' && input[i] != ']' && input[i] != '(' && input[i] != ')' && input[i] != '=' && (input[i])) {
				op.content += input[i];
				i++;
			}
			result.push_back(op);
			i--;
		}
	}

	return result;
}
