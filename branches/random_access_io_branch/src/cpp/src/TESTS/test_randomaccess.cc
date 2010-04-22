////////////////////////////////////////
// example for LCIO tests
////////////////////////////////////////

#include "tutil.h"
#include "lcio.h"

// #include "EVENT/LCIO.h"
// #include "IO/LCWriter.h"
// #include "IMPL/LCEventImpl.h" 
// #include "IMPL/LCCollectionVec.h"

#include "SIO/LCIORandomAccess.h"
#include "SIO/LCIORandomAccessMgr.h"
#include "SIO/RunEventMap.h"


#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

using namespace std ;
using namespace lcio ;

using namespace SIO ;

// replace mytest with the name of your test
const static string testname="random_access";

//=============================================================================

int main(int argc, char** argv ){
    
    // this should be the first line in your test
    TEST MYTEST=TEST( testname, std::cout );
    
    
    MYTEST.LOG( "  ----------------------------   testing class RunEvent"  ) ;

    // fill a few RunEvents into a set - test ordering with operator<()

    set< RunEvent > reSet ;
    reSet.insert( RunEvent(  123456, -1 ) )  ;
    reSet.insert( RunEvent(  123456,  1 ) )  ;
    reSet.insert( RunEvent(  123456,  12 ) )  ;
    reSet.insert( RunEvent(  123450, -1 ) )  ;
    reSet.insert( RunEvent(  123450, 112345 ) )  ;

    reSet.insert(  EVENT::long64(123400) << 32   | 12345678  ) ;

    MYTEST( reSet.size() , unsigned(6)  ,  " set< RunEvent>.size() != 6 " )  ;

    std::vector< RunEvent > v ; // copy to vector for easier comparison
    std::copy( reSet.begin() , reSet.end() , std::back_inserter( v ) ) ;

//     int d=-1 ;
//     for( std::set< RunEvent >::iterator it = reSet.begin() ; it != reSet.end() ; ++it ){
//       std::cout << " set[" << ++d <<"] " << *it  << std::endl ;
//     }

    std::stringstream err ;
    err << " v[0] : " << v[0]  << std::endl 
	<< " v[1] : " << v[1]  << std::endl 
	<< " v[2] : " << v[2]  << std::endl 
	<< " v[3] : " << v[3]  << std::endl 
	<< " v[4] : " << v[4]  << std::endl 
	<< " v[5] : " << v[5]  << std::endl  ;

    MYTEST( v[0] < v[1] , true ,  err.str() )  ;
    MYTEST( v[1] < v[2] , true ,  err.str() )  ;
    MYTEST( v[2] < v[3] , true ,  err.str() )  ;
    MYTEST( v[3] < v[4] , true ,  err.str() )  ;
    MYTEST( v[4] < v[5] , true ,  err.str() )  ;


    MYTEST.LOG( "  -------------------------------------   testing class RunEventMap"  ) ;

    RunEventMap map ;

    int count = 0  ;
    for( std::set< RunEvent >::iterator it = reSet.begin() ; it != reSet.end() ; ++it , ++count ){
      
      //  std::cout << " set[" << count <<"] " << *it  << std::endl ;
      map.add( *it , 1024 * count ) ;
    }
    MYTEST( map.getPosition( v[2] ), 2048 ,  "  EventMap::getPosition() return for exisiting event "  )  ;

    MYTEST( unsigned (map.getNumberOfRunRecords() + map.getNumberOfEventRecords() ),
	    map.size(), "RunEventMap invariant: nRun+nEvt = map.size " );

    MYTEST( map.getNumberOfEventRecords(), 4 , "RunEventMap::getNumberOfEventRecords()  " );

    map.add(  v[2] , 2048000 ) ; // overwrite existing entry

    MYTEST( map.getPosition( v[2] ), 2048000 ,  " test if EventMap::add()  overwrites existing entry "  )  ;

    MYTEST( map.getNumberOfEventRecords(), 4 , "RunEventMap::getNumberOfEventRecords() after addition of duplicate (overwrite) " );



    MYTEST.LOG( "  ---------TO DO !!!!!! ---------------------   testing class LCIORandomAccess"  ) ;

    MYTEST( true , true , " LCIORandomAccess " )  ;



    MYTEST.LOG( "  ---------TO DO !!!!!! ---------------------   testing class LCIORandomAccessMgr"  ) ;

    LCIORandomAccessMgr raMgr ;

    raMgr.addRunEventMap( map ) ;
    

    MYTEST( true , true , " LCIORandomAccessMgr" )  ;
  



    //MYTEST( true , true , " LCIORandomAccess " )  ;

    //if( true ){
    if( false ){
        // force test program to fail in this way:
        MYTEST.FAILED( "oops, something went wrong..." );
    }

    // example with a try&catch fail condition:
    //
    ///////////////////////////////////////////////////////////////////////
    //LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;
    //try{
    //    lcReader->open( "blub.slcio" ) ;
    //}
    //catch( Exception &e ){
    //    MYTEST.FAILED( e.what() );
    //}
    //lcReader->close();
    //delete lcReader;
    ///////////////////////////////////////////////////////////////////////

    return 0;
}

//=============================================================================

