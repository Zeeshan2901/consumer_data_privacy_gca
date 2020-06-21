import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.*;
import java.util.ArrayList;
import java.util.HashMap; 
import java.util.Collections;
import java.util.List;
import java.util.stream.IntStream;



public class User1_Comms {
	//Declaring Communication Objects
	private Socket	socket			= null;
	private ServerSocket server		= null;
	private DataInputStream serverIn	= null;
	private int port;
	
	//CHROMOSOME Count Constant
	final static int CHROMOSOME_COUNT = 22;

	//Necessary Data Structures
	ArrayList<FrameData>[] frames;	//holds frame details

	String test_file="";

	String user1,user2;
	double java_time,gc_time;
	int user1Snips, user2Snips;

	public User1_Comms(String u1, String u2, String jv, String gc, String u1s, String u2s ) {
		// TODO Auto-generated constructor stub
		port = 12345;
		test_file="case5.csv";
		frames	= new ArrayList[CHROMOSOME_COUNT+1];
		for (int i = 1; i <= CHROMOSOME_COUNT; i++) 
			frames[i]= new ArrayList<FrameData>();

		user1=u1.substring(u1.length()-6, u1.length()).replaceAll("[^0-9]","");
		user2=u2.substring(u2.length()-6, u2.length()).replaceAll("[^0-9]","");
		java_time=Double.parseDouble(String.format("%.3f", Double.parseDouble(jv))) ;
		gc_time=Double.parseDouble(String.format("%.3f", Double.parseDouble(gc))) ;
		user1Snips=Integer.parseInt(u1s);
		user2Snips=Integer.parseInt(u2s);
	}

	//Executioner Method
	public void executeComms() {
		// try block to create connection
		try {
			server = new ServerSocket(port);
			System.out.println("Server started");
			System.out.println("Waiting for a client ...");
			socket = server.accept();
			System.out.println("Connection established with Client ");
			System.out.println("Just connected to " + socket.getRemoteSocketAddress());
		} catch (IOException i) {
			System.out.println(i);
		}
		
		// block to create input, output objects
		try {
			// takes input from the client socket
			serverIn = new DataInputStream(new BufferedInputStream(socket.getInputStream()));
		} catch (IOException i) {
			System.out.println(i);
		}
		
		receiveFrameData();
		matchStats();

		// block to close connections
		try {
			socket.close();
			server.close();
			serverIn.close();
			System.out.println("\tConnection closed for User 1  !!!");
		} catch (IOException i) {
			System.out.println(i);
		}
	}

	public void receiveFrameData(){
	
		try{
			int comms=Integer.parseInt(serverIn.readUTF());
			int count=0;
			for (int i =0; i<comms; i++){
				String []temp = serverIn.readUTF().split(",");
				FrameData obj =new FrameData(Integer.parseInt(temp[1]) , Integer.parseInt(temp[2]), Boolean.parseBoolean(temp[3]) );
				ArrayList <FrameData> frame =  frames[Integer.parseInt(temp[0])];
	            		frame.add(obj);
				count++;
			}
			IntStream.range(1,CHROMOSOME_COUNT+1).parallel().forEach(x -> Collections.sort(frames[x]));
			System.out.println("Count : " +count + " Comms : "+comms);
		} catch(IOException e){
			e.printStackTrace();
		}

	}

	public static void main(String[] args) throws IOException {
		// TODO Auto-generated method stub
		User1_Comms client = new User1_Comms( args[0], args[1], args[2], args[3], args[4], args[5]);		
		client.executeComms();
	}

