#include "lcio.h"

#include "IO/LCReader.h"
#include "IMPL/LCTOOLS.h"
#include "EVENT/LCRunHeader.h" 
//#include "CPPFORT/lcioc2f.h"

static const char* FILEN = "recjob.slcio" ; // default file name 


using namespace std ;
using namespace lcio ;

/** Example for an analysis job. No concrete implementations are needed
 *  for reading the data - just the EVENT interfaces. 
 *  See LCTOOLS::dumpEvent(const LCEvent* evt) ) for details.  
 */

int main(int argc, char** argv ){

  // read file name from command line (only argument) 
  if( argc > 1 ) {
    FILEN = argv[1] ;
  }
  
  
  LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;

  try{
    
    lcReader->open( FILEN ) ;
    
    cout << " opened " << FILEN << " for reading " << endl ; 
    
    LCRunHeader *runHdr ;
    
    // loop over all run headers
    
    try{  
      
      while(1) {     // exit through (EndOfData)Exception ?
	
   //   old version without EndOfDataException:
   // while( ( runHdr = lcReader->readNextRunHeader() ) != 0 ){

	runHdr = lcReader->readNextRunHeader() ;
	
	cout << "  Run : " << runHdr->getRunNumber() 
	     << " - "      << runHdr->getDetectorName() 
	     << ":  "      << runHdr->getDescription()  << endl ;
      }

    }      
    catch( EndOfDataException ){
    } // do we realy want to handle EOF as an exception ?
    
    cout << endl ;
    

    lcReader->close() ;
  }
  catch(IOException& e){
    cout << " io error : " << e.what() << endl ;
  }
  
  // now loop over the file again and dump event data
  try {
    
    lcReader->open( FILEN ) ;
    
    
    cout << " reopened " << FILEN << " for reading " << endl ; 
    
    LCEvent* evt ;
    
    // dump all events in the file
    int nEvents = 0 ;
    
    try{
      while(1){
      
	evt = lcReader->readNextEvent() ;
	
	// the following code will fail at runtime - event is read only !
	// if we use " (const LCEvent*) evt " it won't even compile 
	// so we have a twofold protection against data corruption
	// int status = evt->addCollection( new LCCollectionVec(LCIO::SIMCALORIMETERHIT ),"NewCol" ) ; 
	//  if( status != LCIO::SUCCESS ){
	//    cout << " error - couldn't add new collection to the event ! " << endl ;
	//    exit(1) ;
	//  }
	
	LCTOOLS::dumpEvent( evt ) ;
      
	nEvents ++ ;
      
      } // end while
    }      
    catch( EndOfDataException){
    }
    
    cout << endl <<  "  " <<  nEvents << " events read from file : " << FILEN << endl << endl ;
    
    
    lcReader->close() ;
    
  }  
  catch(IOException& e){
    cout << " io error : " << e.what() << endl ;
  }
  catch(Exception& e){
    cout << " exception caught : " << e.what() << endl ;
  }
  
  
  return 0 ;
}
  
  
