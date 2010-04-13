#include "SIO/SIOWriter.h" 

#include "EVENT/LCEvent.h"
#include "EVENT/LCRunHeader.h"
#include "EVENT/LCIO.h"
#include "EVENT/LCCollection.h"

#include "SIO/LCSIO.h"
#include "SIO/SIOEventHandler.h" 
#include "SIO/SIOCollectionHandler.h" 
//#include "SIO/SIOLCRelationHandler.h" 
#include "SIO/SIORunHeaderHandler.h" 

#include "LCIOSTLTypes.h"

#include "SIO_streamManager.h" 
#include "SIO_recordManager.h" 
#include "SIO_blockManager.h" 
#include "SIO_stream.h" 
#include "SIO_record.h" 
#include "IMPL/LCIOExceptionHandler.h"

//#define DEBUG 1
#include "IMPL/LCTOOLS.h"
#include "IMPL/LCRelationImpl.h" 

#include <cstring>

using namespace EVENT ;
using namespace IO ;
using namespace IMPL ;

namespace SIO {

  SIOWriter::SIOWriter() :  _stream(0),
			    _compressionLevel(-1), 
			    _hdrHandler(0), 
			    _runHandler(0)  {
    
#ifdef DEBUG
    SIO_streamManager::setVerbosity( SIO_ALL ) ;
    SIO_recordManager::setVerbosity( SIO_ALL ) ;
    SIO_blockManager::setVerbosity( SIO_ALL ) ;
#else
    SIO_streamManager::setVerbosity( SIO_SILENT ) ;
    SIO_recordManager::setVerbosity( SIO_SILENT ) ;
    SIO_blockManager::setVerbosity(  SIO_SILENT ) ;
#endif
//     SIO_streamManager::setVerbosity( SIO_ERRORS ) ;
//     SIO_recordManager::setVerbosity( SIO_ERRORS ) ;
//     SIO_blockManager::setVerbosity(  SIO_ERRORS ) ;


    _runHandler = new SIORunHeaderHandler( LCSIO_RUNBLOCKNAME  ) ;
    _hdrHandler = new SIOEventHandler( LCSIO_HEADERBLOCKNAME ) ;
  
    _evtRecord = LCSIO::records()[ SIORecords::Event ] ;
    _hdrRecord = LCSIO::records()[ SIORecords::Header ] ;
    _runRecord = LCSIO::records()[ SIORecords:: Run ] ;
    
    LCIOExceptionHandler::createInstance() ;

  }

  SIOWriter::~SIOWriter(){
    
    delete _hdrHandler ;
    delete _runHandler ;
    SIO_blockManager::clear();
    
  }



  void SIOWriter::open(const std::string & filename) throw(IOException, std::exception){

    std::string sioFilename ;  
    getSIOFileName( filename, sioFilename ) ;

    // if the file exists we throw an exception

    FILE* f = fopen( sioFilename.c_str() , "r") ;
    if( f != 0 ){
      fclose(f) ;
      throw IOException( std::string( "[SIOWriter::open()] File already exists: " 
    				      + sioFilename
				      + " \n              open it in append or new mode !\n"
				      )) ;
    }
    // open new file for writing
    return open( filename, EVENT::LCIO::WRITE_NEW ) ;
  }

  void SIOWriter::getSIOFileName(const std::string& filename, 
				 std::string& sioFilename ) {


    if( filename.rfind(LCSIO::FILE_EXTENSION) == std::string::npos ||  // .slcio not found at all
	!(  filename.rfind(LCSIO::FILE_EXTENSION)
	    + strlen( LCSIO::FILE_EXTENSION ) == filename.length() ) ) {  // found, but not at end 
      
      // find_last_of looks for characters and not substrings !!
//     if( filename.find_last_of(LCSIO::FILE_EXTENSION) != filename.length() - 1  ) {

      sioFilename = filename + LCSIO::FILE_EXTENSION ;
    } 
    else 
      sioFilename = filename ;    
  } 

  void SIOWriter::open(const std::string& filename, int writeMode) throw(IOException, std::exception) {

    
    // make sure filename has the proper extension (.slcio) 
    std::string sioFilename ;  
    getSIOFileName( filename, sioFilename ) ;

    // SIO has some rules about valid names for streams, records, etc ...
    //    const char* stream_name = LCSIO::getValidSIOName(sioFilename) ;
    std::string stream_name = LCSIO::getValidSIOName(sioFilename) ;
    _stream = SIO_streamManager::add(  stream_name.c_str() , 32*SIO_KBYTE*SIO_KBYTE ) ;
    
    if( _stream == 0 )
      throw IOException( std::string( "[SIOWriter::open()] Bad or duplicate stream name: " 
 				      + stream_name  )) ;

    // SIO_stream takes any value and maps it to [-1,0,1...,9]
    _stream->setCompressionLevel( _compressionLevel ) ;
    
    
    unsigned int  status = 0  ;
    switch( writeMode ) 
      {
      case EVENT::LCIO::WRITE_NEW : 
	status  = _stream->open( sioFilename.c_str() , SIO_MODE_WRITE_NEW ) ; 
	break ;
      case EVENT::LCIO::WRITE_APPEND : 
	status  = _stream->open( sioFilename.c_str() , SIO_MODE_WRITE_APPEND ) ; 
	break ;
      }

    if( !(status &1) )
      throw IOException( std::string( "[SIOWriter::open()] Couldn't open file: " 
				      +  sioFilename ) ) ;
      
//      // tell SIO the record names if not yet known 
//      if( (_runRecord = SIO_recordManager::get( LCSIO::RUNRECORDNAME )) == 0 )
//      _runRecord = SIO_recordManager::add( LCSIO::RUNRECORDNAME ) ;

//      if( (_hdrRecord = SIO_recordManager::get( LCSIO::HEADERRECORDNAME )) == 0 )
//      _hdrRecord = SIO_recordManager::add( LCSIO::HEADERRECORDNAME ) ;

//      if( (_evtRecord = SIO_recordManager::get( LCSIO::EVENTRECORDNAME )) ==0 ) 
//      _evtRecord = SIO_recordManager::add( LCSIO::EVENTRECORDNAME ) ;

//      _hdrRecord->setCompress( _compressionLevel != 0 ) ;
//      _evtRecord->setCompress( _compressionLevel != 0 ) ;
//      _runRecord->setCompress( _compressionLevel != 0 ) ; 

    LCSIO::records().setCompress( _compressionLevel != 0 ) ; 
     
  }

