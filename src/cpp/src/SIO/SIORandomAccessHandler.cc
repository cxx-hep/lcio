#include "SIO/SIORandomAccessHandler.h"

#include "SIO/LCIORandomAccess.h"
#include "SIO/LCSIO.h"
#include "SIO_functions.h"

#include "EVENT/LCIO.h"

#include <iostream>


namespace SIO  {


  SIORandomAccessHandler::SIORandomAccessHandler(const std::string& name, LCIORandomAccessMgr* raMgr) : 
    SIO_block( name.c_str() ),
    _raMgr( raMgr ) {
    
  }
 

  SIORandomAccessHandler::~SIORandomAccessHandler(){ }

  
  unsigned int SIORandomAccessHandler::xfer( SIO_stream* stream, SIO_operation op, 
					  unsigned int versionID){
  
    LCSIO::checkVersion(versionID) ;
    
    unsigned int status ; // needed by the SIO_DATA macro
  

    if( op == SIO_OP_READ ){ 

      LCIORandomAccess* ra = new LCIORandomAccess ;

      SIO_DATA( stream ,  &(ra->_minRunEvt.RunNum) , 1  ) ;
      SIO_DATA( stream ,  &(ra->_minRunEvt.EvtNum) , 1  ) ;
      
      SIO_DATA( stream ,  &( ra->_maxRunEvt.RunNum) , 1  ) ;
      SIO_DATA( stream ,  &( ra->_maxRunEvt.EvtNum) , 1  ) ;
      
      SIO_DATA( stream ,  &( ra->_nRunHeaders) , 1  ) ;
      SIO_DATA( stream ,  &( ra->_nEvents) , 1  ) ;
      SIO_DATA( stream ,  &( ra->_recordsAreInOrder) , 1  ) ;
      
      SIO_DATA( stream ,  &( ra->_indexLocation) , 1  ) ;
      SIO_DATA( stream ,  &( ra->_prevLocation) , 1  ) ;
      SIO_DATA( stream ,  &( ra->_nextLocation) , 1  ) ;
      SIO_DATA( stream ,  &( ra->_firstRecordLocation) , 1  ) ;
      
      _raMgr->addLCIORandomAccess( ra ) ;

      //std::cout << " ... LCIORandomAccess read from stream : " << *ra << std::endl ;


    }  else if( op == SIO_OP_WRITE ){ 
    
      LCIORandomAccess* ra = new LCIORandomAccess ;  // FIXME: get from EventMap

      SIO_DATA( stream ,  &(ra->_minRunEvt.RunNum) , 1  ) ;
      SIO_DATA( stream ,  &(ra->_minRunEvt.EvtNum) , 1  ) ;
      
      SIO_DATA( stream ,  &( ra->_maxRunEvt.RunNum) , 1  ) ;
      SIO_DATA( stream ,  &( ra->_maxRunEvt.EvtNum) , 1  ) ;
      
      SIO_DATA( stream ,  &( ra->_nRunHeaders) , 1  ) ;
      SIO_DATA( stream ,  &( ra->_nEvents) , 1  ) ;
      SIO_DATA( stream ,  &( ra->_recordsAreInOrder) , 1  ) ;
      
      SIO_DATA( stream ,  &( ra->_indexLocation) , 1  ) ;
      SIO_DATA( stream ,  &( ra->_prevLocation) , 1  ) ;
      SIO_DATA( stream ,  &( ra->_nextLocation) , 1  ) ;
      SIO_DATA( stream ,  &( ra->_firstRecordLocation) , 1  ) ;
      
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
  
  unsigned int   SIORandomAccessHandler::version(){
    
    int version = SIO_VERSION_ENCODE( EVENT::LCIO::MAJORVERSION, EVENT::LCIO::MINORVERSION ) ;
    return version ;
    
  }
  
}