	public void matchStats() {
		System.out.println("\n\nMatch Stats \n");
		ArrayList<String> segments= new ArrayList<String>(CHROMOSOME_COUNT);
		ArrayList<Integer> chromosomeWiseTotalCM= new ArrayList<Integer>(CHROMOSOME_COUNT);
		ArrayList<Integer> chromosomeWiseMatchCM= new ArrayList<Integer>(CHROMOSOME_COUNT);
		segments.add("");
		chromosomeWiseTotalCM.add(0);
		chromosomeWiseMatchCM.add(0);
		int cMPerFrame=5;
		int totalFrames=0;
		int matchFrames=0;
		int matchcM=0;
		int totalcM=0;
		int mathingSegments=0;
		int common_snips=0;
		int padded_snips=0;
			
		for (int i=1;i<=CHROMOSOME_COUNT;i++) {
			int matchFrame=0;			//Number of matching Frames
			int matchCMcount=0;			//Matching cM
			int cMCount=0;				//Number of CM
			int start=0;				//Start of a matching segment
			int end=0;				//End of a matching segment
			String len="";				//Segment String for each chromosome
			int tempSt=0, tempEnd=0;
			int startOfCM=0, endOfCm=0;		//CM end Points
			int currentChromosomeTotalFrames = frames[i].size();
			for (int j=0;j<currentChromosomeTotalFrames;j++) {
				FrameData obj = frames[i].get(j);
				if (j==0 || j%5==0){
					common_snips+=obj.geneCount;
					padded_snips+=obj.paddedCount;
				}
				if (startOfCM==0 && endOfCm==0) {
					startOfCM=obj.start;
					endOfCm=obj.end;
				}
				else if (startOfCM >=0 && endOfCm >0 && endOfCm == (obj.end -1))
					endOfCm=obj.end;
				else {
					cMCount += endOfCm-startOfCM;
					if (cMCount > obj.start && j < currentChromosomeTotalFrames)
						startOfCM=cMCount;
					else
						startOfCM=obj.start;
					endOfCm=obj.end;
				}
				 
				if (j==currentChromosomeTotalFrames -1) {
					if (cMCount > obj.start )
						cMCount += endOfCm-cMCount;
					else	
						cMCount += endOfCm-startOfCM;
				}

				
				if (obj.match) {
					matchFrame++;
					if (start == 0 && end == 0 && tempEnd == 0 && tempSt == 0) {
						start=obj.start;
						end=obj.end;
					}
					else if (obj.end > tempSt && tempSt >0) {
						/*if (obj.end > end){
							len += "[ "+start+"cM - " +end +"cM ]";
							matchCMcount += (end-start);
							mathingSegments++;
							tempSt=tempEnd=0;
						}*/
						end=obj.end;
						if ((obj.end - end ) >= cMPerFrame )
							start= obj.start;
						
					}
					else if ((obj.end - end ) <= cMPerFrame ) {
						end=obj.end;
					}
					else if (obj.end - end > cMPerFrame){
						len += "[ "+start+"cM - " +end +"cM ]";
						matchCMcount += (end-start);
						mathingSegments++;
						tempSt=tempEnd=0;
						start= obj.start;
						end=obj.end;
					}
				}
				if (!obj.match && end >0) {
					tempSt=start;
					tempEnd=end;
					if (end >0 && tempEnd > obj.start) {}
					else {
						len += "[ "+start+"cM - " +end +"cM ]";
						matchCMcount += (end-start);
						mathingSegments++;
						tempSt=tempEnd=0;
						start=end=0;
					}
				}
				if (currentChromosomeTotalFrames > 0 && start >=0 && end >0) {
					if (j==currentChromosomeTotalFrames-1) {
						matchCMcount += (end-start);
						if ( end >0) {
							len += "[ "+start+"cM - " +end +"cM ]";
							mathingSegments++;
						}
					}
				}	
			}
			totalFrames+=currentChromosomeTotalFrames;
			matchFrames+=matchFrame;
			matchcM+=matchCMcount;
			totalcM+=cMCount;
			segments.add(len);
			chromosomeWiseMatchCM.add(matchCMcount);
			chromosomeWiseTotalCM.add(cMCount);
		}
		
		
		System.out.println("\n\n\n\t\t\tMatching Segments");
		System.out.println("Chromosome\tTotal cM\tMatching cM\tMatching Segments");
		for (int i=1;i<=CHROMOSOME_COUNT;i++) 
			System.out.println( i+"\t"  +"\t" +chromosomeWiseTotalCM.get(i)+"\t"+"\t" +chromosomeWiseMatchCM.get(i) 
					+ "\t"+"\t"+ segments.get(i));
		System.out.println("\n\n\nOverall Total    Frames are	"+ totalFrames);
		System.out.println("Overall Matching Frames are	"+ matchFrames);
		System.out.println("Overall Total	 CMs	are	"+ totalcM);
		System.out.println("Overall Matching CMs	are	"+ matchcM);

		int id=findLastline(test_file);
		String appString = id + "," + user1 + "," + user2 + "," + 
				totalFrames + "," + matchFrames + "," + totalcM + "," + matchcM + "," + mathingSegments + "," +
				java_time + "," + gc_time + "," + (java_time+gc_time) + "," + user1Snips + "," + user2Snips + "," +
				common_snips + "," + padded_snips;
		append(test_file,appString);
	}


	public int findLastline(String filename) {
		String s="",lastline="";
		try {
			FileReader fr = new FileReader(filename);
			BufferedReader bf = new BufferedReader(fr);
			while ( ( s= bf.readLine()) != null) {
				lastline=s;
			}
			bf.close();
			fr.close();
		} catch (IOException e) {
			System.out.println(e); 
		}
		String [] temp = lastline.split(",");
		return (Integer.parseInt(temp[0]) + 1);
	}
	public void append(String file, String appendString) {
		
		try {
			FileWriter fw = new FileWriter(file, true);
			BufferedWriter bw = new BufferedWriter(fw);
			PrintWriter out = new PrintWriter(bw);	
			out.println(appendString);
			out.close();
			bw.close();
			fw.close();
			
		} catch (IOException e) {
			System.out.println(e); 
		}
	}
	
	public static int nextPowerOf2 (int num) {
		
		if (num <= 0 || num > 65536)
			return -1;
		if (num > 0 && ((num & (num - 1)) == 0))
			return num;
		else 
			return (int) Math.pow(2, (int) ( (Math.log(num) / Math.log(2)) + 1));
			
	}



}
