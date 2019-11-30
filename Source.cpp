#include <iostream>
#include <string>
#include "XML_Element.h"
#include "User.h"
#include <vector>
#include <fstream>
using namespace std;
int main() {
	run();
   // std::string test;
	//std::getline(std::cin, test);
	//XML_Element testt;
	//testt = testt.parseDocument(test);
	//testt.NewChild("222");
	//std::cout<<testt.Select("7","imaginery");
	//std::cout<<testt.Text("8");
	//testt.PrintElements();
	//vector<XML_Element*> ala;
	//testt.searchByName(testt, ala, "body");

	/*<people><person id=”0”><name>John Smith</name><address>USA</address></person><person id=”1”><name>Ivan Petrov</name><address>Bulgaria</address></person></people>
*/

	
	
	//<head id="1" ff="malo">aloo<m>Gabii</m><body id="0" atr="alo" gr="fle"><m></m><mind id="1" ggg="22"><mind mm="kk">gabe</mind></mind></body></head>
	/*for (size_t i = 0; i < test1.size(); i++)
	{

		std::cout << test1[i].tag_name<<test1[i].id<<std::endl;
	}
	XML_Element testt;
	testt= testt.parseDocument(test);
	testt.PrintElements();
	std::string xx;
	std::vector<XML_Element*> l;*/
	//person(address="USA")/name  person(@id)
	//testt.searchByName(testt, l, "mind");
	
	//testt.Delete("0", "gr");
	

	//testt.Set("0", "gender", "of");
	//std::cout <<"           "<< testt.Select("0", "atr")<<std::endl;
	//std::cout << "           " << testt.Select("7", "mm") << std::endl;
	//XML_Element* bb = testt.Child("1",2);

	/*testt.Set("13", "ggg", "222");
	testt.Set("7", "mm", "222");
	testt.NewChild("0");
	testt.NewChild("7"); 
	testt.NewChild("0");
	testt.Delete("0","gender");
	testt.PrintElements();
	std::cout << testt.Text("7");*/
	
	
	
	
	
	
	
	system("pause");
	return 0;
}