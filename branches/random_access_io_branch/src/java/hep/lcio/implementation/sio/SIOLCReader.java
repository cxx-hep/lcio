package hep.lcio.implementation.sio;

import hep.io.sio.SIOBlock;
import hep.io.sio.SIOReader;
import hep.io.sio.SIORecord;

import hep.lcio.event.LCEvent;
import hep.lcio.event.LCIO;
import hep.lcio.event.LCRunHeader;

import hep.lcio.io.LCEventListener;
import hep.lcio.io.LCReader;
import hep.lcio.io.LCRunListener;

import java.io.EOFException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;


/**
 *
 * @author Tony Johnson
 * @version $Id: SIOLCReader.java,v 1.16.10.1 2009-10-20 23:18:21 tonyj Exp $
 */
class SIOLCReader implements LCReader
{
   private List eventListeners = new ArrayList();
   private List runListeners = new ArrayList();
   private RandomAccessBlock fileRandomAccessBlock;
   private List<RandomAccessBlock> indexRandomAccessBlocks = new ArrayList<RandomAccessBlock>();
   private SIOReader reader;
   private boolean indexBlocksRead;

   private String[] _filenames ;
   private int _currentIndex ;

   public void close() throws IOException
   {
      reader.close();
   }

   public void open(String filename) throws IOException
   {
      reader = new SIOReader(filename);
   }

    /** Opens a list of files for reading (read-only). All subsequent
     * read operations will operate on the list, i.e. if an EOF is encountered
     * the next file in the list will be opened and read transparently to the
     * user.
     *
     * @throws IOException
     */
    public void open(String[] filenames) throws IOException{
	_filenames = filenames ;
	_currentIndex=0 ;
	open( _filenames[ _currentIndex ] );
    }


   public LCEvent readEvent(int runNumber, int evtNumber) throws IOException
   {
      try
      {
         for (;;)
         {            
            SIORecord record = reader.readRecord();
            String name = record.getRecordName();
            if (SIOFactory.eventHeaderRecordName.equals(name)) continue;

            SIOEvent event = new SIOEvent(record,LCIO.READ_ONLY);
            if (event.getRunNumber() == runNumber && event.getEventNumber() == evtNumber)
            {
               event.readData(reader.readRecord());
               return event;
            }
            else reader.readRecord();
         }
      }
      catch (EOFException x)
      {
	if( _filenames != null  && ++_currentIndex < _filenames.length ){
	    close() ;
	    open( _filenames[ _currentIndex ] ) ;
	    return readEvent( runNumber, evtNumber) ;
	} 
	return null;
      }
   }

   public LCEvent readNextEvent(int accessMode) throws IOException
   {
      try
      {
         for (;;)
         {
            SIORecord record = reader.readRecord();
            String name = record.getRecordName();
            if (name.equals("LCIORandomAccess")) addRandomAccessRecord(record);
            if (!name.equals(SIOFactory.eventHeaderRecordName))
               continue;

            SIOEvent event = new SIOEvent(record,accessMode);
            event.readData(reader.readRecord());
            return event;
         }
      }
      catch (EOFException x)
      {
	if( _filenames != null  && ++_currentIndex < _filenames.length ){
	    close() ;
	    open( _filenames[ _currentIndex ] ) ;
	    return readNextEvent( accessMode ) ;
	} 
         return null;
      }
   }

   public void skipNEvents(int n) 
   {
      int nEvents = 0 ;
	  try
	  {
	     while( nEvents < n )
	     {
		    SIORecord record = reader.readRecord();
		    String name = record.getRecordName();
		    if ( ! name.equals( SIOFactory.eventHeaderRecordName) )
			   continue;
             ++nEvents ;
	     }
	  }
	  catch (IOException x)
	  {
 	  }
   }
   public LCEvent readNextEvent() throws IOException
   {
      return readNextEvent(LCIO.READ_ONLY);
   }

   public LCRunHeader readNextRunHeader() throws IOException
    {
	return readNextRunHeader(LCIO.READ_ONLY);
    }

   public LCRunHeader readNextRunHeader(int accessMode) throws IOException
   {
      try
      {
         for (;;)
         {
            SIORecord record = reader.readRecord();
            String name = record.getRecordName();
            if (!name.equals(SIOFactory.runRecordName))
               continue;

            SIOBlock block = record.getBlock();
			int major = block.getMajorVersion() ;
			int minor = block.getMinorVersion() ;
			if (( major < 1) && ( minor < 8))
               throw new IOException("Sorry: files created with versions older than v00-08" + " are no longer supported !");

	    // FIX ME: need to set access mode here....
            return new SIORunHeader(block.getData(),major,minor);
         }
      }
      catch (EOFException x)
      {
	if( _filenames != null  && ++_currentIndex < _filenames.length ){
	    close() ;
	    open( _filenames[ _currentIndex ] ) ;
	    return readNextRunHeader( accessMode ) ;
	} 
         return null;
      }
   }

