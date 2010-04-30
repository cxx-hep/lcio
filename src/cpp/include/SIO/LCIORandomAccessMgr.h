#ifndef SIO_LCIORandomAccessMgr_H
#define SIO_LCIORandomAccessMgr_H 1

#include "LCIORandomAccess.h"
#include "RunEventMap.h"

#include <iostream>
#include <map>
#include <list>

class SIO_stream ;

namespace SIO{ // IO or IMPL ?

  class LCIORandomAccessMgr ;
  class SIOIndexHandler ;
  class SIORandomAccessHandler ;
   
  std::ostream & operator<<(std::ostream& os, const LCIORandomAccessMgr& ra ) ;

/**  Manager class for LCIORandomAccess objects and direct access
 *
 * @author gaede
 * @version $Id: LCIORandomAccessMgr.h,v 1.1.2.4 2010-04-30 21:30:52 gaede Exp $
 */

  class LCIORandomAccessMgr {
    
    friend class SIOIndexHandler ;
    friend class SIORandomAccessHandler ;
    friend std::ostream & operator<<(std::ostream& os, const LCIORandomAccessMgr& ra ) ;

  public:

    LCIORandomAccessMgr() ;
    
    virtual ~LCIORandomAccessMgr() ;
 
    //    void addRunEventMap( const RunEventMap& reMap)  {  _runEvtMap.insert( reMap.begin() , reMap.end()  ) ; }
    
    RunEventMap& map() {  return _runEvtMap ;  } 

    LCIORandomAccess* createFromEventMap() ;
    
    const LCIORandomAccess* lastLCIORandomAccess() {

      return (_list.empty() ?  0 : _list.back() )  ; 
    } 

    void addLCIORandomAccess( LCIORandomAccess* ra ) { _list.push_back( ra ) ;  }
    

    //    bool readLastLCIORandomAccess( SIO_stream* stream ) ;

    bool readLCIORandomAccessAt( SIO_stream* stream , long64 pos) ;

    bool readLCIOIndexAt( SIO_stream* stream , long64 pos) ;

  protected:

    bool readLCIORandomAccess( SIO_stream* stream ) ;

    bool readLCIOIndex( SIO_stream* stream ) ;
   
    RunEventMap _runEvtMap ;
    
    std::list< LCIORandomAccess* > _list ;
    
  }; // class
  
  
  
  
} // namespace 
#endif 