  void SIOWriter::setCompressionLevel(int level) {
    _compressionLevel = level ;
  }


  void SIOWriter::writeRunHeader(const EVENT::LCRunHeader * hdr)  throw(IOException, std::exception) {

    // create a new handler for every new run 
    
    _runRecord->disconnect(LCSIO_RUNBLOCKNAME ) ;
    _runRecord->connect( _runHandler );


    _runHandler->setRunHeader(  hdr ) ;
    
    if( _stream->getState()== SIO_STATE_OPEN ){
      
//       _hdrRecord->setCompress( _compressionLevel != 0 ) ;
//       _evtRecord->setCompress( _compressionLevel != 0 ) ;
//       _runRecord->setCompress( _compressionLevel != 0 ) ; 

      LCSIO::records().setCompress( _compressionLevel != 0 ) ; 
      
      // write LCRunHeader record
      unsigned int status =  _stream->write( LCSIO_RUNRECORDNAME    ) ;
      
      if( !(status & 1)  )
	throw IOException( std::string( "[SIOWriter::writeRunHeader] couldn't write run header to stream: "
					+  *_stream->getName() ) ) ;
    } else {
      
      throw IOException( std::string( "[SIOWriter::writeRunHeader] stream not opened: "
				      +  *_stream->getName() ) ) ;
      
    }
  }



  /** Creates Handlers needed for writing the event on this stream.
   * Needs to be called for every event.
   */
  void SIOWriter::setUpHandlers(const LCEvent * evt){
  
    
    _hdrRecord->disconnect( LCSIO_HEADERBLOCKNAME ) ;
    _hdrRecord->connect( _hdrHandler ) ;
    
    // need to disconnect all blocks for multiple I/O streams
    _evtRecord->disconnectAll() ;
    
    const std::vector<std::string>* strVec = evt->getCollectionNames() ;
    
    for( std::vector<std::string>::const_iterator name = strVec->begin() ; name != strVec->end() ; name++){
      
      
      SIOCollectionHandler* ch = dynamic_cast<SIOCollectionHandler*> 
	( SIO_blockManager::get( name->c_str() )  ) ;
      
      LCCollection* col = evt->getCollection( *name ) ;
      
      if(! col->isTransient() ){ // if a collection is transient we simply ignore it

	try{

	  if( ch == 0 ) {
	    ch =  new SIOCollectionHandler( *name, col->getTypeName())  ;
	  }
	  _evtRecord->connect( ch ) ;
// 	  _connectedBlocks.push_back( ch ) ;  
	  ch->setCollection( col ) ; 
	  
	} 
	catch(Exception& ex){   // unsuported type !
	  delete ch ;
	  ch =  0 ;
	}
	
      }
    } 
  }

  void SIOWriter::writeEvent(const LCEvent* evt)  throw(IOException, std::exception) {

    
    
    //here we set up the collection handlers 
    
    try{   setUpHandlers( evt) ;
    
    }catch(...){
      throw IOException(  "[SIOWriter::writeEvent] could not set up handlers " ) ;
    }

//     _hdrRecord->setCompress( _compressionLevel != 0 ) ;
//     _evtRecord->setCompress( _compressionLevel != 0 ) ;
//     _runRecord->setCompress( _compressionLevel != 0 ) ; 

    LCSIO::records().setCompress( _compressionLevel != 0 ) ; 

    if( _stream->getState()== SIO_STATE_OPEN ){
   
      // need to set the event in event header handler
      _hdrHandler->setEvent( evt ) ;

      unsigned int  status = 0  ;

      // write LCEventHeader record
      status =  _stream->write( LCSIO_HEADERRECORDNAME    ) ;

      if( ! (status & 1) )
	throw IOException(  std::string("[SIOWriter::writeEvent] couldn't write event header to stream: "
					+  *_stream->getName() )) ;
      
      
      // write the event record
      status =  _stream->write( LCSIO_EVENTRECORDNAME    ) ;

      if( ! (status & 1) )
	throw IOException(  std::string("[SIOWriter::writeEvent] couldn't write event header to stream: "
					+  *_stream->getName() )) ;
    }
    else      

      throw IOException(  std::string("[SIOWriter::writeEvent] stream not opened : "
				      +  *_stream->getName()  )) ;
    
  }


  void SIOWriter::close() throw (IOException, std::exception) {
  
    const std::string* streamName  = _stream->getName() ;

    int status  =  SIO_streamManager::remove( _stream ) ;
    
    if(! (status &1) ) 
      throw IOException(  std::string("[SIOWriter::close] couldn't close stream  : "
				      + *streamName  )) ;

  }

  void SIOWriter::flush() throw (IOException, std::exception) {
  
    const std::string* streamName  = _stream->getName() ;

    int status =  _stream->flush() ;
    
    if(! (status &1) ) 
      throw IOException(  std::string("[SIOWriter::flush] couldn't flush stream  : "
				      + *streamName  )) ;

  }

} // namespace

