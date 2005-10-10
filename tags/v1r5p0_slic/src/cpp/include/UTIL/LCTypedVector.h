#ifndef LCTypedVector_h
#define LCTypedVector_h 1

#include <vector>

#include "EVENT/LCObject.h"
#include "EVENT/LCCollection.h"

#include <sstream>
#include <typeinfo>


namespace UTIL{


  /** Templated helper class that is an std::vector<T*> of the elements in the LCCollection.
   *  This can be used, e.g. if the objects are to be passed to an algorithm in terms of
   *  iterators. For example:<p>
   *   &nbsp; LCTypedVector<SimTrackerHits> sim( evt->getCollection("tpc") ) ;<br>
   *   &nbsp; LCCollectionVec* trackerHits = new LCCollectionVec( LCIO::TRACKERHIT )  ;<br>
   *   &nbsp; std::transform( sim.begin(), sim.end(), std::back_inserter( *trackerHits ) , digitizer ) ;
   *  <p>Note that instantiating an LCTypedVector involves copying and casting of the pointers in 
   *  the collection. In a future release of LCIO this might not be neccessary any more.
   *  @author gaede
   *  @version $Id: LCTypedVector.h,v 1.1 2005-05-27 07:55:54 gaede Exp $
   */
  template <class T> 
  class LCTypedVector : public  std::vector<T*> {
    
  public:  
    
    LCTypedVector( LCCollection* col ) : _col( col) {
      
      resize( _col->getNumberOfElements() ) ;
      
      for(int i=0;i<_col->getNumberOfElements();i++ ) {
	
	(*this)[i] = dynamic_cast<T*>( col->getElementAt(i) ) ;
	
	// check the first element for the proper type
	if( i == 0 && (*this)[i] == 0  ){

	  std::stringstream str ;
	  str << "LCTypedVector: cannot convert " << col->getTypeName() << " to " 
	      << typeid(T).name()  ;

	  throw Exception(  str.str() ) ; 
	}
      }
    }
    
    LCCollection* col() { return _col ; }
    
  protected:
    LCCollection* _col ;
  } ; 
  
} // namespace
#endif


