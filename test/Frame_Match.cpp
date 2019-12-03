#include <emp-tool/emp-tool.h>
#include "dependencies/emp-ag2pc/emp-ag2pc/2pc.h"


using namespace std;
using namespace emp;

int main(int argc, char** argv) {

	/*
	* Capture relevant command line arguments
	*/
	string file_dir(argv[3]);			//input/User1/  or 2/ 
	string input_file(argv[4]);			//1_0_123.txt
	string circuit(argv[5]);			//circuit_1148.txt
	
	
	/*
	* Create the networking connection between users 
	*/
	int port, party;
	parse_party_and_port(argv, &party, &port);
	//nullptr to connect to localhost; pass IP to connect to remote computers
	NetIO* io = new NetIO(party==ALICE ? nullptr:IP, port);
	io->set_nodelay();
	

	


	/*
	* Creating CircuitFile obj with our custom circuit
	*/	
	string circuit_file = "src/"+file_dir+circuit;		//input/User1/circuit_121.txt
	CircuitFile cf(circuit_file.c_str());	
	
	/*
	* Calling necessary AG2PC Library functions
	*/		
	C2PC twopc(io, party, &cf);
	io->flush();
	twopc.function_independent();
	io->flush();
	twopc.function_dependent();
	io->flush();

	/*
	*Creating input and output array variables
	*/
	int i;
	int input_len = (party == ALICE) ? cf.n1 : cf.n2 ; 
	bool in[input_len];
	bool out[cf.n3];


	/*
	* Reading Input from the file to the array
	*/
	input_file = "src/"+file_dir+input_file;
	string line;
	ifstream i1 (input_file);
        if (i1.is_open()){
               for(i=0; i<input_len; i++){
                       getline(i1,line);
                       in[i]=atoi(line.c_str());
               }
        }

	/*
	* Executong the circuit file on the input array to get output
	*/	
	twopc.online(in,out);
	i1.close();
	delete io;	
	
	if (party == 2 && out[0]==1){
		cout <<"\nFor Input FIle : "<<input_file <<" "" Its a Match !!!\n\n"	;	
		return 1;
	}
	else if (party == 2 && out[0]==0)
		cout <<"\nFor Input FIle : "<<input_file <<" Its a XXXXX !!!\n\n"	;
	

	return 0;

}
