#include "lcio.h"

#include "IO/LCWriter.h"
#include "IO/LCReader.h"
#include "IO/LCEventListener.h"
#include "IO/LCRunListener.h"

#include "EVENT/LCIO.h"

#include "IMPL/LCEventImpl.h" 
#include "IMPL/LCCollectionVec.h"
#include "IMPL/SimCalorimeterHitImpl.h"
#include "IMPL/MCParticleImpl.h" 
#include "IMPL/LCFlagImpl.h" 
#include "IMPL/LCTOOLS.h"


#include <cstdlib>
#include <iostream>

using namespace std ;
using namespace lcio ;


static char* FILEN = "simjob.slcio" ;
static char* OUTFILEN = "recjob.slcio" ;
static const int NHITS = 50 ;  // calorimeter hits per event


/** Example of reading events from the file, add sth. to the event
 * and write it to a new file. This uses the listener mechanism to read 
 * the input file (run headers and events) record by record. <br>
 * 
 *  The RunEventProcessor class is defined for processing run and event records.
 *  This is our analysis module.
 *  For simplicity it is defined in the same file - in a real world application 
 *  it should of course be defined in sepparate header and source files.
 *  
 */

class RunEventProcessor : public LCRunListener, public LCEventListener{
  
protected:
  LCWriter* lcWrt ;
  int nEvent ;
  
public:
  
  RunEventProcessor() : nEvent(0) {
    
    // open outputfile
    lcWrt = LCFactory::getInstance()->createLCWriter() ;

    try{ lcWrt->open( OUTFILEN , LCIO::WRITE_NEW ) ; } 
    
    catch(IOException& e){
      cout << "[RunEventProcessor()] Can't open file for writing -  " 
	   << e.what()  << endl ;
      exit(1) ;
    }
    
  }
  
  ~RunEventProcessor(){

    // close outputfile
    lcWrt->close()  ;

    cout << endl 
	 << " added collection HCALReco with " << NHITS << " hits" 
	 << " to   " << nEvent <<" events"  
	 << " and added one extra MCParticle to each event."
	 << endl << endl ;
  }
  
  void processEvent( LCEvent * evt ) { /* used for 'read only' access*/ 

    // this is our event loop code
    
    // read collection with MCParticles
    LCCollection* mcVec = evt->getCollection( LCIO::MCPARTICLE )  ;
    
    int NMCPART = mcVec->getNumberOfElements() ;
    
    // ---- trying to modify objects here would cause a ReadOnlyExcpetion. e.g. -----
//         for(int i=0 ; i< NMCPART ; i++ ){
//           MCParticleImpl* part =  dynamic_cast<MCParticleImpl*>( mcVec->getElementAt( i )) ;
//           part->setPDG(1234) ;      // <<<<< ------- will cause ReadOnlyException ---------
//         }
    // ---- also  adding  sth. to en existing collection is not allowed here ----
    //     MCParticleImpl* part = new MCParticleImpl ;
    //     part->setPDG( 1234 ) ;
    //     part->setParent( dynamic_cast<MCParticle*>( mcVec->getElementAt(0) )) ;
    //     mcVec->addElement( part ) ;  // <<<<< ------- will cause ReadOnlyException ---------
    

    // create a new collection to be added to the event
    // for simplicity just add some calorimeter hits (don't have cluster class yet) 
    
    LCCollectionVec* calVec = new LCCollectionVec( LCIO::SIMCALORIMETERHIT )  ;

    // set flag for short format (not including position )
    // and no PDG     
    LCFlagImpl chFlag ;
    chFlag.unsetBit( LCIO::CHBIT_LONG ) ;  // not really necessary - just for demonstration
    chFlag.unsetBit( LCIO::CHBIT_PDG ) ;
    calVec->setFlag( chFlag.getFlag()  ) ;

    
    for(int j=0;j<NHITS;j++){
      SimCalorimeterHitImpl* hit = new SimCalorimeterHitImpl ;
      hit->setEnergy( 3.1415 * rand()/RAND_MAX   )  ;
      float pos[3] = { 1.1* rand()/RAND_MAX , 2.2* rand()/RAND_MAX , 3.3* rand()/RAND_MAX } ;
      hit->setPosition( pos ) ;
      calVec->push_back( hit ) ;
      
      // assign the hits randomly to MC particles
      float rn =  1.*rand()/RAND_MAX ;
      int mcIndx = static_cast<int>( NMCPART * rn ) ;
      hit->addMCParticleContribution(  dynamic_cast<MCParticle*>(mcVec->getElementAt( mcIndx )) , 
				       0.2876 , 0.007 , 565656 ) ;
      
    }
    

    try{

      evt->addCollection( (LCCollection*) calVec , "HCALReco" ) ;

    }catch(EventException& e){
      cout << "exception: HCALReco not added: " << e.what() << endl ;
      // no need to exit
    }


    
    nEvent ++ ;

  }

