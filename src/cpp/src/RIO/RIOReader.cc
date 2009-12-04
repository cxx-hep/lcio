#include "RIO/RIOReader.h" 

#include "RIO/RIOLCCollectionHandler.h"

#include "LCIOSTLTypes.h"

#include "EVENT/LCIO.h"
#include "IMPL/LCIOExceptionHandler.h"
#include "EVENT/MCParticle.h"
#include "EVENT/SimCalorimeterHit.h"
#include "EVENT/SimTrackerHit.h"

#include "TFile.h"
#include "TTree.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <climits>

#include <sys/stat.h> 

using namespace EVENT ;
using namespace IO ;
using namespace IOIMPL ;
using namespace IMPL ;

#define EVENTKEY(RN,EN)  ( EVENT::long64( RN ) << 32 ) | EN 
 
namespace RIO {


//#define DEBUG 1
  


  RIOReader::RIOReader( int lcReaderFlag ) :
    _file(0),
    _evtTree(0),
    _runTree(0),
    _haveBranches( false ),
    _evtImpl(0), 
    _runImpl(0), 
    _myFilenames(0), 
    _currentFileIndex(0),
    _readEventMap( lcReaderFlag & LCReader::directAccess ), 
    _evtEntry(-1), _runEntry(-1) {
    
#ifdef DEBUG
#else
#endif  
    
    //    _evtImpl = new IMPL::LCEventImpl ;

    LCIOExceptionHandler::createInstance() ;
  }


  RIOReader::~RIOReader(){
    
    // clean up branch handlers
    for( BranchHandlerMap::iterator it=_branches.begin() ; it!=_branches.end() ; ++it){
      delete it->second ; 
    }
  }


  void RIOReader::open(const std::vector<std::string>& filenames) 
    throw( IOException , std::exception){

    unsigned int i;
    struct stat fileinfo ;
    std::string missing_files;
    
    // JE: first we check if all files exist
    for(i=0; i < filenames.size(); i++){
        
      if ( stat( filenames[i].c_str(), &fileinfo ) != 0 ){
	missing_files += filenames[i] ;
	missing_files += "  " ;
      }
    }

    // JE: if not raise IOException
    if( missing_files.size() != 0 ){
      throw IOException( std::string( "[RIOReader::open()] File(s) not found:  " + missing_files )) ;
    }
    
    _myFilenames = &filenames ;

    _currentFileIndex = 0 ;

    open( (*_myFilenames)[ _currentFileIndex ]  ) ;
  }



  void RIOReader::open(const std::string& filename) throw( IOException , std::exception)  {


    std::string rioFilename ;  
    // ---- we don't require the standard file extension for reading any more
    //if( !( filename.rfind(".") filename.length() ))
    //  sioFilename = filename + LCRIO::FILE_EXTENRION ;
    //else 

    rioFilename = filename ;
    
    _file = new TFile( rioFilename.c_str() , "READ");    

    if( _file->IsZombie() )
      throw IOException( std::string( "[RIOReader::open()] zombie root file : " 
				      +  rioFilename ) ) ;
    
    if( !_file->IsOpen() )
      throw IOException( std::string( "[RIOReader::open()] Couldn't open file: " 
				      +  rioFilename ) ) ;


    _evtTree = (TTree*) _file->Get("LCEvent") ;
    if( !_evtTree ){
      throw IOException( std::string( "[RIOReader::open()] Couldn't find \"LCEvent\"-TTree " 
				      +  rioFilename ) ) ;
    }
    _runTree = (TTree*) _file->Get("LCRunHeader") ;
    if( !_runTree ){
      throw IOException( std::string( "[RIOReader::open()] Couldn't find \"LCRunHeader\"-TTree " 
				      +  rioFilename ) ) ;
    }
    

    if( _readEventMap ){
      getEventMap() ;
    }
  }
  
  void RIOReader::getEventMap() {


    // //     int status = _stream->seek(0) ; // go to start - FIXME - should we store the current position ?

    // //     if( status != RIO_STREAM_SUCCESS ) 
    // //       throw IOException( std::string( "[RIOReader::getEventMap()] Can't seek stream to 0" ) ) ;
    
    // // //    std::cout << " RIOReader::getEventMap() recreating event map for direct access ..." 
    // // //	      << std::endl ;
    
    // //     { // -- scope for unpacking evt header --------
    // //       RIOUnpack hdrUnp( RIOUnpack::EVENTHDR ) ;
      
    // //       while( true ){
	
    // // 	RIO_blockManager::remove(  LCRIO::HEADERBLOCKNAME ) ;
    // // 	RIO_blockManager::add( _evtHandler ) ;

    // // 	//----	  readRecord() ;
    // // 	// read the next record from the stream
    // // 	if( _stream->getState()== RIO_STATE_OPEN ){
      
    // // 	  unsigned int status =  _stream->read( &_dummyRecord ) ;
	  
    // // 	  if( ! (status & 1)  ){

    // // 	    if( status & RIO_STREAM_EOF ){
    // // 	      break ;
    // // 	    }
	    
    // // 	    throw IOException( std::string(" io error on stream: ") + *_stream->getName() ) ;
    // // 	  }
    // // 	} else {
    // // 	  throw IOException( std::string(" stream not open: ")+ *_stream->getName() ) ;
    // // 	}
	
    // // 	//--
    // // 	int runNum = (*_evtP)->getRunNumber() ;
    // // 	int evtNum = (*_evtP)->getEventNumber() ;
	
    // //         _evtMap[  EVENTKEY( runNum , evtNum ) ] = _stream->lastRecordStart() ;
	
    // // // 	EVENT::long64 key  = (EVENT::long64( runNum ) << 32 ) | evtNum ;
    // // // 	std::cout << "  " <<  key << " - " << _stream->lastRecordStart()  
    // // // 		  << " evt: " << evtNum << std::endl ;
	
    // //       } // while

    // //       _stream->seek(0) ; // go to start - FIXME - should we store the current 

    // //       if( status != RIO_STREAM_SUCCESS ) 
    // // 	throw IOException( std::string( "[RIOReader::getEventMap()] Can't seek stream to 0" ) ) ;

    // //     }// -- end of scope for unpacking evt header --

    // // //    std::cout << " RIOReader::getEventMap() : done " << std::endl ;

  }
  


