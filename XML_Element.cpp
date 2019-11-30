#include "XML_Element.h"
#include <vector>
#include <string>
#include <iostream>
#include "XMLtokenize.h"
#include "XPathTokenize.h"


XML_Element::XML_Element()
{
	this->tag_name = "";
	this->identificator = "";
	this->text = "";

}

XML_Element XML_Element::parseDocument(std::string xmlText)
{
	XML_Element doc;
	std::vector<tag> tags = createTags(xmlText);
	doc.tag_name = tags[0].tag_name;
	doc.identificator = tags[0].id;
	if (!(tags[0].text.empty())) {
		doc.text = tags[0].text;

	}
	if (!tags[0].attributes.empty()) {
		for (unsigned i = 0; i < tags[0].attributes.size(); i++)
		{
			doc.attributes.push_back(tags[0].attributes[i]);

		}
	}
	int reached=0; ///This int keeps the  index of the last tag we went through.
	while (reached<tags.size()-1)///Because parseNode is executed till it reaches the first nested elements of the root,I manually check if there are more and parse again.
	{
		XML_Element::parse_node(tags, doc, reached + 1, reached, tags[0].tag_name);
	}


	return doc;
}

void XML_Element::parse_node(std::vector<tag> tags, XML_Element& parent, int i, int& reached_index, std::string root_name) 
{
	reached_index = i;
	if (tags[i].isOpening) {
		XML_Element* child = new XML_Element;
		child->tag_name = tags[i].tag_name;
		child->identificator = tags[i].id;
		if (!(tags[i].text.empty())) {
			child->text = tags[i].text;
		}

		for (int m = 0; m < tags[i].attributes.size(); m++) {
			child->attributes.push_back(tags[i].attributes[m]);
		}
		parent.nested_elements.push_back(child);
		i++;

		if (!(tags[i].isOpening) && tags[i].tag_name != root_name) {
			i++;
			parse_node(tags, parent, i, reached_index, root_name);

		}
		else if (tags[i].isOpening) {
			parse_node(tags, *child, i, reached_index, root_name);

		}
		else return;
	}
}

void XML_Element::copy(XML_Element const& other) {
	this->tag_name = other.tag_name;
	this->text = other.text;
	this->identificator = other.identificator;
	for (unsigned i = 0; i < other.nested_elements.size(); i++) {
		XML_Element* node = new XML_Element();
		*node = *other.nested_elements[i];
		this->nested_elements.push_back(node);
	}
	for (unsigned i = 0; i < other.attributes.size(); i++)
	{
		this->attributes.push_back(other.attributes[i]);
	}
}

XML_Element::XML_Element(const XML_Element& other) {
	this->copy(other);
}

void XML_Element::destroy()
{
	for (size_t i = 0; i < this->nested_elements.size(); i++)
	{
		delete this->nested_elements[i];

	}
}

XML_Element::~XML_Element()
{
	this->destroy();

}




XML_Element& XML_Element::operator=(XML_Element const& other) {
	if (this != &other) {
		this->destroy();
		this->copy(other);

	}
	return *this;

}

void XML_Element::searchByName(XML_Element node, std::vector<XML_Element*>& array, std::string search_name)

{
	if (node.tag_name == search_name) {
		XML_Element* copy = new XML_Element();
		*copy = node;
		array.push_back(copy);
		for (size_t i = 0; i < node.nested_elements.size(); i++)
		{
			searchByName(*node.nested_elements[i], array, search_name);
		}
		return;
	}
	else {
		for (size_t i = 0; i < node.nested_elements.size(); i++)
		{
			searchByName(*node.nested_elements[i], array, search_name);
		}
		return;
	}
	
}


void XML_Element::searchByAttributeName(std::vector<XML_Element*> subdirectory, std::vector<XML_Element*>& array, std::string search_name)
{
	for (size_t i = 0; i < subdirectory.size(); i++)
	{
		for (size_t j = 0; j < subdirectory[i]->attributes.size(); j++)
		{
			if (subdirectory[i]->attributes[j].first == search_name) {
				XML_Element* copy = new XML_Element();
				copy = subdirectory[i];
				array.push_back(copy);

			}

		}
		searchByAttributeName(subdirectory[i]->nested_elements, array, search_name);
		
	}
	return;
}


void XML_Element::searchByText(std::vector<XML_Element*> subdirectory, std::vector<XML_Element*>& newSubdirectory, std::string name_element, std::string text)
{
	for (size_t i = 0; i < subdirectory.size(); i++)
	{
		if (subdirectory[i]->tag_name == name_element && subdirectory[i]->text == text) {
			XML_Element* copy = new XML_Element();
			*copy = *subdirectory[i];
			newSubdirectory.push_back(copy);
		}
		searchByText(subdirectory[i]->nested_elements,newSubdirectory,name_element,text);
	}
	return;

}
	


