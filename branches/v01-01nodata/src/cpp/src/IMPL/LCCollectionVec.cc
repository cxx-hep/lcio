
#include "IMPL/LCCollectionVec.h"
#include "EVENT/LCIO.h"
#include <iostream>

using namespace EVENT ;

namespace IMPL {

//     std::string _typeName ;
//     int _flag ;


LCCollectionVec::LCCollectionVec( const std::string& type ) :
  _typeName( type ),
  _flag(0){ 
  //,
  // _access(LCIO::UPDATE ) {

}
  
// LCCollectionVec::LCCollectionVec( const EVENT::LCCollection& col ) :
//   _typeName( col.getTypeName() ),
//   _flag( col.getFlag() ),
//   _access( col._access ) {

//   // deep copy of all elements  - requires clone of original elements
//   //
//   int nElements = col.getNumberOfElements() ;

//   for(int i=0; i< nElements ; i++){
//     push_back(  col.getElementAt( i )->clone() ) ;
//   }

// }

  // overwrite the default implementation
  // set flag in all elements
  void LCCollectionVec::setReadOnly(bool readOnly){

    AccessChecked::setReadOnly(readOnly ) ;

    LCObjectVec::const_iterator iter = begin() ;
    while( iter != end() ){
      AccessChecked* element = dynamic_cast<AccessChecked*>(*iter++) ;
      if(element){
	element->setReadOnly( readOnly ) ;
      }
    }
  }

LCCollectionVec::~LCCollectionVec() {

  // delete all elements
  LCObjectVec::const_iterator iter = begin() ;
  while( iter != end() ){
    delete *iter++ ;
  }

}


int LCCollectionVec::getNumberOfElements() const{
  return size() ;
}


const std::string & LCCollectionVec::getTypeName() const{
  return _typeName ;
}



EVENT::LCObject * LCCollectionVec::getElementAt(int index) const{
  return this->operator[](index) ;
}


int LCCollectionVec::getFlag() const{
  return _flag ;
}


void LCCollectionVec::setFlag(int flag){
  _flag  = flag ;
}


  void LCCollectionVec::addElement(EVENT::LCObject * obj) throw (ReadOnlyException){
    
    //    if(_access != LCIO::UPDATE )
    //  throw ReadOnlyException("LCCollectionVec::addElement:  event is read only") ;
    checkAccess("LCCollectionVec::addElement") ;
    this->push_back( obj ) ; 
  }

  void LCCollectionVec::removeElementAt(int i) throw (EVENT::ReadOnlyException){
    
    //    if(_access != LCIO::UPDATE )
    //  throw ReadOnlyException("LCCollectionVec::addElement:  event is read only") ;
    checkAccess("LCCollectionVec::removeElementAt") ;
    this->erase( begin() + i ) ;

  }

}; // namespace IMPL

