package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;
import hep.lcd.io.sio.SIORef;

import hep.lcio.event.MCParticle;
import hep.lcio.event.SimTrackerHit;

import hep.lcio.implementation.event.ISimTrackerHit;

import java.io.IOException;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOSimTrackerHit.java,v 1.1 2003-06-06 13:05:57 gaede Exp $
 */
public class SIOSimTrackerHit extends ISimTrackerHit
{
   SIOSimTrackerHit(SIOInputStream in) throws IOException
   {
      cellID = in.readInt();
      position[0] = in.readDouble();
      position[1] = in.readDouble();
      position[2] = in.readDouble();
      dEdx = in.readFloat();
      time = in.readFloat();
      particle = in.readPntr();
   }

   public MCParticle getMCParticle()
   {
      if (particle instanceof SIORef)
         particle = ((SIORef) particle).getObject();
      return (MCParticle) particle;
   }

   static void write(SimTrackerHit hit, SIOOutputStream out) throws IOException
   {
      if (hit instanceof SIOSimTrackerHit)
         ((SIOSimTrackerHit) hit).write(out);
      else
      {
         out.writeInt(hit.getCellID());

         double[] pos = hit.getPosition();
         out.writeDouble(pos[0]);
         out.writeDouble(pos[1]);
         out.writeDouble(pos[2]);
         out.writeFloat(hit.getdEdx());
         out.writeFloat(hit.getTime());
         out.writePntr(hit.getMCParticle());
      }
   }

   private void write(SIOOutputStream out) throws IOException
   {
      out.writeInt(cellID);
      out.writeDouble(position[0]);
      out.writeDouble(position[1]);
      out.writeDouble(position[2]);
      out.writeFloat(dEdx);
      out.writeFloat(time);
      out.writePntr(particle);
   }
}