   public void readStream() throws IOException
   {
      readStream(-1);
   }

   public void readStream(int maxRecords) throws IOException
   {
      try
      {
         for (int nRead = 0; (maxRecords < 0) || (nRead < maxRecords);
               nRead++)
         {
            SIORecord record = reader.readRecord();
            if (record.getRecordName().equals(SIOFactory.runRecordName))
            {
               int l = runListeners.size();
               if (l > 0)
               {
                  SIOBlock block = record.getBlock();

     			  int major = block.getMajorVersion() ;
	    		  int minor = block.getMinorVersion() ;
		    	  if (( major < 1) && ( minor < 8))
                     throw new IOException("Sorry: files created with versions older than v00-08" + " are no longer supported !");

                  SIORunHeader header = new SIORunHeader(block.getData(),major,minor);
                  for (int i = 0; i < l; i++){
		      // FIX ME: need to set access mode here....
                     ((LCRunListener) runListeners.get(i)).processRunHeader(header);
                     ((LCRunListener) runListeners.get(i)).modifyRunHeader(header);
		          }
               }
            }
            else if (record.getRecordName().equals(SIOFactory.eventHeaderRecordName))
            {
               int l = eventListeners.size();
               if (l > 0)
               {
                  SIOEvent event = new SIOEvent(record,LCIO.READ_ONLY);
		  // fg20070813 changed order of update and process to be consistent with C++
		  // (needed for Marlin modifying processors )
		  event.setReadOnly(false);
                 event.readData(reader.readRecord());
                 for (int i = 0; i < l; i++){ ((LCEventListener) eventListeners.get(i)).modifyEvent(event); }
                 for (int i = 0; i < l; i++){ ((LCEventListener) eventListeners.get(i)).processEvent(event); }
               }
            }
         }
      }
      catch (EOFException x)
      {
	if( _filenames != null  && ++_currentIndex < _filenames.length ){
	    close() ;
	    open( _filenames[ _currentIndex ] ) ;
	    readStream( maxRecords ) ;
	} 

	if (maxRecords < 0)
            return;
	else
            throw x;
      }
   }
   public void registerLCEventListener(LCEventListener ls)
   {
      eventListeners.add(ls);
   }

   public void registerLCRunListener(LCRunListener ls)
   {
      runListeners.add(ls);
   }

   public void removeLCEventListener(LCEventListener ls)
   {
      eventListeners.remove(ls);
   }

   public void removeLCRunListener(LCRunListener ls)
   {
      runListeners.remove(ls);
   }

   private void addRandomAccessRecord(SIORecord record) throws IOException
   {
      RandomAccessBlock ra = new RandomAccessBlock(record);
      System.out.println("Found ra="+ra);
      if (ra.getIndexLocation() == 0) fileRandomAccessBlock = ra;
      else indexRandomAccessBlocks.add(ra);
   }

   private long findNextRunHeader() throws IOException {
      RandomAccessBlock fab = findFileRandomAccessBlock();
      for (RandomAccessBlock rab : findIndexRandomAccessBlocks()) {
          
      }
   }

   private long findEvent(int run, int event) throws IOException {
      if (!indexBlocksRead) readIndexBlocks();
      // FIXME: Assumes records are ordered
      RunEvent re = new RunEvent(run,event);
      if (!fileRandomAccessBlock.contains(re)) return -1;

      int location = Collections.binarySearch(indexRandomAccessBlocks,re);
      return 0;
   }

    private void readIndexBlocks() throws IOException {
       RandomAccessBlock fab = findFileRandomAccessBlock();

       if (indexRandomAccessBlocks.isEmpty()) {
           SIORecord record = reader.readRecord(fab.getPreviousLocation());
           RandomAccessBlock rab = new RandomAccessBlock(record);
           indexRandomAccessBlocks.add(rab);
           indexBlocksRead =  rab.getNextLocation() == 0;
       }
       while (!indexBlocksRead) {
           long nextLocation = indexRandomAccessBlocks.get(indexRandomAccessBlocks.size()-1).getNextLocation();
           SIORecord record = reader.readRecord(nextLocation);
           RandomAccessBlock rab = new RandomAccessBlock(record);
           indexRandomAccessBlocks.add(rab);
           indexBlocksRead =  rab.getNextLocation() == 0;
       }
    }

    private RandomAccessBlock findFileRandomAccessBlock() throws IOException {
        if (fileRandomAccessBlock == null) {
           SIORecord record = reader.readRecord(0);
           fileRandomAccessBlock = new RandomAccessBlock(record);
        }
        return fileRandomAccessBlock;
    }

    private Iterable<RandomAccessBlock> findIndexRandomAccessBlocks() {
        throw new UnsupportedOperationException("Not yet implemented");
    }
}
