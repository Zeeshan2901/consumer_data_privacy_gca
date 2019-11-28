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
	

	string circ_file= io_dir+"frame_circuit.txt";

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
        if (i1.is_open()){
               for(i=0; i<input_len; i++){
                       getline(i1,line);
                       in[i]=atoi(line.c_str());
               }
        }


	//memset(in, false, input_len);
	//memset(out, false, cf.n3);
	
	

        twopc.online(in,out);
	i1.close();

	cout << endl<<circ_file <<"\t"<<time_from(t1)<<endl;

	//cout<<"time in secs"<<"\t"<<t1/CLOCKS_PER_SEC<<" sec";	
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
	//cout<<"\nInput ";
	//for (i=0; i<input_len; i++)
	//	cout<<"\t"<<in[i];
	
	
	if (party==2){
	cout<<"\nOutput ";
	cout<<"\n DNA Matching outputs ***************";
	cout<<"\nFor 700 inputs***";
	for(i=4209; i<=9801; i+=8){
		cout<<"\t"<<i<<" : "<<out[i]<<"\n";
	}
	cout<<"For Fabricated inputs ***";
	for (i=9809;i<=12393; i+=8){
		cout<<"\t"<<i<<" : "<<out[i]<<"\n";
	}
	cout<<"Half Adder Outputs";
	for (i=12394;i<=13417;i++){
		cout<<"\t"<<i<<" : "<<out[i]<<"\n";
	}
	cout<<"\nFull Adder Outputs";
	for(i=13424;i<=25609;i+=8){
		cout<<"\t"<<i<<" : "<<out[i]<<"\n";
		cout<<"\t"<<i+1<<" : "<<out[i+1]<<"\n";
	}
	cout<<"Final Adder Output:";
	for(i=25537;i<=25609;i+=8){
		cout<<"\t"<<i<<" : "<<out[i]<<"\n";
	}
	cout<<"\t"<<"25608"<<" : "<<out[25608]<<"\n";
	//cout<<"\n\nOutput_file : "<<output<<"\n\n";
	cout<<"less than t2 outputs :";
	for (i=25610;i<=25630;i++)
		cout<<"\t"<<i<<" : "<<out[i]<<"\n";
	
	}

	delete io;
	return 0;

}
