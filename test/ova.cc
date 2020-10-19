#include "test.h"
#include "pugixml.hpp"

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

}