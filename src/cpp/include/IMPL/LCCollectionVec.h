#ifndef EVENT_LCCOLLECTIONVEC_H
#define EVENT_LCCOLLECTIONVEC_H 1

#include <string>
#include <vector>

#include "EVENT/LCCollection.h"
#include "EVENT/LCObject.h"
#include "AccessChecked.h"
#include "LCParametersImpl.h"
#include "EVENT/LCIO.h"
#include "LCIOTypes.h"

#include <iostream>


namespace IMPL {
  
  /** Implementation of the LCCollection using (inheriting from) an STL vector
   *  of LCObjects.
   * 
   * @author gaede 
   * @version $Id: LCCollectionVec.h,v 1.11.20.4 2009-12-01 11:11:03 gaede Exp $
   * @see LCObject
   * @see LCCollection
   */

  template <class T>
  class LCCollectionTVec : public EVENT::LCCollection, public AccessChecked {
    
    
    //  protected:
  public:
    /**  Default Constructor is protected  - every LCCollection needs to know the type
     *   of its elements.
     */
    LCCollectionTVec() {  /* no default c'tor */ }
    
  public:
    
    /** The public default constructur that takes the name of the type of the elements.     */
    LCCollectionTVec( const std::string& type ) :
      _typeName( type ),
      _flag(0) {
    }
    
    /* Coppy constructor creating a deep copy of an LCCollection.
     */
    //    LCCollectionTVec( const EVENT::LCCollection& col ) ;
    
    
    /** Destructor.
     */    
    virtual ~LCCollectionTVec() {
      
      if( ! isSubset() ){
	// delete all elements
	typename std::vector<T*>::const_iterator iter = _vec.begin() ;
	//    std::cout << "deleting collection " 
	//  	    << std::endl ;
	//    UTIL::LCTOOLS::printParameters( parameters() )  ;
	
	while( iter != _vec.end() ){
	  delete *iter++ ;
	}
      }
    }
    
    /**Returns the number of entries in the collection.
     */
    virtual int getNumberOfElements() const {
      return _vec.size() ;
    }

    /** Returns the type name of the collection - valid names are defined in LCIO.
     */
    virtual const std::string & getTypeName() const {
      return _typeName ;
    }
    
    /** Returns pointer to element at index - no range check !.
     */
    virtual EVENT::LCObject * getElementAt(int index) const {
      return _vec.operator[](index) ;
    }
    
    /** Returns flag word for collection. Bits 16-31 are reserved for LCIO
     *  Depending on the object type stored they have a special meaning, e.g. 
     *  for SimCalorimeterHits: <br>
     *  CHBIT_LONG = 31   -  store position <br>
     *  CHBIT_BARREL = 30 -  endcap or barrel <br>
     *  CHBIT_ID1 = 29 -   cellid1 is sored <br>
     *  CHBIT_PDG = 28 - store pdg of secondaries <br>
     *  &nbsp;<br>
     *  Bit 16 is used to flag collection as transient <br>
     *  Bit 17 is used to flag collection as default <br>
     *  Bit 18 is used to flag collection as subset <br>
     *  Bits 0-15 are subdetector/user specific.
     * @see isTransient()
     */
    virtual int getFlag() const {
      return _flag ;
    }

    /** True if collection is transient, i.e. will not be written to any LCIO file.
     *  Convenient method that checks bit 16 of the flag word.
     */
    virtual bool isTransient() const { 
      return (_flag & (1<<BITTransient) ) ;
    }
    

    /** Sets the transient flag for this collection. Transient collections are not written
     * to LCIO files.
     */
    void setTransient(bool val=true)  {
      if(val) _flag |= (1<<BITTransient) ;
      else _flag &= ~(1<<BITTransient) ;
    }


    /** True if collection is the default collection for the given type.
     *  This implies that the collection is complete and unambigous.
     *  Convenient method that checks bit BITDefault of the flag word.
     */
    virtual bool isDefault() const  { 
      return (_flag & (1<<BITDefault) ) ; 
    }

    /** Sets the default flag for this collection. User need to make sure this 
     * flag is unique for a type.
     */
    void setDefault(bool val=true)  {
      if(val) _flag |= (1<<BITDefault) ;
      else _flag &= ~(1<<BITDefault) ;
    }

    /** True if the collection holds a subset of objects from other collections. 
     *  If the collection is not transient only the pointers/references to the original
     *  objects will be stored.
     *  Convenient method that checks bit BITSubset of the flag word.
     */
    virtual bool isSubset() const { 
      return (_flag & (1<<BITSubset) ) ; 
    }

