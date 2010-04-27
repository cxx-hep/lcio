#include "SIO/LCIORandomAccessMgr.h"

namespace SIO{


  LCIORandomAccessMgr::LCIORandomAccessMgr() {
  }

  LCIORandomAccessMgr::~LCIORandomAccessMgr() {

    // cleanup 
    for( std::list<LCIORandomAccess* >::iterator i = _list.begin() ; i != _list.end() ; ++i ){

      delete *i ; 
    }

  }


  LCIORandomAccess LCIORandomAccessMgr::createFromEventMap() {

    // not needed yet ...
  }
 
  std::ostream& operator<<(std::ostream& os, const LCIORandomAccessMgr& ra ){
    
    os << " LCIORandomAccessMgr:  ----------------------- " << std::endl   ;

    for( std::list<LCIORandomAccess* >::const_iterator i = ra._list.begin() ; i != ra._list.end() ; ++i ){

      os << **i ; 
    }

    os  <<  ra._runEvtMap   << std::endl ;

    return os ;
  }

}



