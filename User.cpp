#include "User.h"
#include <algorithm>
#include <iostream>
#include <string>
#include "XML_Element.h"
#include <vector>
#include <fstream>
using namespace std;
void run() {

	string user_input;
	cout << "Hello,Welcome to XML Parser!" << endl;
	cout << endl;
	cout << "Possible commands are:" << endl;
	cout << "1. Open..../opens a file/;" << endl;
	cout << "2. Close..../closes the current file without saving it/;" << endl;
	cout << "3. Save..../saves the current file/;" << endl;
	cout << "4. SaveAs..../saves the file in a new path direction,chosen by the user/;" << endl;
	cout << endl;
	cout << endl;
	cout << "Please,type a command here: ";
	ifstream read_file;
	ofstream write_file;
	string file_to_open="";
	XML_Element doc;
	do {
		cin >> user_input;
		command(user_input, read_file, write_file, doc, file_to_open);
	} while (user_input != "Exit");




}


void command(string input, std::ifstream& read_file, ofstream& edit_file,XML_Element& doc,string& text_to_open) {
	string input1,input2,input3;
	int n;

	 if (input == "Open") {
	
		cout << "Please type the name of the file you want to open ,/example - Text.xml/:";
		cin>>text_to_open;

		

		read_file.open(text_to_open, ifstream::in);
		if (read_file.fail()) {
			cerr << "Failed attempt to open file,creating new with the same name you typed :";
			edit_file.open(text_to_open);
			if (edit_file.is_open()) {
				cout << "Successfully created a new file"<< endl;
				
			}
			edit_file.close();
		}
		else {
			string reading_from_file;
			while (!read_file.eof()) {
				cout << "Successfully opened file!" << endl;
				while (getline(read_file, reading_from_file, '\0')) {
					cout << reading_from_file << endl;
					
				}
			}
			read_file.close();
			//now removin the '\n'
			reading_from_file.erase(remove(reading_from_file.begin(), reading_from_file.end(), '\n'), reading_from_file.end());
			
			doc=doc.parseDocument(reading_from_file);
			cout << "Please type a command.The commands are:Print,Select,Set,Children,Child,Text,Delete,NewChild,XPath,Open,Close,Save,SaveAs" << endl;
			//changes

		}


	}
	 else if (text_to_open=="") {
		 cout << "No opened file!"<<endl;
		 return;
	 }
	 else if (input == "Save") {
	 string spaces = "";
	 edit_file.open(text_to_open, std::ofstream::out | std::ofstream::trunc);
	 save(edit_file, doc, spaces);
	 edit_file.close();

	}
	else if (input == "SaveAs") {
		 cout << "Please type the name of the new file:";
		 string new_file;
		 cin >> new_file;
		 string spaces = "";
		 edit_file.open(new_file, std::ofstream::out | std::ofstream::trunc);
		 save(edit_file, doc, spaces);
		 
	}
	else if (input == "Exit") {
		return;
	}
	else if (input == "Close") {
		text_to_open = "";
		read_file.close();
		edit_file.close();
	}
	else if (input == "Print") {
		read_file.open(text_to_open, ifstream::in);
		string reading_from_file;
		while (!read_file.eof()) {
			
			while (getline(read_file, reading_from_file, '\0')) {
				cout << reading_from_file << endl;
				
			}
		}
		cout << "Successfully PRINTED file!" << endl;
		read_file.close();

	}
	else if (input == "Select") {
		cout << "Please type id and key:";
		cin >> input1 >> input2;
		cout << doc.Select(input1, input2)<<endl;

	}
	else if (input == "Set") {
		 cout << "Please type id , key, value:";
		 cin >> input1 >>input2>>input3;
		 doc.Set(input1, input2, input3);
		 cout << endl;

	}
	else if (input == "Text") {
		 cout << "Please type id :";
		 cin >> input1;
		 cout << doc.Text(input1)<<endl;

	}
	else if (input == "Child") {
		 cout << "Please type id and n:";
		 cin >> input1 >> n;
		 if (doc.Child(input1, n) == nullptr) {
			 cerr << "Error we!" << endl;
		 }
		 else {
			 XML_Element child = *(doc.Child(input1, n));

			 child.PrintElements();
		 }
	}
	else if (input == "Children") {
	cout << "Please type id:";
	cin >> input1;
	if (doc.Children(input1) == nullptr) {
		cerr << "Error!";
		return;
	}
	vector<pair<string, string>> attributes = *(doc.Children(input1));
	if (attributes.empty()) {
		cout << "No attributes found!" << endl;
		return;
	}
	for (size_t i = 0; i < attributes.size(); i++)
	{
		cout << attributes[i].first << "=" << attributes[i].second << endl;
	}

	}
	else if (input == "XPath") {
	cout << "Please type an XPath query:";
	cin >> input1;
	vector<XML_Element*> xpath=doc.XPathCommandRead(input1);
	cout << xpath[0]->getName();
	}
	else if (input == "NewChild") {
	cout << "Please type id:";
	cin >> input1;
	doc.NewChild(input1);
	}
	else if (input == "Delete") {
	cout << "Please type id and key:";
	cin >> input1>>input2;
	doc.Delete(input1, input2);
	}
}


void save(std::ofstream& edit_file, XML_Element doc,string spaces)
{
	spaces += " ";
	edit_file << "<" <<doc.getName() << " " << "id=" <<'"'<< doc.getId()<<'"';
	if (!doc.getAtr().empty()) {
		for (unsigned i = 0; i < doc.getAtr().size(); i++)
		{
			edit_file<< " "<<doc.getAtr()[i].first << "=" <<'"'<< doc.getAtr()[i].second<<'"';

		}
	}
	
	edit_file << ">" ;
	edit_file << doc.getText();
	if (doc.getNestedElements().empty()) {
		if (doc.getText().empty()) {
			edit_file  << "</" << doc.getName() << ">" << endl;
		} else if(!doc.getText().empty()){
		edit_file<< "</" << doc.getName() << ">" << endl;
		}
		return;
	}
	else {
		if(doc.getText().empty()) edit_file << endl;
		
	
		for (size_t i = 0; i < doc.getNestedElements().size(); i++)
		{
			edit_file << spaces;
			save(edit_file, *(doc.getNestedElements()[i]),spaces);
		}
		spaces.erase(spaces.begin());
		edit_file<<spaces<< "</" << doc.getName() << ">"<<endl;
	}

	
}
