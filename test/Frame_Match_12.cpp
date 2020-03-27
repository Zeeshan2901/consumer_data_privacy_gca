const static int runs = 12;

#include <emp-tool/emp-tool.h>
#include "dependencies/emp-ag2pc/emp-ag2pc/amortized_2pc.h"

using namespace std;
using namespace emp;

int main(int argc, char** argv) {

	/*
	* Capture relevant command line arguments
	*/
	string dir(argv[3]);				//input/User
	string lof(argv[4]);				//128.txt or 256.txt , etc.
	string circuit(argv[5]);			//version_2/circuit/512.txt or 2048.txt , etc.
	

	/*
	* Create the networking connection between users 
	*/
	int port, party;
	parse_party_and_port(argv, &party, &port);
	//nullptr to connect to localhost; pass IP to connect to remote computers
	NetIO* io = new NetIO(party==ALICE ? nullptr:IP, port);
	io->set_nodelay();
	string file_dir = dir + to_string(party) + "/";

	/*
	* Creating CircuitFile obj with our custom circuit
	*/	
	CircuitFile cf(circuit.c_str());
	

	if (party==2){
		cout<<"\nDIR : "<<dir;
		cout<<"\nLOF : "<<lof;
		cout<<"\nCIR : "<<circuit;
		cout<<"\nRUN : "<<runs;
	}
	
	/*
	* Calling necessary AG2PC Library functions
	*/		
	AmortizedC2PC<runs> twopc(io, party, &cf);
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

	int count =0;
	/*
	* Reading Each input file from list of file
	*/
	ifstream i1(lof);
	string line;
	if (i1.is_open()){
		while ( getline(i1,line) )
		{
			if (count == runs)
				return 0;
			//Variables to capture the frame attributes
			string chromosome="",cm="",geneCount="";
			string delimiter = "_";
			int delims=0;
			string temp2="";
			//loop to traverse the string to split into tokens and finding frame attributes
			for (int i=0; (unsigned)i<line.size();i++){
				//cout<<data[i];
				if (line[i]=='_'){
					delims++;
					if (delims==1)
						geneCount =temp2;
					else
						chromosome=temp2;
					temp2="";
				} else
					temp2 = temp2 + line[i];		
			}
			cm=temp2;

			/*cout<<"\nline		: "<<line;
			cout<<"\ngeneCount	: "<<geneCount;
			cout<<"\nchromosome	: "<<chromosome;
			cout<<"\nCM		: "<<cm;*/
			string inputfile = file_dir + line + ".txt";
			ifstream i2 (inputfile);
			string lines;
			if (i2.is_open()){
				for(i=0; i<input_len; i++){
					getline(i2,lines);
					in[i]=atoi(lines.c_str());
               			}
			}
			twopc.online(in,out);
			count++;
			i2.close();
			/*
			* Writing Results of test
			*/
			if (party==2 && out[0]==1 ){
				ofstream writeFile; 
				writeFile.open("version_2/matching_results.txt" , std::ios_base::app);
				writeFile <<chromosome<<","<<geneCount<<","<<cm <<",true\n";
				writeFile.close();
				//cout<<"Chr : "<<chromosome<<" || GC : "<<geneCount<<" || CM : "<<cm <<" is a Match\n";
			}else if (party == 2 && out[0]==0){
				ofstream writeFile; 
				writeFile.open("version_2/matching_results.txt" , std::ios_base::app);
				writeFile <<chromosome<<","<<geneCount<<","<<cm <<",false\n";
				writeFile.close();
				//cout<<"Chr : "<<chromosome<<" || GC : "<<geneCount<<" || CM : "<<cm <<" is a Nopes\n";
			}
			//sleep (10);
					
		}
        }
	cout<<"\n\nNumber of Files Processed :"<<count;
	i1.close();
	delete io;
	return 0;
}















	

