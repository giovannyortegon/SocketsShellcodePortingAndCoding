/*
* TCPServer1.java
*
* TCP server socket program to bind, listen for
* request, print request and send response
* using TCP and HTTP 1.0 protocols.
*
* Usage:
*
* java TCPServer1 <local_port>
*
*
*/

import java.io.*;
import java.net.*;

public class TCPServer1
{
	public static void main(String [] args)
	{
		InputStreamReader isr = null;
		LineNumberReader lnr = null;
		OutputStream os = null;
		ServerSocket serv = null;
		InputStream is = null;
		Socket clnt = null;
		String send = null;
		String tmp = null;
		int port = 0;
		int x = 0;

		if (args.length != 1)
		{
			System.err.println("Usage: java TCPServer1 <local_port>");
			System.err.println("Example: java TCPServer1 80");
			System.exit(1);
		}

		tmp = args[0];

		try
		{
			// convert port value to integer
			port = Integer.parseInt(tmp);

			// init, bind, listen
			serv = new ServerSocket(port);

			System.out.println("**** Listening on por ****" + port);

			// accept new connection
			clnt = serv.accept();

			// get input stream
			is = clnt.getInputStream();

			// convert to LineNumberReader
			isr = new InputStreamReader(is);
			lnr = new LineNumberReader(isr);

			// reader request
			x = 0;

			while ((tmp = lnr.readLine()) != null)
			{
				System.out.println(x + ") " + tmp);
				++x;

				//handle double-newline HTTP request delimiter
				if (tmp.length() == 0)
				{
					break;
				}
			}

			// get output stream
			os = clnt.getOutputStream();

			// send response

			send = "HTTP/1.0 200 OK\r\n\r\nTCPServer1!";

			os.write(send.getBytes());

			// close client
			clnt.close();

			// close server
			serv.close();
		}
		catch (NumberFormatException nfe)
		{
			// non-numeric port value
			System.err.println("NumberFormatException: " + nfe.getMessage());
		}
		catch (IOException ioe)
		{
			// connection failed
			System.out.println("IOException: " + ioe.getMessage());
		}
	}
}
