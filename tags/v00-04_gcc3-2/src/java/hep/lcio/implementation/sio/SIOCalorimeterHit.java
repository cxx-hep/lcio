package hep.lcio.implementation.sio;

import hep.lcd.io.sio.SIOInputStream;
import hep.lcd.io.sio.SIOOutputStream;
import hep.lcd.io.sio.SIORef;

import hep.lcio.event.CalorimeterHit;
import hep.lcio.event.LCIO;
import hep.lcio.event.MCParticle;

import hep.lcio.implementation.event.ICalorimeterHit;

import java.io.IOException;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOCalorimeterHit.java,v 1.3 2003-05-09 15:16:45 gaede Exp $
 */
class SIOCalorimeterHit extends ICalorimeterHit
{
   SIOCalorimeterHit(SIOInputStream in, int flags) throws IOException
   {
      cellId0 = in.readInt();
      cellId1 = in.readInt();
      energy = in.readFloat();

      if ((flags & (1<<LCIO.CHBIT_LONG)) != 0)
	  {
	      position[0] = in.readFloat();
	      position[1] = in.readFloat();
	      position[2] = in.readFloat();

	  }
      nContributions = in.readInt();
      particle = new SIORef[nContributions];
      energyContrib = new float[nContributions];
      time = new float[nContributions];

      boolean hasPDG = (flags & (1<<LCIO.CHBIT_PDG)) != 0;
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
   }

   public MCParticle getParticleCont(int i)
   {
      if (particle[i] instanceof SIORef)
         particle[i] = ((SIORef) particle[i]).getObject();
      return (MCParticle) particle[i];
   }

   static void write(CalorimeterHit hit, SIOOutputStream out, int flags) throws IOException
   {
      if (hit instanceof SIOCalorimeterHit)
         ((SIOCalorimeterHit) hit).write(out, flags);
      else
      {
         out.writeInt(hit.getCellID0());
         out.writeInt(hit.getCellID1());
         out.writeFloat(hit.getEnergy());

	 if ((flags & (1<<LCIO.CHBIT_LONG) ) != 0)
	     {
		 float[] pos = hit.getPosition();
		 out.writeFloat(pos[0]);
		 out.writeFloat(pos[1]);
		 out.writeFloat(pos[2]);

	     }
	 boolean hasPDG = (flags & (1<<LCIO.CHBIT_PDG)) != 0;
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
      }
   }

   private void write(SIOOutputStream out, int flags) throws IOException
   {
      out.writeInt(cellId0);
      out.writeInt(cellId1);
      out.writeFloat(energy);

      if ((flags & (1<<LCIO.CHBIT_LONG)) != 0)
	  {
	      out.writeFloat(position[0]);
	      out.writeFloat(position[1]);
	      out.writeFloat(position[2]);
	  }

      boolean hasPDG = (flags & (1<<LCIO.CHBIT_PDG)) != 0;
      out.writeInt(nContributions);
      for (int i = 0; i < nContributions; i++)
      {
         out.writePntr(particle[i]);
         out.writeFloat(energyContrib[i]);
         out.writeFloat(time[i]);
         if (hasPDG)
            out.writeInt(pdg[i]);
      }
   }
}