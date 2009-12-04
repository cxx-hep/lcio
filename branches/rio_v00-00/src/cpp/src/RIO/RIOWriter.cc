#include "RIO/RIOWriter.h" 

#include "EVENT/LCEvent.h"
#include "EVENT/LCRunHeader.h"
#include "EVENT/LCIO.h"
#include "EVENT/LCCollection.h"

#include "LCIOSTLTypes.h"

#include "EVENT/MCParticle.h"
#include "EVENT/SimCalorimeterHit.h"
#include "EVENT/SimTrackerHit.h"

#include "IMPL/LCIOExceptionHandler.h"
#include "UTIL/LCTOOLS.h"

#define DEBUG 1
#ifdef DEBUG
#include "IMPL/LCTOOLS.h"
#endif

#include "IMPL/LCRelationImpl.h" 

#include <cstring>


#include "TFile.h"
#include "TTree.h"

#include "RIO/RIOLCCollectionHandler.h"


using namespace EVENT ;
using namespace IO ;
using namespace IMPL ;

namespace RIO {

  RIOWriter::RIOWriter() : 
    _compressionLevel(-1) ,
    _file(0),
    _evtTree(0),_runTree(0),
    _evtImpl(0), _runImpl(0),
    _runHdrBranch(0),
    _haveBranches( false ) {
    
    	// file needs 


    LCIOExceptionHandler::createInstance() ;
  }
  

  RIOWriter::~RIOWriter(){
  }

  void RIOWriter::open(const std::string & filename) throw(IOException, std::exception){
    
    std::string rioFilename ;  
    getRIOFileName( filename, rioFilename ) ;
    
    // if the file exists we throw an exception
    _file = new TFile( rioFilename.c_str() , "NEW");    

    if( _file->IsOpen() ){ 

      _file->Close() ;      

      throw IOException( std::string( "[RIOWriter::open()] File already exists: " 
    				      + rioFilename
				      + " \n              open it in append or new mode !\n"
				      )) ;
    }

    // open new file for writing
    return open( filename, EVENT::LCIO::WRITE_NEW ) ;
  }

  
  void RIOWriter::getRIOFileName(const std::string& filename, 
				 std::string& rioFilename ) {
    
    
    if( filename.rfind(RIO_FILE_EXTENSION) == std::string::npos ||  // .rlcio not found at all
	!(  filename.rfind(RIO_FILE_EXTENSION)
	    + strlen( RIO_FILE_EXTENSION ) == filename.length() ) ) {  // found, but not at end 
      
      rioFilename = filename + RIO_FILE_EXTENSION ;
    } 
    else 
      
      rioFilename = filename ;    
  } 
  
  void RIOWriter::open(const std::string& filename, int writeMode) throw(IOException, std::exception) {
    
    
    // make sure filename has the proper extension (.slcio) 
    std::string rioFilename ;  
    
    getRIOFileName( filename, rioFilename ) ;
    

    if( _compressionLevel  < 0  ){

      _compressionLevel = RIO_DEFAULT_COMPRESS ; 
    } 


    switch( writeMode )  {
      
    case EVENT::LCIO::WRITE_NEW : 
      
      _file = new TFile( rioFilename.c_str() , "RECREATE" , " LCIO file " , _compressionLevel );
      _evtTree = new TTree( "LCEvent" , "lcio event tree");
      _runTree = new TTree( "LCRunHeader" , "lcio run header tree");
      break ;
      
    case EVENT::LCIO::WRITE_APPEND : 
      
      _file = new TFile( rioFilename.c_str() , "UPDATE" , " LCIO file " , _compressionLevel );
      _evtTree =  (TTree*) _file->Get(  "LCEvent" ) ;
      _runTree =  (TTree*) _file->Get(  "LCRunHeader" ) ;
      
      if( _evtTree == 0 ) {
	throw IOException( std::string( "[RIOWriter::open()]  LCIO event tree not found in file: " 
					+  rioFilename ) ) ;
      } else {
	_file->Delete(   "LCEvent;1" ) ; // delete the existing old cycle
      }
      if( _runTree == 0 ) {
	throw IOException( std::string( "[RIOWriter::open()]  LCIO run header tree not found in file: " 
					+  rioFilename ) ) ;
      } else {
	_file->Delete(   "LCRunHeader;1" ) ; // delete the existing old cycle
      }
      
      break ;
    }
    
    if( _file->IsZombie() )
      throw IOException( std::string( "[RIOWriter::open()] zombie root file : " 
				      +  rioFilename ) ) ;
    
    
    if( !_file->IsOpen() )
      throw IOException( std::string( "[RIOWriter::open()] Couldn't open file: " 
				      +  rioFilename ) ) ;


    _evtTree->SetAutoSave() ;
    _runTree->SetAutoSave() ;
  }
  

  void RIOWriter::setCompressionLevel(int level) {
    _compressionLevel = level ;
  }


