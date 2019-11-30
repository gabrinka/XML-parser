#pragma once
#include <iostream>
#include <string>
#include "XML_Element.h"
#include <vector>
#include <fstream>
/*!This file works reads a file and writes to it,"run" is used to start the program.*/

void run();
void command(std::string input,std::ifstream& read_file, std::ofstream& edit_file, XML_Element& doc,std::string& text_to_open);
void save(std::ofstream& edit_file, XML_Element doc,std::string spaces);

