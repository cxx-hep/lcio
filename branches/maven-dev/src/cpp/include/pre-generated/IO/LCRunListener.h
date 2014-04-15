// -*- C++ -*-
// AID-GENERATED
// =========================================================================
// This class was generated by AID - Abstract Interface Definition          
// DO NOT MODIFY, but use the org.freehep.aid.Aid utility to regenerate it. 
// =========================================================================
#ifndef IO_LCRUNLISTENER_H
#define IO_LCRUNLISTENER_H 1

#include "EVENT/LCRunHeader.h"

namespace IO {

/** Listener for the occurence of LCRunHeader when reading a stream.
 * User classes should implement this interface for their analysis code 
 * and then register the listener with LCReader.
 * 
 * @author gaede 
 * @version Mar 11, 2003
 * @see LCRunHeader
 * @see LCReader
 */

class LCRunListener {

public: 
    /// Destructor.
    virtual ~LCRunListener() { /* nop */; }

    /** Call back for processing an LCRunHeader. Access is 'read only'. 
     */
    virtual void processRunHeader(EVENT::LCRunHeader * rh) = 0;

    /** Call back for modifying an existing LCRunHeader. 
     */
    virtual void modifyRunHeader(EVENT::LCRunHeader * rh) = 0;

//     /** Call back for analyzing an LCRunHeader.
//      * @deprecated - use processRunHeader().
//      */
//     public void analyze(const  LCRunHeader* rh ) ;
//     /** Call back for updating an LCRunHeader.
//      * @deprecated - use modifyRunHeader().
//      */
//     public void update( LCRunHeader* rh ) ;
}; // class
} // namespace IO
#endif /* ifndef IO_LCRUNLISTENER_H */
