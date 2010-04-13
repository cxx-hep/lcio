#include "SIO/LCIORandomAccessMgr.h"


namespace SIO{

    RunEvent _minRunEvt ;
    RunEvent _maxRunEvt ;
    int  _nRunHeaders ;
    int  _nEvents ;

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
       << std::endl ;

    return os ;
  }

}