XML_Element * XML_Element::searchById(std::string id, XML_Element * element) //we will return a poiter to the element
{
	if (element->identificator == id) return element;
	else {
		for (size_t i = 0; i < element->nested_elements.size(); i++)
		{
			XML_Element * element1 = searchById(id, element->nested_elements[i]);
			if (element1 != nullptr) {
				return element1;
			}
		}
		return nullptr;
	}

}


void XML_Element::printElement(XML_Element element)
{
	std::cout << "  ";
	std::cout << "<" << element.tag_name << " " << "id=" << element.identificator << " ";
	

	for (unsigned i = 0; i < element.attributes.size(); i++)
	{
		std::cout << element.attributes[i].first << " = " << element.attributes[i].second << " ";

	}

	std::cout << ">";
	std::cout << element.text;

	if (element.nested_elements.empty()) {
		std::cout << "</" << element.tag_name << " " << ">" << std::endl;
		return;
	}
	else {
		std::cout << std::endl;
		std::cout << "  ";
		for (unsigned i = 0; i < element.nested_elements.size(); i++)
		{
			std::cout << "  ";
			printElement(*element.nested_elements[i]);

		}
		std::cout << "  </" << element.tag_name << ">" << std::endl;

	}

}

void XML_Element::PrintElements()
{
	std::cout << "<" << this->tag_name << " " << "id=" << this->identificator << " ";
	if (!this->attributes.empty()) {
		for (unsigned i = 0; i < this->attributes.size(); i++)
		{
			std::cout << this->attributes[i].first << " = " << this->attributes[i].second << " ";

		}
	}
	std::cout << this->text;
	std::cout << ">" << std::endl;

	for (size_t i = 0; i < this->nested_elements.size(); i++)
	{
		printElement(*(this->nested_elements[i]));
	}
	std::cout << "</" << this->tag_name << ">";


}



std::string XML_Element::Select(std::string id, std::string key) 
{
	XML_Element* result = nullptr;
	if (this->identificator == id) { //first checking the root attributes 
		for (int i = 0; i < this->attributes.size(); i++) {
			if (this->attributes[i].first == key) return this->attributes[i].second;
		}
	}
	for (size_t i = 0; i < this->nested_elements.size() && result==nullptr; i++) { //we havent found a matching id
	
		result = this->searchById(id,this->nested_elements[i]);
	}
	if (result == nullptr) return "Error!";
	if (result->attributes.empty()) return "Error!";
	for (size_t i = 0; i < result->attributes.size(); i++)
	{
		if (result->attributes[i].first == key) return result->attributes[i].second;

	}
	return "Error!";
}

XML_Element * XML_Element::Child(std::string id, int n)
{
	XML_Element* ptr = nullptr;
	if (this->identificator == id) {
		if (n > this->nested_elements.size()) return nullptr;
		for (size_t i = 0; i < this->nested_elements.size(); i++)
		{
			if (i == n - 1) return this->nested_elements[i];

		}
	}
	for (size_t m = 0; m < this->nested_elements.size() && ptr==nullptr; m++) { //we havent found a matching id
		ptr = this->searchById(id, this->nested_elements[m]);
	}
	if (ptr == nullptr) return nullptr;
	if (n> ptr->nested_elements.size()) return nullptr;
	for (size_t i = 0; i < ptr->nested_elements.size(); i++)
	{
		if (i == n - 1) return ptr->nested_elements[i];

	}

}

std::string XML_Element::Text(std::string id) 
{
	XML_Element* ptr = nullptr;
	if (this->identificator == id) return this->text;
	for (size_t m = 0; m < this->nested_elements.size() && ptr==nullptr; m++) { //we havent found a matching id
		ptr = this->searchById(id, this->nested_elements[m]);
	}
	if (ptr->text.empty()) return "Error!";
	return ptr->text;
}

void XML_Element::Delete(std::string id, std::string key)
{
	if (this->identificator == id) {
		for (size_t i = 0; i < this->attributes.size(); i++)
		{
			if (this->attributes[i].first == key) {
				this->attributes.erase(this->attributes.begin() + i);
			}

		}
	}
	
	XML_Element* ptr = nullptr;
	for (size_t m = 0; m < this->nested_elements.size() && ptr == nullptr; m++) { //we havent found a matching id
		ptr = this->searchById(id, this->nested_elements[m]);
	}
	if (ptr == nullptr) return;
	for (size_t k = 0; k < ptr->attributes.size(); k++)
	{
		if (ptr->attributes[k].first == key) {
			ptr->attributes.erase(ptr->attributes.begin() + k);
		}

	}

}

