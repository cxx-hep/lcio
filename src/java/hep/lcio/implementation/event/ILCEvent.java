package hep.lcio.implementation.event;

import hep.lcio.event.LCCollection;
import hep.lcio.event.LCEvent;

import java.util.LinkedHashMap;
import java.util.Map;
import hep.lcio.exceptions.DataNotAvailableException; 

/**
 * A default implementation of LCEvent
 * @author Tony Johnson
 * @version $Id: ILCEvent.java,v 1.8.2.1 2004-04-06 11:26:00 gaede Exp $
 */
public class ILCEvent extends ILCObject implements LCEvent
{
   protected String detectorName;
   protected int eventNumber;
   protected int runNumber;
   protected long timeStamp;
   private Map collections = new LinkedHashMap();

   public LCCollection getCollection(String name) throws DataNotAvailableException
   {
       LCCollection col = (LCCollection) collections.get(name);
       if( col == null )
	   throw new DataNotAvailableException() ;
       return col ; 
   }

   public LCCollection getCollectionData(String name)
   {
      return (LCCollection) collections.get(name);
   }

   public String[] getCollectionNames()
   {
      String[] keys = new String[collections.size()];
      collections.keySet().toArray(keys);
      return keys;
   }

   public void setDetectorName(String detectorName)
   {
      checkAccess();
      this.detectorName = detectorName;
   }

   public String getDetectorName()
   {
      return detectorName;
   }

   public void setEventNumber(int event)
   {
      checkAccess();
      this.eventNumber = event;
   }

   public int getEventNumber()
   {
      return eventNumber;
   }

   public void setRunNumber(int run)
   {
      checkAccess();
      this.runNumber = run;
   }

   public int getRunNumber()
   {
      return runNumber;
   }

   public void setTimeStamp(long timeStamp)
   {
      checkAccess();
      this.timeStamp = timeStamp;
   }

   public long getTimeStamp()
   {
      return timeStamp;
   }

   public void addCollection(LCCollection col, String name)
   {
      collections.put(name, col);
   }

   public void removeCollection(String name)
   {
      checkAccess();
      collections.remove(name);
   }
}
