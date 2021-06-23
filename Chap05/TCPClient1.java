/*
 *
 * TCPClient1.java
 *
 * TCP Client socket program to connect, request
 * and received data using TCP and HTTP 1.0
 * protocols.
 *
 * Usage:
 *
 * java TCPClient1 <target_ip> <target_port> <resource>
 *
 *
 */

import java.io.*;
import java.net.*;

public class TCPClient1
{
	public static void main(String args[])
	{
		InputStream		is = null;
		OutputStream	os = null;
		Socket			sock = null;
		String			addr = null;
		String			res  = null;
		String			send = null;
		String			tmp = null;
		byte[]			recv = new byte[4096];
		int				port	= 0;
		int				len		= 0;

		if (args.length != 3)
		{
			System.err.println("usage: java TCPClient1 " +
							   "<target_ip> <target_port> " +
							   "<resource>.");

			System.err.println("Example: java TCPClient1" +
							   " 127.0.0.1 80 /");

			System.exit(1);
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

			// get connection input & output streams
			is = sock.getInputStream();
			os = sock.getOutputStream();

			// no exception thrown, connection established
			send = "GET " + res + " HTTP/1.0\r\n\r\n";

			// send HTTP request
			os.write(send.getBytes());

			// read response
			len = is.read(recv);

			// close connection
			sock.close();

			if (len > 0)
			{
				// convert  recv'd bytes to string..
				tmp = new String(recv);

				// display via stdout
				System.out.println(tmp);
			}
		}
		catch (NumberFormatException nfe)
		{
			// non-numeric port value?
			System.err.println("NumberFormatException: " +
							   nfe.getMessage());
		}
		catch (IOException ioe)
		{
			// connection failed
			System.err.println("IOException: " +
							   ioe.getMessage());
		}
	}
}