  LCRunHeader* RIOReader::readNextRunHeader() throw (IOException , std::exception ) {
    return readNextRunHeader( LCIO::READ_ONLY ) ;
  }

  LCRunHeader* RIOReader::readNextRunHeader(int accessMode) throw (IOException , std::exception ) {
    
    _runEntry ++ ;

    // read event header first
    if( ! _runHdrBranch )   {

      _runHdrBranch = (TBranch*) _runTree->GetBranch( "LCRunHeader" ) ;
      
      if( _runHdrBranch == 0 ){        
	
	throw IOException( std::string( "[RIOReader::readNextRunHeader()] cant open branch \"LCRunHeader\" " ) ) ;
      } 

      _runHdrBranch->SetAddress( &_runImpl ) ;
    }
    

    Long64_t tentry =  _runTree->LoadTree( _runEntry );

    int nbyte=0 ; 
    nbyte = _runHdrBranch->GetEntry(tentry);

    if( tentry < 0 ){
      return 0 ; // EOF ?
    }

    return _runImpl ;
  }
  
  void RIOReader::setUpHandlers(const LCEventImpl * evt){

    if( !_haveBranches ) {  // only for first event ....


      _haveBranches = true ;

      typedef std::vector< std::string > StrVec ; 

      const StrVec* strVec = evt->getCollectionNames(false) ;

      for(  StrVec::const_iterator name = strVec->begin() ; name != strVec->end() ; name++){
	
	// 	LCCollection* col = evt->getCollection( *name ) ;
	// 	const LCParameters&  params =  col->getParameters()  ;
	// 	std::string typeName = col->getTypeName() ;

 	std::string typeName("YetUnknown") ;

#ifdef DEBUG
 	std::cout << " registering collection " << *name << " of " <<  typeName <<  std::endl ;
#endif

	// 	if( *name == "MCParticlesSkimmed" )
	//	if( *name == "PandoraPFOs" )

	_branches[ *name ] =  new RIO::RIOLCCollectionHandler( *name, typeName   , _evtTree)  ;	

      }

    }
  }


  LCEvent* RIOReader::readNextEvent() throw (IOException , std::exception ) {

    return readNextEvent( LCIO::READ_ONLY ) ;

  }

  LCEvent* RIOReader::readNextEvent(int accessMode) throw (IOException, std::exception ) {

    //     if( _evtImpl != 0 )   // memory handling in ROOT I/O ? 
    //       delete _evtImpl ;
    //------------------------------------------------------  
    
      
    _evtEntry ++ ;

    // read event header first
      
    TBranch* br = (TBranch*) _evtTree->GetBranch( "LCEvent" ) ;
      
    if( br == 0 ){        
	
      throw IOException( std::string( "[RIOReader::readNextEvent()] cant open branch \"LCEvent\" " ) ) ;
    } 
      
    br->SetAddress( &_evtImpl ) ;

    Long64_t tentry =  _evtTree->LoadTree( _evtEntry );

    int nbyte=0 ; 
    nbyte = br->GetEntry(tentry);

    if( tentry < 0 ){

      return 0 ; // EOF ?
    }

    if( !_haveBranches ) {
      setUpHandlers( _evtImpl ) ;
    }    

       
    _evtImpl->setCurrentEvent( _evtImpl ) ;

    for( BranchHandlerMap::iterator it=_branches.begin() ; it!=_branches.end() ; ++it){
      (it->second)->fromBranch( _evtImpl , tentry ) ;
    }
       
       

#ifdef DEBUG

    typedef std::vector< std::string > StrVec ; 

    const StrVec& strVec = *( _evtImpl->getCollectionNames() ) ; 

    std::cout << " >>>>>>>>> tentry : " << tentry 
	      << " _evtEntry " << _evtEntry  
	      << "  eventnum " << _evtImpl->getEventNumber()  
	      << " ncols: " << strVec.size() 
	      << " nbyte: " << nbyte
	      << " _evtImpl " << _evtImpl  
	      << std::endl ;
#endif      

    //------------------------------------------------------

    return _evtImpl ;      
  }
  
