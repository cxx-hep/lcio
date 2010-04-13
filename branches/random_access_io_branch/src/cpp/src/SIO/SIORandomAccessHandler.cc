#include "SIO/SIORandomAccessHandler.h"

#include "SIO/LCIORandomAccess.h"
#include "SIO/LCSIO.h"
#include "SIO_functions.h"

#include "EVENT/LCIO.h"

#include <iostream>


namespace SIO  {


  SIORandomAccessHandler::SIORandomAccessHandler(const std::string& name) : 
    SIO_block( name.c_str() ),
    _raP(0) {
  }

  SIORandomAccessHandler::SIORandomAccessHandler(const std::string& name, LCIORandomAccess** aRhP) : 
    SIO_block( name.c_str() ),
    _raP( aRhP ) {
    
    *_raP = 0 ;
  }


  SIORandomAccessHandler::~SIORandomAccessHandler(){ }

  
  void SIORandomAccessHandler::setWritePtr(const LCIORandomAccess* rh ){
    _ra = rh ;
  }
  void SIORandomAccessHandler::setReadPtr( LCIORandomAccess** hdrP ) {
    _raP= hdrP ;
  } 


  unsigned int SIORandomAccessHandler::xfer( SIO_stream* stream, SIO_operation op, 
					  unsigned int versionID){
  
    LCSIO::checkVersion(versionID) ;
    
    unsigned int status ; // needed by the SIO_DATA macro
  
    if( op == SIO_OP_READ ){ 

      if(!_raP) return EVENT::LCIO::ERROR ;  // in read mode we need an address for the pointer

      // delete the old object 
      // -> for every handler there will only be one object at any given time
      if (*_raP != 0 )  delete *_raP ;
      *_raP = new LCIORandomAccess ;

      SIO_DATA( stream ,  &( (*_raP)->_minRunEvt.RunNum )   , 1  ) ;
      SIO_DATA( stream ,  &( (*_raP)->_minRunEvt.EvtNum )   , 1  ) ;

      SIO_DATA( stream ,  &( (*_raP)->_maxRunEvt.RunNum )   , 1  ) ;
      SIO_DATA( stream ,  &( (*_raP)->_maxRunEvt.EvtNum )   , 1  ) ;

      SIO_DATA( stream ,  &( (*_raP)->_nRunHeaders )   , 1  ) ;
      SIO_DATA( stream ,  &( (*_raP)->_nEvents )   , 1  ) ;
      SIO_DATA( stream ,  &( (*_raP)->_recordsAreInOrder )   , 1  ) ;
      
      SIO_DATA( stream ,  &( (*_raP)->_indexLocation )   , 1  ) ;
      SIO_DATA( stream ,  &( (*_raP)->_prevLocation )   , 1  ) ;
      SIO_DATA( stream ,  &( (*_raP)->_nextLocation )   , 1  ) ;
      SIO_DATA( stream ,  &( (*_raP)->_firstRecordLocation )   , 1  ) ;

      
    }  else if( op == SIO_OP_WRITE ){ 
    
      if( _ra ){

	LCSIO_WRITE( stream ,   _ra->_minRunEvt.RunNum ) ;
	LCSIO_WRITE( stream ,   _ra->_minRunEvt.EvtNum ) ;
	
	LCSIO_WRITE( stream ,   _ra->_maxRunEvt.RunNum ) ;
	LCSIO_WRITE( stream ,   _ra->_maxRunEvt.EvtNum ) ;
	
	LCSIO_WRITE( stream ,   _ra->_nRunHeaders ) ;
	LCSIO_WRITE( stream ,   _ra->_nEvents ) ;
	LCSIO_WRITE( stream ,   _ra->_recordsAreInOrder ) ;
	
	LCSIO_WRITE( stream ,   _ra->_indexLocation ) ;
	LCSIO_WRITE( stream ,   _ra->_prevLocation ) ;
	LCSIO_WRITE( stream ,   _ra->_nextLocation ) ;
	LCSIO_WRITE( stream ,   _ra->_firstRecordLocation  ) ;
      }
    }
    
    
    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
  unsigned int   SIORandomAccessHandler::version(){
    
    int version = SIO_VERSION_ENCODE( EVENT::LCIO::MAJORVERSION, EVENT::LCIO::MINORVERSION ) ;
    return version ;
    
  }
  
}
