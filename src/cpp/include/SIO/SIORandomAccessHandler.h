#ifndef SIO_SIORandomAccessHandler_H
#define SIO_SIORandomAccessHandler_H 1

//#include <string>

#include "LCIORandomAccessMgr.h"
#include "SIO_block.h"

namespace SIO {
    
/** Handler for LCIOrandomAccess and LCIOIndex objects/blocks.
 * 
 * @author gaede
 * @version $Id: SIORandomAccessHandler.h,v 1.2 2010-06-22 13:49:54 gaede Exp $
 */
  class SIORandomAccessHandler : public SIO_block{
    
  protected:
    SIORandomAccessHandler() : SIO_block("UNKNOWN") { /* no default c'tor*/  ;} 

  public:
    
    /** C'tor.
     */
    SIORandomAccessHandler(const std::string& name, LCIORandomAccessMgr* raMgr) ; 

//     SIORandomAccessHandler(const std::string& name, LCIORandomAccess** raP) ;
    virtual ~SIORandomAccessHandler() ;
    
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
