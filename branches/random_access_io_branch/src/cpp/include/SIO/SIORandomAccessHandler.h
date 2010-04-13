#ifndef SIO_SIORandomAccessHandler_H
#define SIO_SIORandomAccessHandler_H 1

//#include <string>

#include "LCIORandomAccess.h"
#include "SIO_block.h"

namespace SIO {
    
/** Handler for LCIOrandomAccess objects.
 * 
 * @author gaede
 * @version $Id: SIORandomAccessHandler.h,v 1.1.2.1 2010-04-13 11:10:44 gaede Exp $
 */
  class SIORandomAccessHandler : public SIO_block{
    
  protected:
    SIORandomAccessHandler() : SIO_block("UNKNOWN") { /* no default c'tor*/  ;} 

  public:
    
    /** C'tor for writing
     */
    SIORandomAccessHandler(const std::string& name) ;

    /** C'tor for reading.
     */
    SIORandomAccessHandler(const std::string& name, LCIORandomAccess** raP) ;
    virtual ~SIORandomAccessHandler() ;
    
    // interface from SIO_block
    virtual unsigned int   xfer( SIO_stream*, SIO_operation, unsigned int ) ;
    virtual unsigned int   version() ;
    
    void setWritePtr(const LCIORandomAccess* ra ) ; 
    void setReadPtr( LCIORandomAccess** raP ) ; 
    
  private: 
    LCIORandomAccess** _raP ;  // address for reading
    const LCIORandomAccess* _ra ;  // object for writing
    
  }; // class
  
} // namespace

#endif /* ifndef SIO_SIORUNHEADERHANDLER_H */
