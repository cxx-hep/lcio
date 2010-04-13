#ifndef SIO_SIOIndexHandler_H
#define SIO_SIOIndexHandler_H 1

//#include <string>

#include "LCIORandomAccess.h"
#include "SIO_block.h"

namespace SIO {
    
/** Handler for LCIOIndex blocks ...
 * 
 * @author gaede
 * @version $Id: SIOIndexHandler.h,v 1.1.2.1 2010-04-13 19:35:08 gaede Exp $
 */
  class SIOIndexHandler : public SIO_block{
    
  protected:
    SIOIndexHandler() : SIO_block("UNKNOWN") { /* no default c'tor*/  ;} 

  public:
    
    /** C'tor.
     */
    SIOIndexHandler(const std::string& name, RunEventMap* map) ;

    virtual ~SIOIndexHandler() ;
    
    // interface from SIO_block
    virtual unsigned int   xfer( SIO_stream*, SIO_operation, unsigned int ) ;
    virtual unsigned int   version() ;
    
    
  private: 
    RunEventMap* _map ;
    
  }; // class
  
} // namespace

#endif /* ifndef SIO_SIORUNHEADERHANDLER_H */