  void RIOReader::skipNEvents(int n) {
    
    _evtEntry += n ;
  }


  EVENT::LCEvent * RIOReader::readEvent(int runNumber, int evtNumber) 
    throw (IOException , std::exception) {
    
    
    // FIXME: here we should use an index of the LCEvent branch for the event map

    //     EventMap::iterator it = _evtMap.find( EVENTKEY( runNumber,evtNumber ) ) ;
    
    //     if( it != _evtMap.end() ) {
    
    //       int status = _stream->seek( it->second ) ;
    
    //       if( status != RIO_STREAM_SUCCESS ) 
    // 	throw IOException( std::string( "[RIOReader::readEvent()] Can't seek stream to"
    // 					" requested position" ) ) ;
    
    //       return readNextEvent() ;
    //     } 
    //     else 
    
    //       return 0 ;
    
    
    return _evtImpl ;
  }


  void RIOReader::close() throw (IOException, std::exception ){
  
    _file->Close() ;
  }




  void RIOReader::registerLCEventListener(LCEventListener * ls){ 
    _evtListeners.insert( _evtListeners.end() , ls );
  }
  void RIOReader::removeLCEventListener(LCEventListener * ls){ 
    _evtListeners.erase( _evtListeners.find( ls )  );
  }
  
  void RIOReader::registerLCRunListener(LCRunListener * ls){ 
    _runListeners.insert( _runListeners.end() , ls );
  }

  void RIOReader::removeLCRunListener(LCRunListener * ls){
    _runListeners.erase( _runListeners.find( ls ) );
  }

  void RIOReader::readStream() throw ( IO::IOException, std::exception ){

    int maxInt = INT_MAX ; // numeric_limits<int>::max() ;
    readStream( maxInt ) ;
  }


  void RIOReader::readStream(int maxRecord) throw (IOException, std::exception ){
    

    //     bool readUntilEOF = false ;
    //     if( maxRecord == INT_MAX ) 
    //       readUntilEOF = true ;
    
    //     // here we need to read all the records on the stream
    //     // and then notify the listeners depending on the type ....
    
    //     int recordsRead = 0 ;
    //     while( recordsRead < maxRecord ){ 
	
    //       try{ 
    // 	readRecord() ;
    //       }
    //       catch(EndOfDataException){
	
    // 	// only throw exception if a 'finite' number of records was 
    // 	// specified that couldn't be read from the file
    // 	if( readUntilEOF ){  
    // 	  return ;
    // 	}else{
    // 	  std::stringstream message ;
    // 	  message << "RIOReader::readStream(int maxRecord) : EOF before " 
    // 		  << maxRecord << " records read from file" << std::ends ;
    // 	  throw EndOfDataException( message.str())  ;
    // 	}
    //       }
      
    //       // notify LCRunListeners 
    //       if( ! strcmp( _dummyRecord->getName()->c_str() , LCRIO::RUNRECORDNAME )){
	
    // 	recordsRead++ ;

    // 	std::set<IO::LCRunListener*>::iterator iter = _runListeners.begin() ;
    // 	while( iter != _runListeners.end() ){

    // 	  _runImpl->setReadOnly( false ) ;
    // 	  (*iter)->modifyRunHeader( *_runP ) ;

    // 	  _runImpl->setReadOnly( true ) ;
    // 	  (*iter)->processRunHeader( *_runP ) ;
	  
    // 	  iter++ ;
    // 	}
    //       }
    //       // notify LCEventListeners 
    //       if( ! strcmp( _dummyRecord->getName()->c_str() , LCRIO::EVENTRECORDNAME )){
	
    // 	recordsRead++ ;

    // 	std::set<IO::LCEventListener*>::iterator iter = _evtListeners.begin() ;
    // 	while( iter != _evtListeners.end() ){

    // // 	  // restore the daughter relations from the parent relations
    // // 	  RIOParticleHandler::restoreParentDaughterRelations( *_evtP ) ;
    // 	  postProcessEvent() ;

    // 	  // fg20070813 changed order of update and process (needed for 
    // 	  // Marlin modifying processors )
    // 	  (*_evtP)->setAccessMode( LCIO::UPDATE ) ;
    // 	  (*iter)->modifyEvent( *_evtP ) ;

    // 	  (*_evtP)->setAccessMode( LCIO::READ_ONLY ) ; // set the proper acces mode
    // 	  (*iter)->processEvent( *_evtP ) ;


    // 	  iter++ ;
	  
    // 	}
    //       }
    //     }
  }
  
  void  RIOReader::postProcessEvent() {
    // restore the daughter relations from the parent relations
    //    RIOParticleHandler::restoreParentDaughterRelations( *_evtP ) ;
    //     // fill the relation map from intermediate vector
    //     RIOLCRelationHandler::fillRelationMap(  *_evtP ) ;
  }
  
} // namespace
