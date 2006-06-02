package hep.lcio.example;

import hep.lcio.event.*;

import hep.lcio.implementation.event.*;
import hep.lcio.implementation.io.LCFactory;

import hep.lcio.io.*;

import java.io.IOException;
import java.util.Random;


/**
 *
 * @author Tony Johnson
 * @version $Id: SimJob.java,v 1.11 2003-09-15 21:49:52 tonyj Exp $
 */
public class SimJob
{
   private final static int NRUN = 10;
   private final static int NEVENT = 10;
   private final static int NMCPART = 10; // mc particles per event
   private final static int NHITS = 50; // calorimeter hits per event
   private final static String detName = "D09TileHcal";
   private final static String ecalName = "ECAL007";
   private final static String tpcName = "TPC4711";

   /**
    * @param args the command line arguments
    */
    public static void main(String[] args) throws IOException
   {
      if (args.length == 0)
         help();

      // create sio writer
      Random random = new Random();
      LCWriter lcWrt = LCFactory.getInstance().createLCWriter();

      lcWrt.open(args[0]);

      // loop over runs
      for (int rn = 0; rn < NRUN; rn++)
      {
         ILCRunHeader runHdr = new ILCRunHeader();
         runHdr.setRunNumber(rn);
         runHdr.setDetectorName(detName);
         runHdr.setDescription(" these are just dummy runs for testing lcio - no physics whatsoever !");
         runHdr.addActiveSubdetector(ecalName);
         runHdr.addActiveSubdetector(tpcName);

         lcWrt.writeRunHeader(runHdr);

         // EventLoop - create some events and write them to the file
         for (int i = 0; i < NEVENT; i++)
         {
            // we need to use the implementation classes here
            ILCEvent evt = new ILCEvent();

            evt.setRunNumber(rn);
            evt.setEventNumber(i);
            evt.setDetectorName("D09TileHcal");

            // create and add some mc particles
            ILCCollection mcVec = new ILCCollection(LCIO.MCPARTICLE);

            IMCParticle mom = null;
            for (int j = 0; j < NMCPART; j++)
            {
               IMCParticle mcp = new IMCParticle();

               mcp.setPDG(101 + (j * 100));
               if (mom != null)
               {
                  mcp.setParent(mom);

                  MCParticle[] daughters = new MCParticle[1];
                  daughters[0] = mcp;
                  mom.setDaughters(daughters);
               }

               float[] p = { 2.f / 1024.f, 4.f / 1024.f, 8.f / 1024.f };
               mcp.setMomentum(p);
               mcp.setMass((float) 3.01);

               mom = mcp; // one body decays :-)
               mcVec.add(mcp);
            }

            // now add some calorimeter hits
            ILCCollection calVec = new ILCCollection(LCIO.SIMCALORIMETERHIT);

            // set flag for long format - including position
            int flag = 1 << LCIO.CHBIT_LONG;
            flag = flag | (1 << LCIO.CHBIT_PDG); // include pdg as well
            calVec.setFlag(flag);

            for (int j = 0; j < NHITS; j++)
            {
               ISimCalorimeterHit hit = new ISimCalorimeterHit();
               hit.setEnergy(3.1415f * random.nextFloat());

               float[] pos = 
               {
                  1.1f * random.nextFloat(), 2.2f * random.nextFloat(),
                  3.3f * random.nextFloat()
               };

               hit.setPosition(pos);

               calVec.add(hit);

               // assign the hits randomly to MC particles
               int mcIndx = random.nextInt(NMCPART);

               // in order to access a MCParticle,  we need a dynamic cast as the
               // LCCollection returns an LCIOObject - this is like vectors in Java
               hit.addMCParticleContribution((MCParticle) mcVec.getElementAt(mcIndx), 0.314159f, 0.1155f, 121212);
            }

            // and finally some tracker hits
            // with some user extensions (4 floats) per track:
            // we just create a parallel collection of float vectors
            ILCCollection trkVec = new ILCCollection(LCIO.SIMTRACKERHIT);
            ILCCollection extFVec = new ILCCollection(LCIO.LCFLOATVEC);
            ILCCollection extIVec = new ILCCollection(LCIO.LCINTVEC);

            for (int j = 0; j < NHITS; j++)
            {
               ISimTrackerHit hit = new ISimTrackerHit();
               ILCFloatVec   extF = new ILCFloatVec();
               ILCIntVec     extI = new ILCIntVec();

               hit.setdEdx(30e-9f);

               double[] pos = 
               {
                  1.1 * random.nextDouble(), 2.2 * random.nextDouble(),
                  3.3 * random.nextDouble()
               };
               hit.setPosition(pos);

               // assign the hits randomly to MC particles
               int mcIndx = random.nextInt(NMCPART);

               hit.setMCParticle((MCParticle) mcVec.getElementAt(mcIndx));

               // fill the extension vectors (4 floats, 2 ints)
               extF.add(3.14159f);
               for (int k = 0; k < 3; k++)
                  extF.add((float) pos[k] * 0.1f);
               
               extI.add( 123456789 );
               extI.add( mcIndx );
               
               // add the hit and the extensions to their corresponding collections
               trkVec.add(hit);
               extFVec.add(extF);
               extIVec.add(extI);
            }

            // add all collection to the event
             
            evt.addCollection(mcVec, "MCParticle");
            evt.addCollection(calVec, ecalName);
            evt.addCollection(trkVec, tpcName);
            evt.addCollection(extFVec, tpcName + "UserFloatExtension");
            evt.addCollection(extIVec, tpcName + "UserIntExtension");


    // test: add a collection for one event only:
    if( rn == NRUN-1 && i == 0 ) { // first event o last run
      ILCCollection addExtVec = new ILCCollection( LCIO.LCFLOATVEC )  ;
      ILCFloatVec addExt = new ILCFloatVec() ;
      addExt.add( 1.f );
      addExt.add( 2.f );
      addExt.add( 3.f );
      addExt.add( 4.f );
      addExtVec.add( addExt ) ;
      evt.addCollection( addExtVec , "AdditionalExtension" ) ;
    }
    
//cng    
    // even though this is a simjob we can store 'real data' objects :)
    // --- for example we can store TPC hits ------------

    ILCCollection TPCVec = new ILCCollection( LCIO.TPCHIT )  ;

    boolean storeRawData = true ;

    int tpcFlag = 0 ;
     if(  storeRawData )  // if we want to store the raw data we need to set the flag
      tpcFlag = 1 << LCIO.TPCBIT_RAW ;
    TPCVec.setFlag( tpcFlag  ) ;
    
    for(int j=0;j<NHITS;j++){
      
      ITPCHit tpcHit = new ITPCHit();
      
      tpcHit.setCellID( j ) ;
      tpcHit.setTime( 0.1234567f ) ;
      tpcHit.setCharge( 3.14159f ) ;
      tpcHit.setQuality(  0xbad ) ;

      if(  storeRawData ) {
        // generate a random number of datawords less than 10
        int size = random.nextInt(10);
        int[] rawData = new int[size] ;
        // fill some random numbers into the array;   
        for(int k=0;k<size;k++){
          rawData[k] = random.nextInt() ;   
        }
        // set the raw data
        tpcHit.setRawDataWords( rawData ) ;
      }

      TPCVec.add( tpcHit ) ;
    }   
    evt.addCollection( TPCVec , "TPCRawFADC" ) ;
    
    //--------------  all for TPC --------------------    
//cng
            // write the event to the file
            lcWrt.writeEvent(evt);

            // dump the event to the screen
            LCTools.dumpEvent(evt);
         }

         // evt loop
      }

      // run loop
      System.out.println();
      System.out.println(" created  " + NRUN + " runs with  " + (NRUN * NEVENT) + " events");
      lcWrt.close();
   }

   private static void help()
   {
      System.out.println("java " + SimJob.class.getName() + " <output-file>");
      System.exit(1);
   }
}