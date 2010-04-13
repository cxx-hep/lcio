#ifndef SIO_LCIORandomAccessMgr_H
#define SIO_LCIORandomAccessMgr_H 1

#include "LCIORandomAccess.h"
#include "RunEventMap.h"

#include <iostream>
#include <map>
#include <list>


namespace SIO{ // IO or IMPL ?

  class LCIORandomAccessMgr ;
  class SIOIndexHandler ;
  class SIORandomAccessHandler ;

  std::ostream & operator<<(std::ostream& os, const LCIORandomAccessMgr& ra ) ;

/**  Manager class for LCIORandomAccess objects and direct access
 *
 * @author gaede
 * @version $Id: LCIORandomAccessMgr.h,v 1.1.2.1 2010-04-13 19:35:08 gaede Exp $
 */

  class LCIORandomAccessMgr {
    
    friend class SIOIndexHandler ;
    friend class SIORandomAccessHandler ;
    friend std::ostream & operator<<(std::ostream& os, const LCIORandomAccessMgr& ra ) ;

  public:
    
    virtual ~LCIORandomAccessMgr(){ /* nop */; }
    
  protected:
    RunEvent _minRunEvt ;
    RunEvent _maxRunEvt ;
    int  _nRunHeaders ;
    int  _nEvents ;


    //    int  _recordsAreInOrder ;  

//     long64  _indexLocation ;
//     long64  _prevLocation ;
//     long64  _nextLocation ;
//     long64  _firstRecordLocation ;


    RunEventMap _runEvtMap ;

    std::list< LCIORandomAccess* > _list ;

  }; // class

  


} // namespace 
#endif 
