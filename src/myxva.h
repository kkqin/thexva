#pragma once
#include "mytar.h"

using XTarPtr = std::shared_ptr<mytar::XTar>;

namespace XVA {
	class XvaSt {
	public:	
		bool open_xva(const std::string& filename);
		void read_xva(long long offset, char** buffer);
	private:
		XTarPtr tarfile;
	};
}
