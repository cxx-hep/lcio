package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;
import hep.lcd.io.sio.SIORef;

import hep.lcio.event.LCIO;
import hep.lcio.event.MCParticle;
import hep.lcio.event.SimCalorimeterHit;

import hep.lcio.implementation.event.ISimCalorimeterHit;

import java.io.IOException;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOSimCalorimeterHit.java,v 1.12 2004-04-15 14:52:55 gaede Exp $
 */
class SIOSimCalorimeterHit extends ISimCalorimeterHit
{
   SIOSimCalorimeterHit(SIOInputStream in, int flags, SIOEvent owner, int major, int minor) throws IOException
   {
      setParent(owner);
      cellId0 = in.readInt();


      if ((flags & (1 << LCIO.CHBIT_ID1)) != 0 || ( major == 0 && minor == 8) )  cellId1 = in.readInt();
      else cellId1 = 0;
      energy = in.readFloat();

      if ((flags & (1 << LCIO.CHBIT_LONG)) != 0)
      {
         position[0] = in.readFloat();
         position[1] = in.readFloat();
         position[2] = in.readFloat();
      }
      nContributions = in.readInt();
      particle = new Object[nContributions];
      energyContrib = new float[nContributions];
      time = new float[nContributions];

      boolean hasPDG = (flags & (1 << LCIO.CHBIT_PDG)) != 0;
      if (hasPDG)
         pdg = new int[nContributions];
      for (int i = 0; i < nContributions; i++)
      {
         particle[i] = in.readPntr();
         energyContrib[i] = in.readFloat();
         time[i] = in.readFloat();
         if (hasPDG)
            pdg[i] = in.readInt();
      }
	  double version  = (double) major + ( (double) minor ) /  10. ;
	  if( version > 1.0 )
	    in.readPTag(this);
   }

   public MCParticle getParticleCont(int i)
   {
      if (particle[i] instanceof SIORef)
         particle[i] = ((SIORef) particle[i]).getObject();
      return (MCParticle) particle[i];
   }

   static void write(SimCalorimeterHit hit, SIOOutputStream out, int flags) throws IOException
   {
      if (hit instanceof SIOSimCalorimeterHit)
         ((SIOSimCalorimeterHit) hit).write(out, flags);
      else
      {
         out.writeInt(hit.getCellID0());
         if ((flags & (1 << LCIO.CHBIT_ID1)) != 0) out.writeInt(hit.getCellID1());
         out.writeFloat(hit.getEnergy());

         if ((flags & (1 << LCIO.CHBIT_LONG)) != 0)
         {
            float[] pos = hit.getPosition();
            out.writeFloat(pos[0]);
            out.writeFloat(pos[1]);
            out.writeFloat(pos[2]);
         }

         boolean hasPDG = (flags & (1 << LCIO.CHBIT_PDG)) != 0;
         int n = hit.getNMCParticles();
         out.writeInt(n);
         for (int i = 0; i < n; i++)
         {
            out.writePntr(hit.getParticleCont(i));
            out.writeFloat(hit.getEnergyCont(i));
            out.writeFloat(hit.getTimeCont(i));
            if (hasPDG)
               out.writeInt(hit.getPDGCont(i));
         }
		out.writePTag(hit);
      }
   }

   private void write(SIOOutputStream out, int flags) throws IOException
   {
      out.writeInt(cellId0);
      if ((flags & (1 << LCIO.CHBIT_ID1)) != 0) out.writeInt(cellId1);
      out.writeFloat(energy);

      if ((flags & (1 << LCIO.CHBIT_LONG)) != 0)
      {
         out.writeFloat(position[0]);
         out.writeFloat(position[1]);
         out.writeFloat(position[2]);
      }

      boolean hasPDG = (flags & (1 << LCIO.CHBIT_PDG)) != 0;
      out.writeInt(nContributions);
      for (int i = 0; i < nContributions; i++)
      {
         out.writePntr(getParticleCont(i));
         out.writeFloat(energyContrib[i]);
         out.writeFloat(time[i]);
         if (hasPDG)
            out.writeInt(pdg[i]);
      }
  	  out.writePTag(this);
   }
}
