#pragma once
#include <vector>
#include <utility>
#include <string>
#include <cstring>
#include "XMLtokenize.h"
#include "XPathTokenize.h"
/*!This is a class representing the whole XML document.A document could be treated simply as an XML element with nested elements*/

class XML_Element
{
private:
	void printElement(XML_Element element);
	std::string tag_name;
	std::string text;
	std::string identificator;
	std::vector<XML_Element*> nested_elements;
	///Array of pairs,consisting of a key and value,representind the attributes
	std::vector<std::pair<std::string, std::string>> attributes; 
	void copy(XML_Element const& other);
	void destroy();
public:
	XML_Element();
	///This is a function that creates the whole XML document
	static XML_Element parseDocument(std::string xmlText);
	
	XML_Element(const XML_Element& other);
	
	XML_Element& operator=(XML_Element const& other);
	
	~XML_Element();
	
	void PrintElements();
	///This is a recursive helper function to the ParseDocument one
	static void parse_node(std::vector<tag> tags,XML_Element& parent,int i,int& reached_index,std::string root_name);

	///A recursive method to get an XML element by its Id
	XML_Element* searchById(std::string id, XML_Element* element); 

	///A recursive method to get an XML element by its name.It is used in the XPath
	void searchByName(XML_Element node, std::vector<XML_Element*>& array, std::string search_name);

	///A recursive helper function for the XPath
	void searchByAttributeName(std::vector<XML_Element*> subdirectory, std::vector<XML_Element*>& array, std::string search_name);

	///A recursive helper function for the XPath
	void searchByText(std::vector<XML_Element*> subdirectory, std::vector<XML_Element*>& newSubdirectory,std::string name_element,std::string text);

	///Selects an attribute by its XML elements's id
	std::string Select(std::string id, std::string key) ;
	
	///Changes the value of an attribute
	void Set(std::string id, std::string key, std::string value);
	
	///Selects the attributes of an element
	std::vector<std::pair<std::string, std::string>>* Children(std::string id);
	
	///Selects the nth child of a certain element
	XML_Element* Child(std::string id, int n);
	
	std::string Text(std::string id) ;
	
	///Deletes an element's attribute
	void Delete(std::string id, std::string key);
	
	///Creates a new child to an element that has the same name and an id
	void NewChild(std::string id);

	
	std::vector<XML_Element*> XPathCommandSlash(std::vector<XML_Element*> subdirectory, std::vector<XML_Element*>& result, int &i, std::vector<XPath_type> tokens);

	///Reads an XPath query,using tokenization and the recursive helper functions
	std::vector<XML_Element*> XPathCommandRead(std::string input);


	std::string getName() {
		return this->tag_name;
	}

	std::string getText() {
		return this->text;
	}

	std::vector<XML_Element*> getNestedElements() {
		return this->nested_elements;
	}

	std::vector<std::pair<std::string, std::string>> getAtr() {
		return this->attributes;
	}

	std::string getId() {
		return this->identificator;
	}



};

