import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.stream.IntStream;


public class User2_Comms{
	//Declaring Communication Objects
	private Socket 		socket		= null; 
	private DataOutputStream clientOut	= null; 
	String clientAddress;
	int port;
	
	//Location of the input File
	private String location;
	
	
	//CHROMOSOME Count Constant
	final static int CHROMOSOME_COUNT =22;

	//Necessary Data Structures
	ArrayList<FrameData>[] frames;	//holds frame details

	int count;

	public User2_Comms(String loc) {
		// TODO Auto-generated constructor stub
		clientAddress="127.0.0.1"; 
		port=12345;
		
		location=loc;
		frames  = new ArrayList[CHROMOSOME_COUNT+1]; 
		count=0;

		for (int i=1; i<=CHROMOSOME_COUNT; i++) 
			frames[i]= new ArrayList<FrameData>();
		
	}

	//Executioner Method
	public void executeComms() throws IOException {
		//Block to establish the connection 
		try{ 
			socket = new Socket(clientAddress, port); 
			System.out.println("Connected"); 
			System.out.println("Connection established with Server ");
			System.out.println("Just connected to " + socket.getRemoteSocketAddress()); 
		} catch(UnknownHostException u) { 
			System.out.println(u); 
		} catch(IOException i){ 
        		System.out.println("Exception at Client : "+i); 
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
        		try{ 
    				socket = new Socket(clientAddress, port); 
    				System.out.println("Connected"); 
    				System.out.println("Connection established with Server ");
    				System.out.println("Just connected to " + socket.getRemoteSocketAddress()); 
 			} catch(UnknownHostException u) { 
				System.out.println(u); 
			} catch(IOException x){ 
            			System.out.println("Exception at Client : "+x); 
			}
		}
		
        	//block to create input, output objects
  		try {
			// sends output to the socket 
			clientOut= new DataOutputStream(socket.getOutputStream());
  		} catch(IOException i) { 
			System.out.println(i); 
		}
  		
  		createFrameData();
		sendFrameData();

		// block to close connections
		try {
			socket.close();
			clientOut.close();
			System.out.println("\tConnection closed  for User 2   !!!");
		} catch (IOException i) {
			System.out.println(i);
		}
	}
	
	public void createFrameData() {
	
		try{
			String s="";
			FileReader fr = new FileReader(location);
			BufferedReader bf = new BufferedReader(fr);
			while ( (s= bf.readLine()) != null) {
				String []temp=s.split(",");
				FrameData obj =new FrameData(Integer.parseInt(temp[2]) , Integer.parseInt(temp[1]), Boolean.parseBoolean(temp[3]) );
				ArrayList <FrameData> frame =  frames[Integer.parseInt(temp[0])];
			    	frame.add(obj);
				count++;
			}
			bf.close();
			IntStream.range(1,CHROMOSOME_COUNT+1).parallel().forEach(x -> Collections.sort(frames[x]));
		} catch(Exception e){
			e.printStackTrace();
		}

	}

	public void sendFrameData(){
		try{
			clientOut.writeUTF(Integer.toString(count));
			System.out.println("Size : "+count);
			for (int i =1; i <=CHROMOSOME_COUNT; i++){
				for( int j=0; j<frames[i].size(); j++ ){
					FrameData obj = frames[i].get(j);
					String send = i + "," + obj.start + "," + obj.geneCount + "," + obj.match;
					clientOut.writeUTF(send);
					//System.out.println(send);
				}
			}
		} catch(IOException e){
			e.printStackTrace();
		}
	}

	public static void main(String[] args) throws IOException {
		// TODO Auto-generated method stub
		User2_Comms client = new User2_Comms(args[0]);		
		client.executeComms();
	}
}
