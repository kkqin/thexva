#include "myxva.h"
#include <sys/stat.h>
#include "pugixml.hpp"

namespace XVA {

struct xva_info {

};

using XvaInfo = struct xva_info;
using XvaInfoPtr = std::shared_ptr<XvaInfo>;

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

static XvaInfoPtr parse_xml(const char* buffer, size_t size) {
	pugi::xml_document doc;
	auto result = doc.load_buffer(buffer, size);
	if(!result)
		return nullptr;

	return nullptr;
}

bool XvaSt::open_xva(const std::string& filename) {
	if(!exists_file(filename))
		return false;

	if(!is_tar_file(filename))
		return false;

	tarfile = std::shared_ptr<mytar::XTar>(new mytar::XTar{filename.c_str()});

	return true;
}

void XvaSt::read_xva(long long offset, size_t size, char** buffer) {
	if(tarfile == nullptr)
		return;

	std::map<long long, mytar::BlockPtr> out;
	tarfile->parsing([&](std::map<long long, mytar::BlockPtr> m){
		out = m;	
	}, 
	true);

	auto i = 10;
	for(auto it : out) { 
		if(i == 0) break ;
		std::cout << it.first << " "<<  it.second->filename << " " << it.second->offset << std::endl;
		i--;
	}

	auto iter = out.find(offset - 512);
	if(iter == out.end()) {
		std::cout << "doesn't found." << std::endl;
		return;
	}

	auto bl = iter->second;
	auto filesize = bl->filesize;
	std::cout << "catch file: " << bl->filename << std::endl;
	std::cout << "file size: " << filesize << std::endl;

	*buffer = new char[filesize];
	auto file = tarfile->back_file();
	file->seekg(offset);
	file->read(*buffer, filesize);
}

}