  void modifyEvent( LCEvent * evt ) {

    // here we can modify existing objects that have been read from a stream:
    LCCollection* mcVec = evt->getCollection( LCIO::MCPARTICLE )  ;
    int NMCPART = mcVec->getNumberOfElements() ;
    for(int i=0 ; i< NMCPART ; i++ ){
      // in order to have access to the set-methods we need to cast to the implementation
      // of MCParticle 
      MCParticleImpl* part =  dynamic_cast<MCParticleImpl*>( mcVec->getElementAt(i)) ;
      part->setPDG(1234) ;   // <<<<< modifying persistent data
    }
    // or we could add sth. to existing collections
    MCParticleImpl* part = new MCParticleImpl ;
    part->setPDG( 1234 ) ;
    part->addParent( dynamic_cast<MCParticle*>( mcVec->getElementAt(0) )) ;
    mcVec->addElement( part ) ;  // <<<< adding to collections

    LCTOOLS::dumpEvent( evt ) ;
    
    lcWrt->writeEvent( evt ) ;
   }
  

  void processRunHeader( LCRunHeader* run){

    // just copy run headers to the outputfile
    lcWrt->writeRunHeader( run ) ;
  }

  void modifyRunHeader(LCRunHeader* run){/*  we don't modify anything */;}


} ;

//=============================================================================

int main(int argc, char** argv ){
  
    srand(1234) ;
    
    // create reader and writer for input and output streams 
    LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;
    
    
    // read file name from command line 
    if( argc > 1 ) { FILEN = argv[1] ; }
    if( argc > 2 ) { OUTFILEN = argv[2] ; }

    lcReader->open( FILEN ) ;
    // we could catch the exception here - but this not really needed
    // as long as we exit anyhow if the file could not be opened...
    //     try{  lcReader->open( FILEN ) ; } 
    //     catch( IOException& e){
    //       cout << "Can't open file : " << e.what()  << endl ;
    //       exit(1) ;
    //     }
    
    // create a new RunEventProcessor, register it with the reader
    // and read and proccess the whole stream 
    {
      RunEventProcessor evtProc ;
      
      lcReader->registerLCRunListener( &evtProc ) ; 
      lcReader->registerLCEventListener( &evtProc ) ; 
      
      lcReader->readStream() ;
      // here we could in principle also catch the exceptions but for a small program there isn't really
      // any use as we need to exit anyhow if the readStream() had a problem
      // in a larger framework/application one might not want to abort the program but instead read the next 
      // file from a list or so ...
      //       try{ 
      // 	lcReader->readStream() ;
      //       }
      //       catch(IOException &e){
      // 	    cout<< " io error: " <<  e.what() << endl ;
      //          // do sth. reasonable here to carry on with your application
      //       }
      //       catch(exception &e){
      // 	    cout<< "    error: " <<  e.what() << endl ;
      //          // do sth. reasonable here to carry on with your application
      //       }
      
    } 
    
    lcReader->close() ;

    return 0 ;
}

//=============================================================================

