#include <vector>
#include <string>
#include "XMLtokenize.h"


void setIds(std::vector<tag>& tags, int id_counter) {
	id_counter++;
	for (int i = 0; i < tags.size(); i++) {
		if (tags[i].id.empty() && tags[i].isOpening) {
			tags[i].id += std::to_string(id_counter);
			id_counter++;
		}
	}
}

std::vector<token> tokenArray(std::string xmlText) {
	token op_br[2000];
	token identif[2000];
	token cl_br[2000];
	token eq_m[2000];
	token quot_text[2000];
	token cl_s[2000]; //closing sign /
	std::vector<token> token_array; //dividing the xml string into a dynamic array of tokens
	int op_br_counter = 0;
	int cl_br_counter = 0;
	int eq_m_counter = 0;
	int quot_text_counter = 0;
	int id_counter = 0;
	int cl_s_counter = 0;
	for (unsigned i = 0; i < xmlText.length(); i++) {
		if (xmlText[i] != '/' && xmlText[i] != '=' && xmlText[i] != '"' && xmlText[i] != ' ' && xmlText[i] != '>' && xmlText[i] != '<') { //that way we are sure that what follows is a string identifier,which does not include the id;
			   //identifier
			identif[id_counter].type = identifier;
			while (xmlText[i] != '/' && xmlText[i] != '='  && xmlText[i] != ' ' && xmlText[i] != '"' && xmlText[i] != '>' && xmlText[i] != '<') {
				identif[id_counter].content.push_back(xmlText[i]);
				i++;
			}
			token_array.push_back(identif[id_counter]); //filling the array with a identifier
			id_counter++;

		}
		switch (xmlText[i]) {
		case ' ': {
			identif[id_counter].type = identifier;
			i++;
			while (xmlText[i] != '/' && xmlText[i] != '='  && xmlText[i] != ' ' && xmlText[i] != '"' && xmlText[i] != '>' && xmlText[i] != '<') {
				identif[id_counter].content.push_back(xmlText[i]);
				i++;
			}
			token_array.push_back(identif[id_counter]); //filling the array with a identifier
			id_counter++;
			i--;
			break;
		}
		case '<': {
			if (xmlText[i] == '<') {
				//opening brackets
				op_br[op_br_counter].content = '<';
				op_br[op_br_counter].type = left_bracket;
				token_array.push_back(op_br[op_br_counter]); //filling the array
				op_br_counter++;
				//i++;
			}
			break;
		}
		case '>': {
			//finding a closing bracket >
			cl_br[cl_br_counter].content = '>';
			cl_br[cl_br_counter].type = right_bracket;
			token_array.push_back(cl_br[cl_br_counter]); //filling the array with a closing bracket
			cl_br_counter++;
			break;
		}
		case '=': {
			//finding a '=' mark
			eq_m[eq_m_counter].content = '=';
			eq_m[eq_m_counter].type = equals_mark;
			token_array.push_back(eq_m[eq_m_counter]); //filling the array with = ,what follows would be "text",quotation text,but not the id
			eq_m_counter++;
			break;
		}

		case '"': {
			//quoting text which might be an id or an attribute value
			quot_text[quot_text_counter].type = quotation_text;
			//quot_text[quot_text_counter].content.push_back(xmlText[i]);
			i++;
			while (xmlText[i] != '"') { //if we stumble upon a '=' then
				quot_text[quot_text_counter].content.push_back(xmlText[i]);
				i++;

			}
			//quot_text[quot_text_counter].content.push_back(xmlText[i]);

			token_array.push_back(quot_text[quot_text_counter]);
			quot_text_counter++;

			//after reading the text we are sure that what follows would be >

			break;

		}
		case '/': {
			cl_s[cl_s_counter].type = closing_s;
			cl_s[cl_s_counter].content = '/';
			token_array.push_back(cl_s[cl_s_counter]);
			cl_s_counter++;

		}

		}

	}

	return  token_array;
}

std::vector<tag> createTags(std::string xmlText) { //parsing the tokens and giving them ids
	std::vector<token> tokens = tokenArray(xmlText);
	std::vector<tag> tags;
	tag tagArray[300];
	int tag_counter = 0;
	std::vector<std::string> id_array;
	int id_counter = 1;
	//std::string current_id;
	for (unsigned i = 0; i < tokens.size(); i++) {
		if (tokens[i].type == left_bracket) { //beginning of a tag
			i++;
			switch (tokens[i].type)
			{
			case closing_s: {
				tagArray[tag_counter].isOpening = false;
				i++;
				tagArray[tag_counter].tag_name += tokens[i].content;
				i++;
				tags.push_back(tagArray[tag_counter]);
				tag_counter++;
				break;
			}
			case identifier: { //<text...>
				tagArray[tag_counter].tag_name += tokens[i].content;
				i++;
				while (tokens[i].type != right_bracket) {
					if (tokens[i].content.compare("id") == 0 && tagArray[tag_counter].id.empty()) { //checking if we already have an id we have found an id member
						i += 2;
						for (size_t m = 0; m < id_array.size(); m++)
						{
							if (tokens[i].content == id_array[m]) {
								tokens[i].content += std::to_string(id_counter);

							}
						}
						tagArray[tag_counter].id = tokens[i].content;
						id_array.push_back(tagArray[tag_counter].id);
						id_counter++;

					}
					else {
						tagArray[tag_counter].attributes.push_back(std::make_pair(tokens[i].content, tokens[i + 2].content));
						i += 2;


					}
					i++;
				}
				i--;
				break;
			}
			}
		}
		else if (tokens[i].type == right_bracket) {

			if (tokens[i + 1].type == identifier) { // <text>Gabu we have found text
				tagArray[tag_counter].text = tokens[i + 1].content;
			}
			tags.push_back(tagArray[tag_counter]);
			tag_counter++;

		}
	}
	setIds(tags, id_counter);
	return tags;
}


