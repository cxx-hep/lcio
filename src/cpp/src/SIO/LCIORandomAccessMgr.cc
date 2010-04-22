#include "SIO/LCIORandomAccessMgr.h"

namespace SIO{


  LCIORandomAccessMgr::LCIORandomAccessMgr() :
    _minRunEvt(0) ,
    _maxRunEvt(0) ,
    _nRunHeaders(0) ,
    _nEvents(0) {
  }

  LCIORandomAccess LCIORandomAccessMgr::createFromEventMap() {

    

  }
 
 std::ostream& operator<<(std::ostream& os, const LCIORandomAccessMgr& ra ){

    os << " LCIORandomAccessMgr:  ----------------------- " << std::endl  
       << " minRun " <<  ra._minRunEvt.RunNum << std::endl
       << " minEvt " <<  ra._minRunEvt.EvtNum  << std::endl
       << " maxRun " <<  ra._maxRunEvt.RunNum << std::endl  
       << " maxEvt " <<  ra._maxRunEvt.EvtNum << std::endl  
      //       << " nRunHeaders " <<  ra._nRunHeaders << std::endl 
       << " nEvents " <<  ra._nEvents  << std::endl
      //       << " recordsAreInOrder " <<  ra._recordsAreInOrder  << std::endl  
      //        << " indexLocation " <<  ra._indexLocation  << std::endl
      //        << " prevLocation " <<  ra._prevLocation  << std::endl
      //        << " nextLocation " <<  ra._nextLocation  << std::endl
      //        << " firstRecordLocation " <<  ra._firstRecordLocation  << std::endl
       << "---------------------------------------------"  

       <<  ra._runEvtMap 

       << std::endl ;

    return os ;
  }

}



