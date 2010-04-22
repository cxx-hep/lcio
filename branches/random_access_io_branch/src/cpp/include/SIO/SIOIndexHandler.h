#ifndef SIO_SIOIndexHandler_H
#define SIO_SIOIndexHandler_H 1

//#include <string>

#include "LCIORandomAccessMgr.h"
#include "SIO_block.h"

namespace SIO {
    
/** Handler for LCIOrandomAccess and LCIOIndex objects/blocks.
 * 
 * @author gaede
 * @version $Id: SIOIndexHandler.h,v 1.1.2.2 2010-04-22 16:34:09 gaede Exp $
 */
  class SIOIndexHandler : public SIO_block{
    
  protected:
    SIOIndexHandler() : SIO_block("UNKNOWN") { /* no default c'tor*/  ;} 

  public:
    
    /** C'tor.
     */
    SIOIndexHandler(const std::string& name, LCIORandomAccessMgr* raMgr) ; 

//     SIOIndexHandler(const std::string& name, LCIORandomAccess** raP) ;
    virtual ~SIOIndexHandler() ;
    
    // interface from SIO_block
    virtual unsigned int   xfer( SIO_stream*, SIO_operation, unsigned int ) ;
    virtual unsigned int   version() ;
    
//     void setWritePtr(const LCIORandomAccess* ra ) ; 
//     void setReadPtr( LCIORandomAccess** raP ) ; 
    
  private: 
//     LCIORandomAccess** _raP ;  // address for reading
//     const LCIORandomAccess* _ra ;  // object for writing

//     RunEventMap* _reMap ;
    LCIORandomAccessMgr* _raMgr ;

  }; // class
  
} // namespace

#endif /* ifndef SIO_SIORUNHEADERHANDLER_H */
