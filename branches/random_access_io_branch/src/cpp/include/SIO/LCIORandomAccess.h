#ifndef SIO_LCIORandomAccess_H
#define SIO_LCIORandomAccess_H 1

//#include "EVENT/LCObject.h"
#include "LCIOTypes.h"

#include <iostream>


typedef EVENT::long64 long64 ;

struct RunEvent{
  RunEvent(): RunNum( 0 ), EvtNum( 0 ) {}
  RunEvent(int run, int evt): RunNum( run ), EvtNum( evt ) {}
  RunEvent(long64 runEvt): RunNum( (runEvt >> 32 ) & 0xffffffff  ), EvtNum( runEvt &  0xffffffff ) {}
  int RunNum ;
  int EvtNum ;

  operator long64() const {  return  ( long64( RunNum ) << 32  |  ( long64(EvtNum) & 0xffffffff )  ) ; } 
  //bool operator < ( const RunEvent& other) {  return ( RunNum <= other.RunNum && EvtNum < other.EvtNum ) ; } 
};

std::ostream & operator<<(std::ostream& os, const RunEvent& re ) ;

bool operator < ( const RunEvent& r0, const RunEvent& other)  ;



namespace SIO{ // IO or IMPL ?

  class SIORandomAccessHandler ;
  class LCIORandomAccess ;
  class LCIORandomAccessMgr ;
  
  std::ostream & operator<<(std::ostream& os, const LCIORandomAccess& ra ) ;

  bool operator < (const LCIORandomAccess ra0, const LCIORandomAccess& other)  ;


/**  Implementation class for LCIORandomAccess records.
 *
 * @author gaede
 * @version $Id: LCIORandomAccess.h,v 1.1.2.4 2010-04-27 12:17:58 gaede Exp $
 */
//  class LCIORandomAccess : public EVENT LCObject {
  class LCIORandomAccess {
    
    friend class SIORandomAccessHandler ;
    friend class LCIORandomAccessMgr ;
    friend std::ostream & operator<<(std::ostream& os, const LCIORandomAccess& ra ) ;
    friend bool operator < (const LCIORandomAccess ra0, const LCIORandomAccess& other)  ;

  public:
    
    virtual ~LCIORandomAccess(){ /* nop */; }
    
    long64 getIndexLocation() const { return _indexLocation ; }
    long64 getPrevLocation() const  { return _prevLocation ; }
    long64 getNextLocation() const  { return _nextLocation ; }
    long64 getFirstRecordLocation() const  { return _firstRecordLocation ; }

  protected:
    RunEvent _minRunEvt ;
    RunEvent _maxRunEvt ;
    int  _nRunHeaders ;
    int  _nEvents ;
    int  _recordsAreInOrder ;  
    long64  _indexLocation ;
    long64  _prevLocation ;
    long64  _nextLocation ;
    long64  _firstRecordLocation ;
  }; // class



} // namespace 
#endif 
