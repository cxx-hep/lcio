#ifndef SIO_RunEventMap_H
#define SIO_RunEventMap_H 1

#include <map>

#include "LCIORandomAccess.h"

namespace SIO {
    
/** Map that holds positions of Run and Event records.
 * 
 * @author gaede
 * @version $Id: RunEventMap.h,v 1.1.2.1 2010-04-13 19:35:08 gaede Exp $
 */
  class RunEventMap {
    typedef std::map< long64, long64> MapType ;
    typedef MapType::iterator Map_IT ;
    
  public:
    static const int NPos = -1 ;

    RunEventMap() ; 
    ~RunEventMap() ;
    
    /** Add a new entry to the event map - if the RunEvent already exists the new position will be stored.
     */
    void add(const RunEvent& re, long64 pos ) ;
    
    /** Return the position of the specified Event record or Run record respectively (if EventNum == -1 ).
     *  Returns RunEventMap::NPos if no record found.
     */
    long64 getPosition(const RunEvent& re ) {
      return getPosition( long64( re ) ) ;
    }

    /** Same as getPosition(const RunEvent& re ). 
     */
    long64 getPosition( long64 re ) ; 
   

    int getNumberOfEventRecords() { return _nEvt ; }

    int getNumberOfRunRecords()  { return _nRun ; }

    // map funcitons
    size_t size() { return _map.size()  ; } 
  protected: 

    MapType _map ;

    int _nRun ;
    int _nEvt ;

  }; // class
 } // namespace

#endif /* ifndef SIO_RunEventMap_H */
