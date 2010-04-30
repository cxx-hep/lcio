#include "SIO/LCIORandomAccessMgr.h"

#include "SIO/LCSIO.h"
#include "SIO/SIORandomAccessHandler.h"
#include "SIO/SIOIndexHandler.h"

#include "SIO_stream.h"
#include "SIO_blockManager.h"

#include "Exceptions.h"
#include <sstream>

using namespace IO ;

namespace SIO{


  LCIORandomAccessMgr::LCIORandomAccessMgr() {
  }

  LCIORandomAccessMgr::~LCIORandomAccessMgr() {

    // cleanup 
    for( std::list<LCIORandomAccess* >::iterator i = _list.begin() ; i != _list.end() ; ++i ){

      delete *i ; 
    }

  }


  LCIORandomAccess* LCIORandomAccessMgr::createFromEventMap() {

    LCIORandomAccess* ra = new LCIORandomAccess  ;
    
    ra->_minRunEvt =   _runEvtMap.minRunEvent() ;
    ra->_maxRunEvt =   _runEvtMap.maxRunEvent() ;
    ra->_nRunHeaders = _runEvtMap.getNumberOfEventRecords() ;
    ra->_nEvents =     _runEvtMap.getNumberOfRunRecords() ;
    
    ra->_recordsAreInOrder =  true ;  //  ???? how is this defined ????  
    ra->_indexLocation = 0 ;
    ra->_prevLocation = 0 ;
    ra->_nextLocation = 0 ;
    ra->_firstRecordLocation = 0 ;

    return ra ;
  }
 
//   bool LCIORandomAccessMgr::readLastLCIORandomAccess( SIO_stream* stream ) {
//     // go to the end and see if the last record is of type LCIORandomAccess 
//     const int LCIORandomAccess_Size = 132  ; //FIXME - define globally or store at end of file ....

//     LCSIO::seekStream( stream,  -LCIORandomAccess_Size ) ;

//     return readLCIORandomAccess( stream ) ;
//   }



  bool LCIORandomAccessMgr::readLCIORandomAccessAt( SIO_stream* stream , long64 pos) {
    
    LCSIO::seekStream( stream, pos ) ;

    return readLCIORandomAccess( stream ) ;
  }


  bool LCIORandomAccessMgr::readLCIORandomAccess( SIO_stream* stream ) {

    SIO_record* accessRecord = LCSIO::records()[ SIORecords::Access ] ;
    
    SIORecords::Unpack raUnp( SIORecords::Unpack::All ) ;
    
    SIORandomAccessHandler raHandler( LCSIO_ACCESSRECORDNAME,  this ) ;
    
    SIO_blockManager::remove(  LCSIO_ACCESSRECORDNAME ) ;
    SIO_blockManager::add( &raHandler ) ;
    
    int status =  stream->read( &accessRecord ) ;
    
    if( ! (status & 1)  ){
      
      status = stream->reset() ;
      
      if( status != SIO_STREAM_SUCCESS ){
	
	throw IOException( std::string(" io error  reading LCIORandomAccess on stream: ") 
			   + *stream->getName() ) ;
      }
      
      std::cout << " ... no LCIORandomAccess record found - old file ??? " << std::endl ;
      
      return false ;
    }
    
    return true ;
  }

  bool LCIORandomAccessMgr::readLCIOIndexAt( SIO_stream* stream , long64 pos) {

    LCSIO::seekStream( stream, pos ) ;

    return readLCIOIndex( stream ) ;
  }

  bool LCIORandomAccessMgr::readLCIOIndex( SIO_stream* stream ) {

    SIORecords::Unpack raUnp( SIORecords::Unpack::All ) ;
    
    SIOIndexHandler raHandler( LCSIO_INDEXRECORDNAME,  this ) ;
    
    SIO_blockManager::remove( LCSIO_INDEXRECORDNAME  ) ;
    SIO_blockManager::add( &raHandler ) ;
    
    SIO_record* indexRecord = LCSIO::records()[ SIORecords::Index ] ;

    int status =  stream->read( &indexRecord ) ;
    
    if( ! (status & 1)  ){
      
      status = stream->reset() ;
      
      if( status != SIO_STREAM_SUCCESS ){
	  
	throw IOException( std::string(" io error  reading LCIOIndex on stream: ") 
			   + *stream->getName() ) ;
      }
      
      std::cout << " ... no LCIOIndex record found - old file ??? " << std::endl ;
      
      return false ;
    }
    
    return true ;
  }






  std::ostream& operator<<(std::ostream& os, const LCIORandomAccessMgr& ra ){
    
    os << " LCIORandomAccessMgr:  ----------------------- " << std::endl   ;

    for( std::list<LCIORandomAccess* >::const_iterator i = ra._list.begin() ; i != ra._list.end() ; ++i ){

      os << **i ; 
    }

    os  <<  ra._runEvtMap   << std::endl ;

    return os ;
  }

}



