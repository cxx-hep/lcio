#include "SIO/SIOEventHandler.h"
#include "SIO/LCSIO.h"

#include "EVENT/LCEvent.h"
#include "EVENT/LCIO.h"
#include "IOIMPL/LCEventIOImpl.h"
#include "IOIMPL/LCCollectionIOVec.h"

#include "SIO_functions.h"

using namespace EVENT ;
using namespace IOIMPL ;

namespace SIO  {


  SIOEventHandler::SIOEventHandler(const std::string& name) : 
    SIO_block( name.c_str() ),
    _evtP(0) {
  }

  SIOEventHandler::SIOEventHandler(const std::string& name, LCEventIOImpl** anEvtP) : 
    SIO_block( name.c_str() ),
    _evtP( anEvtP ) {
 
    *_evtP = 0 ;
 }


  SIOEventHandler::~SIOEventHandler(){
  }

  
  void SIOEventHandler::setEvent(const LCEvent* evt ){
    _evt = evt ;
  }


  unsigned int SIOEventHandler::xfer( SIO_stream* stream, SIO_operation op, 
				      unsigned int versionID){
  
    unsigned int status ; // needed by the SIO_DATA macro
  

    if( op == SIO_OP_READ ){ 

//      if(!_evtP) return LCIO::ERROR ;  // in read mode we need an address for the pointer

      // delete the old event object 
      // -> for every handler there will only be one event object at any given time      
       if (*_evtP )  delete *_evtP ;
       *_evtP = new LCEventIOImpl ;
      
      SIO_DATA( stream ,  &(*_evtP)->_runNumber  , 1  ) ;
      SIO_DATA( stream ,  &(*_evtP)->_eventNumber  , 1  ) ;
      SIO_DATA( stream ,  &(*_evtP)->_timeStamp  , 1  ) ;
    
      char* detName ; 
      LCSIO_READ( stream,  &detName ) ; 
      (*_evtP)->setDetectorName( detName  )  ;

      // read collection types and names
      // not needed for the event record 
      // but SIO crashes if block is not read completely ...
      int nCol ;
      SIO_DATA( stream ,  &nCol , 1 ) ;
      for( int i=0; i<nCol ; i++ ){
	char* dummy ; 
	LCSIO_READ( stream,  &dummy ) ; 
	std::string colName( dummy ) ;
	LCSIO_READ( stream,  &dummy ) ; 
	// don't attach a new collection to the event
	// will be done by collection handlers
	
	// if we are reading the header, we attach a new collection to the event
	// if we are reading the event this is done by the collection handler
	
	if( ! strcmp( getName()->c_str(), LCSIO::HEADERBLOCKNAME)  ){
	  (*_evtP)->addCollection( new LCCollectionIOVec( dummy ) , colName) ;
	}


      }


    }  else if( op == SIO_OP_WRITE ){ 
    
      if( _evt ){
	LCSIO_WRITE( stream, _evt->getRunNumber() ) ; 
	LCSIO_WRITE( stream, _evt->getEventNumber() ) ;
	LCSIO_WRITE( stream, _evt->getTimeStamp()  ) ;
	LCSIO_WRITE( stream, _evt->getDetectorName() ) ;
      
	// now write a list of colection types and names
      
	const StringVec* strVec = _evt->getCollectionNames() ;
	int nCol = strVec->size() ;
	SIO_DATA( stream, &nCol, 1 ) ;
      
	for( StringVec::const_iterator name = strVec->begin() ; name != strVec->end() ; name++){
	
	  const LCCollection* col = _evt->getCollection( *name ) ;
	  LCSIO_WRITE( stream, *name ) ;
	  LCSIO_WRITE( stream, col->getTypeName() ) ;
		
	} 

      } else {
	return 0 ;
      }
    }
  
  
    return ( SIO_BLOCK_SUCCESS ) ;
  }

  unsigned int   SIOEventHandler::version(){

    int version = SIO_VERSION_ENCODE( LCIO::MAJORVERSION, LCIO::MINORVERSION ) ;
    return version ;

  }

};
