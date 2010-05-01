#include "SIO/LCIORandomAccessMgr.h"

#include "SIO/LCSIO.h"
#include "SIO/SIORandomAccessHandler.h"
#include "SIO/SIOIndexHandler.h"
#include "SIO/SIOEventHandler.h"
#include "SIO/SIORunHeaderHandler.h"

#include "IOIMPL/LCEventIOImpl.h"
#include "IOIMPL/LCRunHeaderIOImpl.h"

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
      
//       std::cout << " ... no LCIORandomAccess record found - old file ??? " << std::endl ;
      
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


  void LCIORandomAccessMgr::initAppend( SIO_stream* stream ) {

    // check if the last record is LCIORandomAccess
    if( ! readLCIORandomAccessAt( stream , -LCSIO_RANDOMACCESS_SIZE) )  {

      // else:
      recreateEventMap( stream ) ; 
      return ;
    }
  }

  bool LCIORandomAccessMgr::getEventMap( SIO_stream* stream ) {

    // check if the last record is LCIORandomAccess
    if( ! readLCIORandomAccessAt( stream , -LCSIO_RANDOMACCESS_SIZE) )  {

      // else:
      return recreateEventMap( stream ) ; 
    }

    //read all remaining LCIORandomAccess records

    const LCIORandomAccess* ra = lastLCIORandomAccess() ;

    EVENT::long64 raPos = ra->getPrevLocation() ;

    EVENT::long64 indexPos = ra->getIndexLocation() ;
    
    readLCIOIndexAt( stream , indexPos ) ;

    while( raPos != 0 ){

      if( readLCIORandomAccessAt( stream , raPos) ){

	ra = lastLCIORandomAccess() ;

	raPos = ra->getPrevLocation() ;	

	//	std::cout << " read ra at " << ra << " : " << *ra << "  - prevPos : " << raPos << std::endl ;

	EVENT::long64 indexPos = ra->getIndexLocation() ;

	readLCIOIndexAt( stream , indexPos ) ;

      }else{
	throw IOException( std::string( "[LCIORandomAccessMgr::ReadEventMap()] Could not read previous LCIORandomAccess record" ) ) ;
      }      
    }

    return true ;

    //    std::cout << " ... LCIORandomAccess read from stream : "<< *ra << std::endl ;
  }

  bool LCIORandomAccessMgr::recreateEventMap( SIO_stream* stream ) {

//      std::cout << " LCIORandomAccessMgr::getEventMap() recreating event map for direct access ..." 
// 	       << std::endl ;

    LCSIO::seekStream( stream, 0 ) ;// go to start of file
    
    
    SIO_record* dummyRecord ; 
    IOIMPL::LCEventIOImpl* evtPtr ;
    IOIMPL::LCRunHeaderIOImpl* runPtr ;
    
    SIORunHeaderHandler runHandler( LCSIO_RUNBLOCKNAME, &runPtr ) ;
    SIOEventHandler eventHandler( LCSIO_HEADERBLOCKNAME, &evtPtr ) ;
    
    SIO_blockManager::remove(  LCSIO_HEADERBLOCKNAME ) ;
    SIO_blockManager::add( &eventHandler ) ;
    SIO_blockManager::remove(  LCSIO_RUNBLOCKNAME ) ;
    SIO_blockManager::add( &runHandler ) ;
    
    { // -- scope for unpacking evt and run headers  --------
      SIORecords::Unpack hdrUnp( SIORecords::Unpack::Header + SIORecords::Unpack::Run ) ;
      
      while( true ){
	
	
	//----	  readRecord() ;
	// read the next record from the stream
	if( stream->getState()== SIO_STATE_OPEN ){
	  
	  unsigned int status =  stream->read( &dummyRecord ) ;
	  
	  if( ! (status & 1)  ){
	    
	    if( status & SIO_STREAM_EOF ){
	      break ;
	    }
	    
	    throw IOException( std::string(" io error on stream: ") + *stream->getName() ) ;
	  }
	} else {
	  throw IOException( std::string(" stream not open: ")+ *stream->getName() ) ;
	}
	
	//--
	int runNum = -1 ;
	int evtNum = -1 ;
	
	if( ! strcmp( dummyRecord->getName()->c_str() , LCSIO_HEADERRECORDNAME )){
	  
	  runNum = evtPtr->getRunNumber() ;
	  evtNum = evtPtr->getEventNumber() ;
	}
	if( ! strcmp( dummyRecord->getName()->c_str() , LCSIO_RUNRECORDNAME )){
	  
	  runNum = runPtr->getRunNumber() ;
	}
	
	_runEvtMap.add(   RunEvent( runNum , evtNum ) ,  stream->lastRecordStart() ) ;
	
	
	//  	EVENT::long64 key  = (EVENT::long64( runNum ) << 32 ) | evtNum ;
	//  	std::cout << "  " <<  key << " - " << stream->lastRecordStart()  
	//  		  << " evt: " << evtNum << std::endl ;
	
      } // while
      
      
      LCSIO::seekStream( stream, 0 ) ;// go to start of file
      
    }// -- end of scope for unpacking evt header --
    
    //     std::cout << " LCIORandomAccessMgr::getEventMap() : done " << std::endl ;
    
    return true ;
  }
  
  void LCIORandomAccessMgr::writeRandomAccessRecords(SIO_stream* stream) {
    
    if( _runEvtMap.empty() ) { 
      return ;       // nothing to write          
    }
    //-------------------------------------

    SIO_record* accessRecord = LCSIO::records()[ SIORecords::Access ] ;
    SIO_record* indexRecord  = LCSIO::records()[ SIORecords::Index ] ;
    
    SIORandomAccessHandler raHandler (  LCSIO_ACCESSRECORDNAME, this ) ;
    SIOIndexHandler idxHandler(  LCSIO_INDEXRECORDNAME,   this ) ;
    
    accessRecord->connect( &raHandler ) ;
    indexRecord->connect( &idxHandler ) ;
    
    
    if( stream->getState() != SIO_STATE_OPEN ){
      throw IOException( std::string( "[LCIORandomAccessMgr::writeRandomAccessRecords] stream not opened: "
				      +  *stream->getName() ) ) ;
    }

    //    LCSIO::records().setCompress( _compressionLevel != 0 ) ; 
    
    // write LCIOIndex record
    unsigned int status =  stream->write( LCSIO_INDEXRECORDNAME    ) ;
    
    if( !(status & 1)  )
      throw IOException( std::string( "[LCIORandomAccessMgr::writeRandomAccessRecords] couldn't write LCIOIndex to stream: "
				      +  *stream->getName() ) ) ;



    // create the LCIORandomAccess object ( linked list of records ) 
    LCIORandomAccess* ra = createFromEventMap() ;
    
    ra->setIndexLocation( stream->lastRecordStart() ) ;

    //FIXME: mis-use getFirstRecordLocation for now - should become an attribute : "thisLocation"
    ra->setFirstRecordLocation(  stream->currentPosition()  ) ;

    const LCIORandomAccess* lRa  = lastLCIORandomAccess() ;

    EVENT::long64 prevPos = (  lRa ? lRa->getFirstRecordLocation()  : 0 ) ;

    //    std::cout << " setting previous location : " << prevPos << "  from last ra : " << lRa << std::endl ;  

    ra->setPreviousLocation(  prevPos ) ;


    // the last LCIORandomAccess object will be written to the file by the SIORandomAccessHandler
    addLCIORandomAccess( ra ) ; 
    
    
    // write LCAccess record
    status =  stream->write( LCSIO_ACCESSRECORDNAME    ) ;
    
    if( !(status & 1)  )
      throw IOException( std::string( "[LCIORandomAccessMgr::writeRandomAccessRecords] couldn't write LCIORandomAccess to stream: "
				      +  *stream->getName() ) ) ;
    


  }



  //---------------------------------------------

  std::ostream& operator<<(std::ostream& os, const LCIORandomAccessMgr& ra ){
    
    os << " LCIORandomAccessMgr:  ----------------------- " << std::endl   ;
    
    for( std::list<LCIORandomAccess* >::const_iterator i = ra._list.begin() ; i != ra._list.end() ; ++i ){
      
      os << **i ; 
    }
    
    os  <<  ra._runEvtMap   << std::endl ;
    
    return os ;
  }
}
