public class FrameData implements Comparable <FrameData> {
	
	int start,end, geneCount, paddedCount;
	boolean match;
	
	public FrameData(int st, int gc, boolean mt) {
		start=st;
		geneCount=gc;
		end=start+5;
		match=mt;
		paddedCount=nextPowerOf2(gc)-geneCount;
	}
	
	
	public void display(FrameData ob, int chromo) {
		System.out.println("\n Chromosome : "+chromo+" || Start : " +ob.start+ " || End : "+ob.end+ " || Match : "+ob.match
					+ " || GeneCount : " + ob.geneCount + " || PaddedCount : " + ob.paddedCount);
	}
	
	public int compareTo(FrameData fd) {
		if (start==fd.start)
			return 0;
		if (fd.start < start)
			return 1;
		else 
			return -1;
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

