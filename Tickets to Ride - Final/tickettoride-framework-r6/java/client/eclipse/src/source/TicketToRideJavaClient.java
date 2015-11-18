
package source;

//import datastruct.*;

public class TicketToRideJavaClient {

	public static void main(String[] args) {
		
		//TTRMap map = new TTRMap(); map.loadFromFile("europe.xml");
		
		TTRClient t = new RandomSolution(
			"JavaUser", "JavaUser", "RandomJavaAgent", "127.0.0.1", 1337);
		t.start(); 
		
		try {Thread.sleep(100);
		} catch (InterruptedException e) {
			e.printStackTrace();}
		t.go();
		//t.join();
	}

}
