#ifndef SIO_SIOSTRVECHANDLER_H
#define SIO_SIOSTRVECHANDLER_H 1

#include "SIO/SIOObjectHandler.h"


namespace SIO {
    
    
/** Implementation of SIOObjectHandler to handle IO of StrVecs.
 *
 * @author vogt
 * @version $Id: SIOStrVecHandler.h,v 1.2 2004-08-20 16:45:25 gaede Exp $
 */
  class SIOStrVecHandler : public SIOObjectHandler {
	
  public:
	
    /** Reads objects from an SIO stream.
     */
    virtual unsigned int read(SIO_stream* stream, 
			      EVENT::LCObject** objP)  ;
	
    /** Writes lcio objects to an SIO stream.
     */
    virtual unsigned int write(SIO_stream* stream, 
			       const EVENT::LCObject* obj) ;
	
  }; // class
}; // namespace

#endif /* ifndef SIO_SIOSTRVECHANDLER_H */
