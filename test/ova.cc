#include "test.h"
#include "pugixml.hpp"

void the_ova(std::string ovafile) {
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("data/ova.xml");
	if(!result)
		std::cout << "load file failed. " << __func__ << std::endl;

	pugi::xpath_node_set res = doc.select_nodes("/value/struct/member[1]/value/array/data/value");
}
