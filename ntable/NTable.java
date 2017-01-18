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

            NetworkTable.setClientMode();
            NetworkTable.setIPAddress("70.95.180.209");
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
                /*if(targetDataRaw == null)
                    break;
                if(targetDataRaw.isEmpty())
                    continue;
				*/
                // Implements the FRC 2015 Network Tables Protocol
                boolean Gear, Boiler;
                double distance = 0, azimuth = 0, altitude = 0, bdistance = 0, bazimuth = 0, baltitude = 0;
                String[] parsed = targetDataRaw.split(";"); 
                Gear = parsed[0].equals("true");
				Boiler =  = parsed[1].equals("true");
                //System.out.println(parsed[0]); 
                if(Gear)
                {
                    distance = Double.parseDouble(parsed[2]);
                    azimuth = Double.parseDouble(parsed[3]);
					altitude = Double.parseDouble(parsed[4]);
                    System.out.println("Gear found!");
                    System.out.println("Distance: " + distance);
                    System.out.println("Azimuth: " + azimuth); 
					System.out.println("Altitude: " + altitude); 
                    System.out.println(); 
                    table.putBoolean("Gear", true);
                    table.putNumber("distance_m", distance);
                    table.putNumber("azimuth_deg", azimuth);
					table.putNumber("altitude_deg", altitude);
                }

				if(Boiler)
                {
                    distance = Double.parseDouble(parsed[5]);
                    azimuth = Double.parseDouble(parsed[6]);
					altitude = Double.parseDouble(parsed[7]);
                    System.out.println("Boiler found!");
                    System.out.println("Distance: " + bdistance);
                    System.out.println("Azimuth: " + bazimuth); 
					System.out.println("Altitude: " + baltitude); 
                    System.out.println(); 
                    table.putBoolean("Boiler", true);
                    table.putNumber("distance_m", bdistance);
                    table.putNumber("azimuth_deg", bazimuth);
					table.putNumber("altitude_deg", baltitude);
                }
                else
                {
                    //System.out.println("Tote not found");
                    table.putBoolean("Gear", false);
					table.putBoolean("Boiler", false);
                    System.out.println(); 
                }
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
