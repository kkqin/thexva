#include "Configure.h"
#include "myxva.h"

using namespace mytar;

int main(int argc, char* argv[]) {

	if(argc < 2) {
		std::cout << "args is less" << std::endl;
		return 0;
	}

	XVA::XvaSt da;
	if(!da.open_xva(argv[1]))
		std::cout << "failed." << std::endl;

	char *buffer = NULL;
	da.read_xva(512, 1048576, &buffer);

	return 0;	
}
