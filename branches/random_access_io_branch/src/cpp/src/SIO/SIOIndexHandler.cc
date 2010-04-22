#include "SIO/SIOIndexHandler.h"

//#include "SIO/LCIORandomAccessMgr.h"
#include "SIO/LCSIO.h"
#include "SIO_functions.h"

#include "EVENT/LCIO.h"

#include <iostream>


namespace SIO  {


  SIOIndexHandler::SIOIndexHandler(const std::string& name, LCIORandomAccessMgr* raMgr) : 
    SIO_block( name.c_str() ),
    _raMgr( raMgr ) {
    
  }
 
  SIOIndexHandler::~SIOIndexHandler(){ }

  unsigned int SIOIndexHandler::xfer( SIO_stream* stream, SIO_operation op, 
					  unsigned int versionID){
  
    LCSIO::checkVersion(versionID) ;
    
    unsigned int status ; // needed by the SIO_DATA macro
  

    if( op == SIO_OP_READ ){ 

      int control ;
      int runMin ;
      long64 baseOffset ;
      int size ;

      SIO_DATA( stream ,  &control, 1  ) ;
     
      bool oneRun     = control & 0x0001 ;
      bool longOffset = control & 0x0002 ;

      //FIXME: do we need this ?
      if( control & 0x0004 ){  // parameters

	std::cerr << " WARNING: SIOIndexHandler: parameters not implemented .... " << std::endl ;

	return SIO_BLOCK_SKIP ; // FIXME: test
      }

      SIO_DATA( stream ,  &runMin, 1  ) ;
      SIO_DATA( stream ,  &baseOffset, 1  ) ;
      SIO_DATA( stream ,  &size, 1  ) ;


      int nEvents = 0;
      int nRunHeaders = 0;
      //      int maxEntries = size;

      int runNum ;
      int evtNum ;
      int runOffset ;
      long64 pos ;

      int dummy_int; 
      long64 dummy_long ;

      for (int i = 0; i < size; i++) {

	runNum = runMin ;

	if( !oneRun ) {
	  SIO_DATA( stream ,  &runOffset, 1  ) ;
	  runNum += runOffset ;
	}
	SIO_DATA( stream ,  &evtNum , 1  ) ;

	if (evtNum >= 0) 
	  nEvents++;
	else 
	  nRunHeaders++;

	if( longOffset ){

	  SIO_DATA( stream ,  &dummy_long , 1  ) ;
	  pos = dummy_long ;

	} else {

	  SIO_DATA( stream ,  &dummy_int , 1  ) ;
	  pos = dummy_int ;
	}

	_raMgr->_runEvtMap.add( RunEvent( runNum , evtNum ) ,  pos ) ;
      }

//       _raMgr->addLCIORandomAccess( ra ) ;
//       std::cout << " ... LCIORandomAccess read from stream : " << *ra << std::endl ;


    }  else if( op == SIO_OP_WRITE ){ 
    
      
//       if( 1 ){

// 	LCSIO_WRITE( stream ,   _ra->_minRunEvt.RunNum ) ;
// 	LCSIO_WRITE( stream ,   _ra->_minRunEvt.EvtNum ) ;
	
// 	LCSIO_WRITE( stream ,   _ra->_maxRunEvt.RunNum ) ;
// 	LCSIO_WRITE( stream ,   _ra->_maxRunEvt.EvtNum ) ;
	
// 	LCSIO_WRITE( stream ,   _ra->_nRunHeaders ) ;
// 	LCSIO_WRITE( stream ,   _ra->_nEvents ) ;
// 	LCSIO_WRITE( stream ,   _ra->_recordsAreInOrder ) ;
	
// 	LCSIO_WRITE( stream ,   _ra->_indexLocation ) ;
// 	LCSIO_WRITE( stream ,   _ra->_prevLocation ) ;
// 	LCSIO_WRITE( stream ,   _ra->_nextLocation ) ;
// 	LCSIO_WRITE( stream ,   _ra->_firstRecordLocation  ) ;
//       }
    }
    
    
    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
  unsigned int   SIOIndexHandler::version(){
    
    int version = SIO_VERSION_ENCODE( EVENT::LCIO::MAJORVERSION, EVENT::LCIO::MINORVERSION ) ;
    return version ;
    
  }
  
}
