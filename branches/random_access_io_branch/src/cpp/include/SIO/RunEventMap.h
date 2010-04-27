#ifndef SIO_RunEventMap_H
#define SIO_RunEventMap_H 1

#include <map>

#include "LCIORandomAccess.h"

namespace SIO {
  class RunEventMap ; 
  std::ostream & operator<<(std::ostream& os, const RunEventMap& rm ) ;


/** Map that holds positions of Run and Event records.
 * 
 * @author gaede
 * @version $Id: RunEventMap.h,v 1.1.2.3 2010-04-27 12:17:58 gaede Exp $
 */
  class RunEventMap {

    typedef std::map< RunEvent, long64> MapType ;

    typedef MapType::iterator Map_IT ;
    typedef MapType::const_iterator Map_cIT ;

    friend std::ostream & operator<<(std::ostream& os, const RunEventMap& rm ) ;
   
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

    Map_IT begin()  { return _map.begin() ; }
    Map_IT end()  { return _map.end() ; }
    Map_cIT begin() const  { return _map.begin() ; }
    Map_cIT end()  const { return _map.end() ; }

    template <class InputIterator>
    void insert(InputIterator b, InputIterator e) { _map.insert( b , e  ) ; } 

  protected: 

    MapType _map ;

    int _nRun ;
    int _nEvt ;

  }; // class
 } // namespace

#endif /* ifndef SIO_RunEventMap_H */
