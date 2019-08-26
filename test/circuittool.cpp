#include <emp-tool/emp-tool.h>
#include "dependencies/emp-ag2pc/emp-ag2pc/2pc.h"


using namespace std;
using namespace emp;

const string circuit_file_location = macro_xstr(EMP_CIRCUIT_PATH);
int main(int argc, char** argv) {

	/* Edit this string to
	 * your corresponding input output directory !!
	 * Hardcoding it to avoid passing runtime arguments !!
	 * This directory contains the input files for Alice and Bob, the circuit file and the ouput file for BOB !!
	 */
	string io_dir="input_output/";


	int port, party;
	parse_party_and_port(argv, &party, &port);
	NetIO* io = new NetIO(party==ALICE ? nullptr:IP, port); // TODO find some way to pass the IP!!!
	io->set_nodelay();
	

	string circ_file= io_dir+"DNA_MATCHING_CIRCUIT.txt";

	CircuitFile cf(circ_file.c_str());      //Creating CircuitFile obj with our custom circuit

	//auto t1 = clock_start();
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
        if (i1.is_open()){
               for(i=0; i<input_len; i++){
                       getline(i1,line);
                       in[i]=atoi(line.c_str());
               }
        }
        twopc.online(in,out);
	i1.close();

	//cout << endl<<circ_file <<"\t"<<time_from(t1)<<endl;

	string output = io_dir + "out_party_" + to_string(party) + ".txt";
	ofstream output_1 (output);
	if (output_1.is_open()){  // Put output data into file

		for (i=0; (unsigned)i<sizeof(out); i++)
     			output_1<<out[i]<<endl;
		output_1.close();
	}
	else cout<<"Unable to Write into file !!!";
	
	//cout<<"\n\n\n \t\t Data for Party : "<< party;
	//cout<<"\nInput Length : "<<input_len;
	//cout<<"\nInput File : "<<input_file;
	cout<<"\nInput  ";
	for (i=0; i<input_len; i++)
		cout<<"\t"<<in[i];
	
	
	if (party==2){
	cout<<"\nOutput  ";
	
	for(i=0; i<cf.n3; i++)
		cout<<"\t"<<out[i];
	//cout<<"\n\nOutput_file : "<<output<<"\n\n";
	}

	delete io;
	return 0;

}