    /** Sets the subset flag for this collection. Collections of subsets are only 
     *  written as pointers to LCIO files, i.e. the objects have to exist in 
     *  another non-transient collections.<br>
     *  NB: Do not add any 'real' objects to a collection flagged as subset, as 
     *  this will lead to memory leaks !
     */
    void setSubset(bool val=true)  {
      if(val) _flag |= (1<<BITSubset) ;
      else _flag &= ~(1<<BITSubset) ;
    }

    /** Sets the flag word for this collection.
     */
    void setFlag(int flag) {
      _flag  = flag ;
    }

    /** Adds the given element to (end of) the collection. Throws an exception 
     * if the collection (event) is 'read only'.
     *
     * @throws ReadOnlyException
     */
    virtual void addElement(EVENT::LCObject * obj) throw (EVENT::ReadOnlyException) {
      //    if(_access != LCIO::UPDATE )
      //  throw ReadOnlyException("LCCollectionVec::addElement:  event is read only") ;
      checkAccess("LCCollectionVec::addElement") ;
      _vec.push_back( (T*) obj ) ; 
    }


    /** Removes the i-th element from the collection. Throws an exception 
     * if the collection (event) is 'read only'.
     *
     * @throws ReadOnlyException
     */
    virtual void removeElementAt(int i) throw (EVENT::ReadOnlyException) {
      //    if(_access != LCIO::UPDATE )
      //  throw ReadOnlyException("LCCollectionVec::addElement:  event is read only") ;
      checkAccess("LCCollectionVec::removeElementAt") ;
      _vec.erase( _vec.begin() + i ) ;
    }
    
    /** Parameters defined for this run.
     */
    virtual const EVENT::LCParameters & getParameters() const { return _params ; } 

    /** Parameters defined for this run.
     */
    virtual EVENT::LCParameters & parameters() { return _params ; } 
    
    /**Helper function to convert object addresses into indices for all objects:  ((hash<<32)||index) */
    void setIndices( unsigned hash ) {
      
      for( unsigned i=0 ; i< _vec.size() ; ++i ){
	
	EVENT::long64 idx(i) ;
	EVENT::long64 hashL( hash ) ;
	
	//       EVENT::long64 index(  idx | hashL<<32 ) ;
	//       std::cout << "   LcCol setIndices : " << idx << " , " << hashL << ", " <<  index  << std::endl ;
	//	_vec.operator[](i)->setIndex(  idx | hashL<<32 ) ;
	_vec[i]->setIndex(  idx | hashL<<32 ) ;
      }
    }

    /** Calls ptrToIndex for all elements */
    void ptrToIndex() {
      for( unsigned i=0 ; i< _vec.size() ; ++i ){
	_vec.operator[](i)->ptrToIndex() ;
      }
    }

    // forward some of std::vector's interface so that we can serve as a wrapper to vector ------------

    inline void push_back(EVENT::LCObject* t){
      _vec.push_back( (T*) t) ;
    }
    inline void resize(size_t nS){   _vec.resize(nS) ; }
    inline EVENT::LCObject* operator[](size_t n){  return _vec[n] ; }
    inline const EVENT::LCObject* operator[](size_t n) const { return _vec[n] ; }
    inline EVENT::LCObject* at(size_t n){ return _vec.at(n)  ; }
    inline size_t size() const { return _vec.size()  ; }
//     inline iterator begin(){  return _vec.begin() ; }
//     inline iterator end(){ return _vec.end() ; }
//     inline const_iterator begin() const {  return _vec.begin() ; }
//     inline const_iterator end() const { return _vec.end() ; }



  protected:
    void setReadOnly(bool readOnly){
      
      AccessChecked::setReadOnly(readOnly ) ;
      
      typename std::vector<T*>::iterator iter = _vec.begin() ;
      while( iter != _vec.end() ){
	AccessChecked* element = dynamic_cast<AccessChecked*>(*iter++) ;
	if(element){
	  element->setReadOnly( readOnly ) ;
	}
      }
    }
    
    std::string _typeName ;
    int _flag ;
    LCParametersImpl _params ;
    //    int _access ;

    //    EVENT::LCObjectVec _vec; 
    std::vector< T*> _vec ;

}; // class

  
  typedef LCCollectionTVec< EVENT::LCObject > LCCollectionVec ;

} // namespace IMPL
#endif /* ifndef EVENT_LCCOLLECTIONVEC_H */
