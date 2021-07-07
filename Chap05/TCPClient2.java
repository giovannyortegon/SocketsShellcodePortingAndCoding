/*
 * TCPClient2.java
 *
 *
 * TCP client socket program to connect, request
 * and receive data using TCP and HTTP 1.0
 * protocol. Read and display response line by
 * line using LineNumberReader class
 *
 * Usage:
 *
 * java TCPClient2 <target_ip> <target_port> <resource>
 *
 */

import java.io.*;
import java.net.*;

public class TCPClient2 {
	public static void main(String [] args) {
		InputStreamReader isr = null;
		LineNumberReader lnr = null;
		InputStream is = null;
		OutputStream os = null;
		Socket sock = null;
		String addr = null;
		String res = null;
		String send = null;
		String tmp = null;
		byte [] recv = new byte [4096];
		int port = 0;
		int x = 0;

		if (args.length != 3)
		{
			System.err.println("usage: java TCPClient2 " +
								"<target_ip> <target_port> " +
								"<resource>.");

			System.err.println("Example: java TCPClient2 " +
								"127.0.0.1 80 /");
		}

		addr = args[0];
		tmp = args[1];
		res = args[2];

		try
		{
			// convert port value to integer
			port = Integer.parseInt(tmp);

			// connect to IP address and port
			sock = new Socket(addr, port);

			// get connection output stream
			os = sock.getOutputStream();

			// format HTTP request
			send = "GET " + res + "HTTP/1.0\r\n\r\n";

			// send HTTP request
			os.write(send.getBytes());

			// get connection input stream
			is = sock.getInputStream();

			// convert to LineNumberReader
			isr = new InputStreamReader(is);
			lnr = new LineNumberReader(isr);

			// read & display response line by line
			x = 0;

			while ((tmp = lnr.readLine()) != null)
			{
				System.out.println(x + ") " + tmp);
				++x;
			}

			// close connection
			sock.close();
		}
		catch (NumberFormatException nfe)
		{
			// non-numeric port value
			System.err.println("NumberFormatException: " + nfe.getMessage());
		}
		catch (IOException ioe)
		{
			// connection failed
			System.err.println("IOException: " + ioe.getMessage());
		}
	}
}
