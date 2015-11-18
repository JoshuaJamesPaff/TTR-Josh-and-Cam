
package source;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;

/**
 * Client application class
 * Establishes a connection, reads strings from socket and allows message sending.
 */
public abstract class TTRClientThread implements Runnable {
	
	/**
	 * BufferedReader object to read from socket.
	 */
	private BufferedReader socketReader;
	
	/**
	 * PrintWriter object to write to socket.
	 */
	private PrintWriter	socketWriter;
	
	/**
	 * Client thread.
	 */
	private Thread	runner;
	
	/**
	 * Server host name.
	 */
	protected String hostName;
	
	/**
	 * Server port.
	 */
	protected int port;
	
	/**
	 * Client quit flag.
	 */
	protected boolean quit;
	
	/**
	 * Client connected flag.
	 */
	protected boolean connected = false;
	
	/**
	 * Constructor.
	 * @warning does not initialize server variables.
	 */
	public TTRClientThread()
	{
		this.runner = new Thread(this, "runner");
		quit = false;
	}
	
	/**
	 * Constructor.
	 * @param name thread name.
	 * @param host server name/address.
	 * @param port server port.
	 */
	public TTRClientThread(String name, String host, int port)
	{
		this.runner = new Thread(this, name);
		this.hostName = new String(host);
		this.port = port; 
		quit = false;
	}
	
	/**
	 * Start client thread.
	 */
	public void start() 
	{
		runner.start();
	}
	
	/**
	 * Set server.
	 * @param host host name / address.
	 */
	public void setHost(String host) 
	{
		this.hostName = new String(host);
	}
	
	/**
	 * Set port.
	 * @param port port.
	 */
	public void setPort(int port)
	{
		this.port = port;
	}
	
	public int localPort = 0;
	/**
	 * Attempt to connect to server.
	 * @return true if success, false otherwise.
	 */
	protected boolean connect() 
	{
        try {
        	Socket socket = new Socket(hostName, port);
        	localPort = socket.getLocalPort();
        	//socket.setSoTimeout(200); socket.setKeepAlive(true);
            socketReader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            socketWriter = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()));
        } 
        catch(Exception e) 
        { 
        	e.printStackTrace(); return false;
        }
        connected = true; 
        return true;
    }
	
	/**
	 * Read string from socket. 
	 * @return read string.
	 * @throws Exception
	 */
	protected String receive() throws Exception 
	{
		String tmp = socketReader.readLine();
		System.out.println("received: "+tmp);
		return tmp;
	}
	
	/**
	 * Write string to socket.
	 * @param s string to be written.
	 */
	protected void send(String s)
	{
		System.out.println("sending: "+s);
		socketWriter.println(s); socketWriter.flush();
	}
	
	/**
	 * Thread run method. Started when start() is called.
	 * Connects to server and reads strings from  socket.
	 * If disconnected calls onDisconnect(); otherwise
	 * calls commandHandler(read string).
	 */
	public void run() 
	{
		while (!quit) {
			if (connected) {
				String buffer = null;
				try {
					buffer = receive();
				} catch (Exception e) {
					e.printStackTrace();
				}
				if (buffer == null) {
					connected = false;
					quit = true;
					onDisconnect();
				}
				else {
					commandHandler(buffer);
				}
			}
		}
	}

	/**
	 * Abstract method. Handles commands.
	 * @param cmd command to handle.
	 */
	public abstract void commandHandler(String cmd);
	
	/**
	 * Abstract method. Handles disconnects.
	 */
	public abstract void onDisconnect();
	
	/**
	 * @warning unused.
	 */
	public abstract void onStatus();
}
