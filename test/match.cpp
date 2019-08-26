#include <emp-tool/emp-tool.h>
#include "dependencies/emp-ag2pc/emp-ag2pc/2pc.h"


using namespace std;
using namespace emp;

const string circuit_file_location = macro_xstr(EMP_CIRCUIT_PATH);
int main(int argc, char** argv) {

	string io_dir="input_output/";


	int port, party;
	parse_party_and_port(argv, &party, &port);
	NetIO* io = new NetIO(party==ALICE ? nullptr:IP, port); // TODO find some way to pass the IP!!!
	io->set_nodelay();

	string circ_file= io_dir+"DNA_MATCHING_CIRCUIT.txt";

	CircuitFile cf(circ_file.c_str());      //Creating CircuitFile obj with our custom circuit

	auto t1 = clock_start();
	C2PC twopc(io, party, &cf);
	twopc.function_independent();
	twopc.function_dependent();

	int i;
	int input_len = (party == ALICE) ? cf.n1 : cf.n2 ;
	bool in[input_len];
	bool out[cf.n3];
	string input_file = io_dir + "input_party_" + to_string(party) + ".txt";
	string line;
	ifstream i1 (input_file);

}
