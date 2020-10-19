#include "test.h"
#include "pugixml.hpp"

void first_member(const pugi::xpath_node_set& res, const std::string key) {
	auto node = res[0].node().child("value").child("struct").child("member");
	while(!node.empty()) {
		if(std::string{node.child_value("name")} == key) {
			std::cout << "find." << std::endl;
			break;
		}
		
		//std::cout << node.child_value("name") << ":";
		//std::cout << node.child_value("value") << std::endl;
		//std::cout << "go next node." << std::endl;
		node = node.next_sibling(); 
	}

	std::cout << node.child_value("name")<< ":" << node.child_value("value") << std::endl;
}

void the_ova(std::string ovafile) {
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("ova.xml");
	if(!result)
		std::cout << "load file failed. " << __func__ << std::endl;

	pugi::xpath_node_set res = doc.select_nodes("/value/struct/member");

	//std::cout << res.size() << std::endl;
	std::cout << res.first().node().name() << std::endl;
	std::cout << res[0].node().child("name").child_value() << std::endl;
	std::cout << res[0].node().child_value("name") << std::endl;
	//std::cout << res.node().name() << std::endl;
	std::cout << "\\\\\\\\\\\\\\\\\\" << std::endl;

	std::cout << res[0].node().child("value").name() << std::endl;
	std::cout << res[0].node().child("value").child("struct").name() << std::endl;
	std::cout << res[0].node().child("value").child("struct").child("member").name() << std::endl;
	auto node = res[0].node().child("value").child("struct").child("member");
	std::cout << node.child("value").child_value() << std::endl;
	std::cout << node.next_sibling().child_value("name") << std::endl;
	std::cout << node.next_sibling().child_value("value") << std::endl;

	std::cout << "\\\\\\\\\\\\\\\\\\" << std::endl;

	std::cout << res[0].node().next_sibling().child_value("name") << std::endl;

	std::cout << "\\\\\\\\\\\\\\\\\\" << std::endl;
/*
	for(auto it : res)
		std::cout << it.attribute() << std::endl;
*/
	first_member(res, "export_vsn");
}
