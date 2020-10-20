#include "myxva.h"
#include <sys/stat.h>
#include "pugixml.hpp"
#include <vector>

namespace XVA {

struct xva_info {
	xva_info() {}
	std::string _disks_name;
	std::string _disks_size;
	std::vector<mytar::BlockPtr> _data;
};

using XvaInfo = struct xva_info;
using XvaInfoPtr = std::shared_ptr<XvaInfo>;

static std::map<std::string, XvaInfoPtr> xva_boxes;

static inline bool exists_file (const std::string& name) {
	struct stat buffer;   
	return (stat (name.c_str(), &buffer) == 0); 
}

static inline bool is_tar_file (const std::string& name) {
	std::ifstream f;
	f.open(name, std::ifstream::in | std::ifstream::binary);

	char* buffer = new char[512]; 
	f.read(buffer, 512);
	if(mytar::is_tar_head(buffer)) {
		delete [] buffer;
		return true;
	}

	delete [] buffer;
	return false;
}

// search down 
void find_node(pugi::xml_node& node, const std::string key) {
        while(!node.empty()) {
                if(std::string{node.child_value("name")} == key) {
                        break;
                }
                node = node.next_sibling();
        }
}

void dispatch(std::map<long long, mytar::BlockPtr> data) {

	mytar::BlockPtr prev_bl;
	for(auto it : data) {
		auto bl = it.second;
		auto pos = bl->filename.find("/");
		auto disk_name = bl->filename.substr(0, pos); 	
		auto check_pos = bl->filename.find("checksum");

		auto iter = xva_boxes.find(disk_name);
		if(iter == xva_boxes.end()) {
			std::cout << "occur error. disks doesn't find." << std::endl;
			continue;
		}

		if(check_pos != std::string::npos)
			continue;

		auto xva_ = iter->second;	
		if(prev_bl != nullptr) {
			auto digital_f = std::stoll(prev_bl->filename.substr(pos + 1));
			auto digital_n = std::stoll(bl->filename.substr(pos + 1));

			if(digital_f + 1 != digital_n) {
				auto diff = digital_n - digital_f - 1;
				auto null_bl = std::shared_ptr<mytar::Block>(new 
					mytar::Block(-1, 
						bl->is_longname, 
						bl->filesize * diff, 
						"nullblock"
						)
				);
				xva_->_data.emplace_back(null_bl);
			}
		}

		xva_ -> _data.push_back(bl);
		prev_bl = bl;
	}
}

std::tuple<long long, long long, std::vector<mytar::BlockPtr>> located_block(std::string disk_name, long long manual_offset) {
	auto disk = xva_boxes.find(disk_name);

	long long index = 0, offset_sum = 0;
	auto v_data = disk->second->_data;
	for(; index < v_data.size(); index++) {
		offset_sum += v_data[index]->filesize;
		if(offset_sum > manual_offset)
			return {index, offset_sum, v_data};
	}

	return {-1, -1, v_data};
}

static void parse_xml(const char* buffer, size_t size) {
	pugi::xml_document doc;
	auto result = doc.load_buffer(buffer, size);
	if(!result)
		return ;

 	pugi::xpath_node_set res = doc.select_nodes("/value/struct/member");
	auto node_range = res[1].node().child("value").child("array").child("data").children("value");
	for(auto it : node_range) {
		auto node = it.child("struct").child("member");
		if( std::string{node.child_value("value")} == "VDI" ) {
			XvaInfoPtr xvaPtr = std::shared_ptr<XvaInfo>(new XvaInfo);

			find_node(node, "id");
			xvaPtr->_disks_name = node.child_value("value");
			find_node(node, "snapshot");
			node = node.child("value").child("struct").child("member");
			find_node(node, "virtual_size");
			xvaPtr->_disks_size = node.child_value("value");

			xva_boxes.insert({xvaPtr->_disks_name , xvaPtr});
		}
	}

	return ;
}

bool XvaSt::open_xva(const std::string& filename) {
	if(!exists_file(filename))
		return false;

	if(!is_tar_file(filename))
		return false;

	tarfile = std::shared_ptr<mytar::XTar>(new mytar::XTar{filename.c_str()});

	std::map<long long, mytar::BlockPtr> out;
	tarfile->parsing([&](std::map<long long, mytar::BlockPtr> m){
		out = m;	
	}, 
	true);

	auto iter = out.find(0);
	if(iter == out.end())
		return false;

	auto bl = iter->second;
	char* buffer = new char[bl->filesize];
	auto file = tarfile->back_file();
	file->seekg(bl->offset);
	file->read(buffer, bl->filesize);
	parse_xml(buffer,  bl->filesize);

	dispatch(out);
	
	return true;
}

void XvaSt::read_xva(long long offset, size_t size, char** buffer) {
	if(tarfile == nullptr)
		return;

	auto [index, disk_offset_sum, v_data] = located_block("Ref:7", offset); //c++ 17

	auto start_offset = offset;
	for(; index > 0; index--)
		start_offset -= v_data[index]->filesize; 

	char *tmp = (char*)malloc(sizeof(char) * size);
	*buffer = tmp;
	auto file = tarfile->back_file();
	while(size) {
		auto bl = v_data[index++];
		auto block_can_read = bl->filesize - start_offset; 
		if(block_can_read > size)
			block_can_read = size;
		start_offset += bl->offset;
		file->seekg(start_offset); //start

		file->read(tmp, block_can_read);
		tmp += block_can_read;
		size -= block_can_read;
		if(size > 0)
			start_offset = 0;
		else
			break;
	}
}

}
