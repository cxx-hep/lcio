// -*- C++ -*-
/** Handler for LCEvent/LCEventIOImpl objects.
 * 
 * @author gaede
 * @version Mar 6, 2003
 */
#ifndef SIO_SIOEVENTHANDLER_H
#define SIO_SIOEVENTHANDLER_H 1

#include <string>

#include "EVENT/LCEvent.h"
#include "IOIMPL/LCEventIOImpl.h"

#include "SIO_block.h"

namespace SIO {
  
  
  class SIOEventHandler : public SIO_block{
    
  public:
    static char* const EVENT="Event" ;
    
  public:
    
    SIOEventHandler() ;
    SIOEventHandler(IOIMPL::LCEventIOImpl** evtP) ;
    ~SIOEventHandler() ;
    
    // interface from SIO_block
    virtual unsigned int   xfer( SIO_stream*, SIO_operation, unsigned int ) ;
    virtual unsigned int   version() ;
    
    void setEvent(const EVENT::LCEvent* evt ) ; 
    
  private: 
    IOIMPL::LCEventIOImpl **_evtP ;     // event for reading 
    const EVENT::LCEvent *_evt ;  // event for writing
    
  }; // class
  
}; // namespace

#endif /* ifndef SIO_SIOEVENTHANDLER_H */
