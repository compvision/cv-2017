import java.net.*;
import java.io.*;
import edu.wpi.first.wpilibj.networktables.NetworkTable;

class NTable
{
    public static void main(String [] args) throws Exception
    {
        String serverName = "localhost";
        int port = 3341; 
		Socket client = new Socket(serverName, port);
        OutputStream outToServer = client.getOutputStream();
           
		DataOutputStream out = new DataOutputStream(outToServer);	
    	InputStream inFromServer = client.getInputStream();
		DataInputStream in = new DataInputStream(inFromServer);
	
		try
        {
            //client = new Socket(serverName, port);
            //outToServer = client.getOutputStream();
            //out = new DataOutputStream(outToServer);

            //inFromServer = client.getInputStream();
            //in = new DataInputStream(inFromServer);

            NetworkTable.setClientMode();
            NetworkTable.setIPAddress("roboRIO-3341-FRC.local");
            NetworkTable table = NetworkTable.getTable("cv");
            //Main loop
            String targetDataRaw = null;
            while(true)
            {
                //send the value request
                out.write('\n');

                //grab the values
                
                Thread.sleep(100); // Update values every 100 ms
                System.out.print("ping\n"); 
				//System.out.print(in.readLine()+"\n");
                
				targetDataRaw = in.readLine();
                System.out.print("waiting\n");
                if(targetDataRaw == null)
                    //break;
                    continue;
                if(targetDataRaw.isEmpty())
                    continue;

                // Implements the FRC 2015 Network Tables Protocol
                System.out.print(targetDataRaw + "\n");
                boolean Gear, Boiler;
                double gearDistance = 0, gearAzimuth = 0, gearAltitude = 0, boilerDistance = 0, boilerAzimuth = 0, boilerAltitude = 0;
                String[] parsed = targetDataRaw.split(";"); 
                Gear = parsed[0].equals("true");
				Boiler = parsed[1].equals("true");
                //System.out.println(parsed[0]); 
                if(Gear)
                {
                    gearDistance = Double.parseDouble(parsed[2]);
                    gearAzimuth = Double.parseDouble(parsed[3]);
					gearAltitude = Double.parseDouble(parsed[4]);
                    System.out.println("Gear found!");
                    System.out.println("Distance: " + gearDistance);
                    System.out.println("Azimuth: " + gearAzimuth); 
					System.out.println("Altitude: " + gearAltitude); 
                    System.out.println(); 
                    table.putBoolean("gearFound", true);
                    table.putNumber("gearDistance", gearDistance);
                    table.putNumber("gearAzimuth", gearAzimuth);
					table.putNumber("gearAltitude", gearAltitude);
                }
				else
				{	
                  	System.out.println("Gear not found");
					table.putBoolean("gearFound", false);
				}	

				if(Boiler)
                {
                    boilerDistance = Double.parseDouble(parsed[5]);
                    boilerAzimuth = Double.parseDouble(parsed[6]);
					boilerAltitude = Double.parseDouble(parsed[7]);
                    System.out.println("Boiler found!");
                    System.out.println("Distance: " + boilerDistance);
                    System.out.println("Azimuth: " + boilerAzimuth); 
					System.out.println("Altitude: " + boilerAltitude); 
                    System.out.println(); 
                    table.putBoolean("boilerFound", true);
                    table.putNumber("boilerDistance", boilerDistance);
                    table.putNumber("boilerAzimuth", boilerAzimuth);
					table.putNumber("boilerAltitude", boilerAltitude);
                }
                else
                {                
					System.out.println("Boiler not found");
					table.putBoolean("boilerFound", false);
				}
					
					
                    System.out.println(); 
       		}
       } 
           
		catch(IOException e)
		{
				/*try
				{ // just in case null pointer exception try/catch
					
				}
				catch(Exception f)
				{
					f.printStackTrace();			
				}*/
            e.printStackTrace();
        }
		finally
		{
			out.close();
           	in.close();
           	client.close();
		}
	}    
}
