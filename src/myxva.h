#pragma once
#include "mytar.h"

using NTarPtr = std::shared_ptr<mytar::NTar>;

namespace XVA {
	class XvaSt {
	public:	
		bool open_xva(const std::string& filename);
		void read_xva(long long offset, size_t size, char* buffer);
		void show_all_disk();
	private:
		NTarPtr tarfile;
	};
}
