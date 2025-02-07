#include <emp-tool/emp-tool.h>
#include "dependencies/emp-ag2pc/emp-ag2pc/2pc.h"


using namespace std;
using namespace emp;

int executeCircuit(bool * input, bool * output, string location, NetIO * io, int party){

	//Creating CircuitFile obj with our custom circuit
	CircuitFile cf(location.c_str());      

	cout<<"\n****Party\t"<<party<<"\t"<<location<<"****\n";

	//Calling necessary AG2PC Library functions		
	C2PC twopc(io, party, &cf);
	io->flush();
	twopc.function_independent();
	cout<<"\n"<<"Party\t"<<party<<"\t"<<"Independent\n";
	io->flush();
	twopc.function_dependent();
	io->flush();
	cout<<"\n"<<"Party\t"<<party<<"\t"<<"Dependent\n";

	//Function that executes the circuit with in input array
	cout<<"\nParty\t"<<party<<"\tCalling Online Function : "<<"\n";
	twopc.online(input,output);
	cout<<"\nParty\t"<<party<<"\tOnline Function Executed: "<<"\n";

	if (party==1)
		sleep(1);
	if (party == 2 && output[0]==1)
		return 1;
	else
		return 0;

}

const string circuit_file_location = macro_xstr(EMP_CIRCUIT_PATH);
int main(int argc, char** argv) {

	/* Edit this string to
	 * your corresponding input output directory !!
	 * Hardcoding it to avoid passing runtime arguments !!
	 * This directory contains the input files for Alice and Bob, the circuit file and the ouput file for BOB !!
	 */
	string input_dir="/home/zeeshan/Desktop/hashing_based/temp_dir/";

	/*
	*Code to create the networking connection between users 
	*/
	int port, party;
	parse_party_and_port(argv, &party, &port);
	//nullptr to connect to localhost; pass IP to connect to remote computers
	NetIO* io = new NetIO(party==ALICE ? nullptr:IP, port); 
	io->set_nodelay();
	

	//Reading the list_of_files_USER*.txt to process each frames
	string list_file = input_dir + "list_of_files_USER" + to_string(party) + ".txt";
        string individualFrames,data,data1,new1;

	

	//creating input stream to read file the list file
        ifstream i1; 
	i1.open(list_file);

	//temp string to hold first part of the absolute location of the file
	string temp= input_dir + "User" + to_string(party) + "_";
	int t= temp.length();

	int counter=0;
	cout<<"\nParty\t"<<party<<"\tStart\n";
	ifstream frameFile;
	//Reading each filename from the list of frame files
        while ( i1 >> individualFrames ){

		//creating input stream to read frame file from the filename location
		 
		frameFile.open(individualFrames);
		//cout<<"\n****"<<individualFrames<<"****\n";
		
		//Capturing the frame attributes from the filename
		//int fLength=individualFrames.length();
		data = individualFrames.substr(t,(individualFrames.length()-t));
		data.erase((data.length()-4),4);

		counter+=1;
		
		/*
		if (counter%100==0){
			cout<<"\n"<<"Party\t"<<party<<"\tI : "<<counter<<" "<<individualFrames;
			cout<<"\n"<<"Party\t"<<party<<"\tN : "<<counter<<" "<<data;
		}*/
		
	
		//Variables to capture the frame attributes
		string chromosome="",cm="",geneCount="";
		string delimiter = "_";
		int delims=0;
		string temp2="";
		//loop to traverse the string to split into tokens and finding frame attributes
		for (int i=0; (unsigned)i<data.size();i++){
			//cout<<data[i];
			if (data[i]=='_'){
				delims++;
				if (delims==1)
					chromosome =temp2;
				else
					cm=temp2;
				temp2="";
			} else
				temp2 = temp2 + data[i];		
		}
		geneCount=temp2;

		//cout<<"\n"<<counter<<"\t Step 2";
		
		
		cout<<"\n"<<"Party\t"<<party<<"\tC : "<<counter<<" "<<chromosome<<"\n";
		cout<<"\n"<<"Party\t"<<party<<"\tM : "<<counter<<" "<<cm<<"\n";
		cout<<"\n"<<"Party\t"<<party<<"\tG : "<<counter<<" "<<geneCount<<"\n";
		



		
		string circ_file= input_dir+"circuit_"+geneCount+".txt";
        	



//Problem Starts Here


		

		
		
		//Creating input and outpt array variables
		int input_len = atoi(geneCount.c_str())*3 ;
		bool in[input_len];
		bool out[1];
		string line;

		cout<<"\n"<<counter<<"  : Genotypes : "<<geneCount<<"\t Array Count "<<input_len;
		
		//cout<<"\nReading Input File Started !!"<<counter;
		//Reading the input data file of genotypes and putting into an array 
		if (frameFile.is_open()){
			for(int i=0; i<input_len; i++){
				getline(i1,line);
				in[i]=atoi(line.c_str());
			}
		}
		
		int result = executeCircuit(in, out, circ_file , io, party);
		
		if (party==2){
                        if (result==0)
				cout<<"\n****Chromosome\t"<<chromosome<<"\tCM\t"<<cm<<"-"<<atoi(cm.c_str())+5<<"\twith count\t"<<geneCount<<"\tis XXXXXXX****\n";
			else
				cout<<"\n****Chromosome\t"<<chromosome<<"\tCM\t"<<cm<<"-"<<atoi(cm.c_str())+5<<"\twith count\t"<<geneCount<<"\tis a Match****\n";
		}
		
		 
//Problem ends here
		
		frameFile.close();
			
        }
	cout<<"\nParty\t"<<party<<"\tEnd\n";
	i1.close();
	delete io;
	return 0;
}