void XML_Element::NewChild(std::string id)
{
	int n = 0;
	if (this->identificator == id) {
		n = this->nested_elements.size();
		XML_Element* newElement = new XML_Element;
		newElement->identificator += id;
		newElement->identificator += "_";
		newElement->identificator += std::to_string(n); //making sure that we set a unique identificator,so that if we decide to add a second new child it will have a unique id 
		this->nested_elements.push_back(newElement);
		return;
	}
	
	XML_Element* ptr = nullptr;
	XML_Element* newElement = new XML_Element;

	for (size_t m = 0; m < this->nested_elements.size() && ptr == nullptr; m++) { //we havent found a matching id
		ptr = this->searchById(id, this->nested_elements[m]);
	}
	if (ptr == nullptr) {
		std::cerr << "Error";
		return;
	}
	n = ptr->nested_elements.size();
	newElement->identificator += id;
	newElement->identificator += "_";
	newElement->identificator += std::to_string(n);
	newElement->tag_name = ptr->tag_name;
	ptr->nested_elements.push_back(newElement);


}

std::vector<XML_Element*> XML_Element::XPathCommandRead(std::string input)
{
	std::vector<XPath_type> tokens = XPath_tokenize(input);
	std::vector<XML_Element*> subdirectory;

	for (int i = 0; i < tokens.size(); i++)
	{
			switch (tokens[i].type) {
			case element_name: {
				searchByName(*this, subdirectory, tokens[i].content);//Entering a subdirectory of the XML document.
				break;
			}
			case at_sign: { //Returning all elements containing @attribute.
				std::vector<XML_Element*> newSubdirectory;
				searchByAttributeName(subdirectory,newSubdirectory,tokens[i].content);
				return newSubdirectory;
				break;
			}
			case slash: {
				std::vector<XML_Element*> result;
				result=XPathCommandSlash(subdirectory,result, i, tokens);
				return result;
				break;
			}
			case left_br: {
				if (tokens[i+1].type == at_sign) break;
				else if(tokens[i+1].type==element_name){
					i++;
					std::string searchNameElement = tokens[i].content;
					i += 3; //skipping the '=' and ' " '
					std::string searchText = tokens[i].content;
					std::vector<XML_Element*> newSubdirectory;
					searchByText(subdirectory,newSubdirectory, searchNameElement, searchText);
					subdirectory = newSubdirectory;


				}
				break;
				
			}

			}




	}
			
}
		

	


std::vector<XML_Element*> XML_Element::XPathCommandSlash(std::vector<XML_Element*> subdirectory,std::vector<XML_Element*>& result,int& i, std::vector<XPath_type> tokens) // JUST AN IDEA, havent tested it :(
{
	i++;
	std::string searchFor = tokens[i].content;//we will return a vector of elements with this tag name
	
	for (size_t j = 0; j < subdirectory.size(); j++)
	{
		searchByName(*subdirectory[j], result, searchFor);
	}

	if (tokens[i + 1].type == square_brackets) { //we want access to a certain element of the result vector
		int index = std::stoi(tokens[i + 1].content);
		std::vector<XML_Element*> oneElement;
		oneElement.push_back(result[index]);
		return oneElement;
	}
	return result;
	
}

void XML_Element::Set(std::string id, std::string key, std::string value)
{
	if (this->identificator == id) {
		for (unsigned i = 0; i < this->attributes.size(); i++) {
			if (this->attributes[i].first == key) {
				this->attributes[i].second.assign(value);
			}
		}
	}
	XML_Element* result = nullptr;
	for (size_t i = 0; i < this->nested_elements.size() && result == nullptr; i++) { //we havent found a matching id
		result=this->searchById(id,this->nested_elements[i]);
	}
	if (result == nullptr || result->attributes.empty()) {
		std::cerr << "Error!" << std::endl;
		return;
	}
	for (size_t i = 0; i < result->attributes.size(); i++)
	{
		if (result->attributes[i].first == key) {
			result->attributes[i].second.assign(value);
			
		}

	}

}

std::vector<std::pair<std::string, std::string>>* XML_Element::Children(std::string id)
{
	XML_Element* ptr = nullptr;
	if (this->identificator == id) { //first checking the root attributes 
		if (this->attributes.empty()) return nullptr;
		return &(this->attributes);
	}
	for (size_t i = 0; i < this->nested_elements.size() && ptr==nullptr; i++) { //we havent found a matching id
		ptr = this->searchById(id, this->nested_elements[i]);
	}
	if (ptr == nullptr) return nullptr;
	return &ptr->attributes;
}