  void RIOWriter::writeRunHeader(const EVENT::LCRunHeader * hdr)  throw(IOException, std::exception) {
    
#ifdef DEBUG
    std::cout << "------- RIOWriter::writeRunHeader() "  <<   hdr->getRunNumber() << std::endl ;
#endif

    if(  !_runHdrBranch ){
      
      _runHdrBranch = (TBranch*) _runTree->GetBranch( RIO_LCRUNHEADER_BRANCHNAME ) ;
      
      if( _runHdrBranch != 0 ){  // branch allready exists -> update/append  mode 
	
	_runHdrBranch->SetAddress( &_runImpl ) ;
	
      } else {
	
	//FIXME: make split level and 'record size' parameters ....
	_runHdrBranch = _runTree->Branch( RIO_LCRUNHEADER_BRANCHNAME , &_runImpl, RIO_RECORD_SIZE, RIO_SPLIT_LEVEL );
      }
#ifdef DEBUG
      std::cout << "------- RIOWriter::writeRunHeader() branch : " << _runHdrBranch <<  std::endl ;
#endif
      
      if( !_runHdrBranch ){
	throw IOException("[RIOWriter::writeRunHeader] cannot create or find branch for LCRunHeader !" ) ;
      }
    } 
    _runImpl =  dynamic_cast<const LCRunHeaderImpl*>( hdr)  ; 
    _runTree->Fill() ;

  }
  
  void RIOWriter::setUpHandlers(const LCEvent * evt){
   
    if( !_haveBranches ) {
      

      // first we create a branch for the event (header) 
      
      TBranch* br = (TBranch*) _evtTree->GetBranch( RIO_LCEVENT_BRANCHNAME ) ;
      
      if( br != 0 ){  // branch allready exists -> update/append  mode 
	
	br->SetAddress( &_evtImpl ) ;
	
      } else {
	
	//FIXME: make split level and 'record size' parameters ....
	br = _evtTree->Branch( RIO_LCEVENT_BRANCHNAME , &_evtImpl, RIO_RECORD_SIZE, RIO_SPLIT_LEVEL );
      }

      // if we want to have one branch per collection (when pointer isssue is resolved)
      //---- loop over all collections in first event ...
      typedef std::vector< std::string > StrVec ; 
      const StrVec* strVec = evt->getCollectionNames() ;
      for(  StrVec::const_iterator name = strVec->begin() ; name != strVec->end() ; name++){
	LCCollection* col = evt->getCollection( *name ) ;
	std::string typeName = col->getTypeName() ;

#ifdef DEBUG
	std::cout << " registering collection " << *name << " of " <<  typeName <<  std::endl ;
#endif
	//	if( *name != "RecoMCTruthLink" ) 
	//	if( *name == "PandoraPFOs" ) 
	  _branches[ *name ] =  new RIO::RIOLCCollectionHandler( *name, typeName, _evtTree) ;	 
      }

      _haveBranches = true ;
    }
  }
  
  
  void RIOWriter::writeEvent(const LCEvent* evt)  throw(IOException, std::exception) {
    
#ifdef DEBUG
    std::cout <<  " RIOWriter::writeEvent : " << std::endl ;
#endif

    if( !_haveBranches ) {
      
      setUpHandlers( evt ) ;
    }    
    
    const LCEventImpl* evtImpl = dynamic_cast<const IMPL::LCEventImpl*> ( evt )   ;
    if( ! evtImpl ){
      throw IO::IOException(" evt is not  IMPL::LCEventImpl !?" ) ;
    }
    
    // create a proxy event - copy of the event w/o collections 
    // then add to the proxy only those collections that are to be written to the ROOT file
    LCEventImpl proxyEvt( *evtImpl ) ;
    
    typedef std::vector< std::string > StrVec ; 
    const StrVec* strVec = evt->getCollectionNames() ;
    for(  StrVec::const_iterator name = strVec->begin() ; name != strVec->end() ; name++){
      
      LCCollection* col = evt->getCollection( *name ) ;

      if( ! col->isTransient() ){
	
	if(_branches.find( *name) != _branches.end() ){

	  proxyEvt.addCollection( col , *name ) ;
	}
	else{
	  std::cout << "WARNING [RIOWriter]: no handler for collection " <<  *name << " found - collection is not stored !!!" << std::endl ;
	}
      }
    }
    
    proxyEvt.ptrToIndex();

    // --- let the handlers write their collections
    for( BranchHandlerMap::iterator it=_branches.begin() ; it!=_branches.end() ; ++it){
      it->second->toBranch( evt ) ;
    }
    
    _evtImpl = &proxyEvt ; 
    _evtTree->Fill() ;
    //_file->Flush() ;

    // now we need to take the ownership for the  collections away from proxy Event
    strVec = proxyEvt.getCollectionNames() ;
    for(  StrVec::const_iterator name = strVec->begin() ; name != strVec->end() ; name++){
	_evtImpl->takeCollection( *name ) ;
    }    

#ifdef DEBUG
    UTIL::LCTOOLS::dumpEvent( evt ) ;
#endif
  }
  
  
  void RIOWriter::close() throw (IOException, std::exception) {

    _file->Write() ;
    _file->Close() ;
  }
  
  void RIOWriter::flush() throw (IOException, std::exception) {

    // fg: we should not flush but use the auto safe feature for ROOT ....
    //_file->Flush() ;
  }
  
} // namespace

