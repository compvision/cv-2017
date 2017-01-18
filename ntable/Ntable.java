import java.net.*;
import java.io.*;
import edu.wpi.first.wpilibj.networktables.NetworkTable;

class NTable
{
    public static void main(String [] args) throws Exception
    {
        String serverName = "localhost";
        int port = 3341; 
        try
        {
            Socket client = new Socket(serverName, port);
            OutputStream outToServer = client.getOutputStream();
            DataOutputStream out = new DataOutputStream(outToServer);

            InputStream inFromServer = client.getInputStream();
            DataInputStream in = new DataInputStream(inFromServer);

            /*NetworkTable.setClientMode();
            NetworkTable.setIPAddress("10.33.41.2");
            NetworkTable table = NetworkTable.getTable("cv");
            //Main loop
            while(true)
            {
                //send the value request
                out.write('\n');

                //grab the values
                Thread.sleep(100); // Update values every 100 ms
                String targetDataRaw = null;
                targetDataRaw = in.readLine();
                if(targetDataRaw == null)
                    break;
                if(targetDataRaw.isEmpty())
                    continue;*/

                // Implements the FRC 2015 Network Tables Protocol
                boolean foundGear, foundBoiler;
                double distance = 0, azimuth = 0, altitude = 0;
                String[] parsed = targetDataRaw.split(";"); 
                foundGear = parsed[0].equals("true");
				foundBoiler = parsed[1].equals("true");
                //System.out.println(parsed[0]); 
                if(foundGear)
                {
                    distance = Double.parseDouble(parsed[2]);
                    azimuth = Double.parseDouble(parsed[3]);
					altitude = Double.parseDouble(parsed[4]);
                    System.out.println("Gear found!");
                    System.out.println("Distance: " + distance);
                    System.out.println("Azimuth: " + azimuth); 
					System.out.println("Altitude: " + altitude); 
                    System.out.println(); 
                    table.putBoolean("Boiler found!", true);
                    table.putNumber("distance_m", distance);
                    table.putNumber("azimuth_deg", azimuth);
					table.putNumber("altitude_deg", altitude);
                }
				if(foundBoiler)
				{

					distance = Double.parseDouble(parsed[2]);
                    azimuth = Double.parseDouble(parsed[3]);
					altitude = Double.parseDouble(parsed[4]);
                    System.out.println("Boiler found!");
                    System.out.println("Distance: " + distance);
                    System.out.println("Azimuth: " + azimuth); 
					System.out.println("Altitude: " + altitude); 
                    System.out.println(); 
                    table.putBoolean("toteFound", true);
                    table.putNumber("distance_m", distance);
                    table.putNumber("azimuth_deg", azimuth);
					table.putNumber("altitude_deg", altitude);

				}
                /*else
                {
                    //System.out.println("Tote not found");
                    table.putBoolean("toteFound", false);
                    System.out.println(); 
                }*/
            } 
            out.close();
            in.close();
            client.close();
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }
   }    
    
}
