#include "IOIMPL/LCFactory.h"



#include "IO/LCWriter.h"
#include "IO/LCReader.h"

#include "SIO/SIOWriter.h"
#include "SIO/SIOReader.h"


#include "RIO/RIOWriter.h"
#include "RIO/RIOReader.h"


using namespace IO ;
//using namespace SIO ;

namespace IOIMPL{

  LCFactory* LCFactory::_me = 0 ;
  
  
  LCFactory::LCFactory() {  
  }
  

  LCFactory* LCFactory::getInstance() { 

    if( !_me ) _me = new LCFactory ;
    return _me ;
  }
  

  LCFactory::~LCFactory() { 
    delete _me ;
  }
  
  LCWriter * LCFactory::createLCWriter() { 
    
    // the reason for having this class
    // so far we just create SIO objects

    //    return new SIO::SIOWriter ;
    return new RIO::RIOWriter ;
  }
  
  LCReader * LCFactory::createLCReader(int lcReaderFlag) {

    // so far we just create SIO objects
    //    return new SIO::SIOReader( lcReaderFlag );
   return new RIO::RIOReader( lcReaderFlag );
  }
  
  
} // namespace
