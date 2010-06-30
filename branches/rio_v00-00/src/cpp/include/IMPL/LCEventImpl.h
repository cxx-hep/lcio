#ifndef IMPL_LCEventImpl_H
#define IMPL_LCEventImpl_H 1

#include <string>
#include <map>
#include <set>
#include "EVENT/LCEvent.h"
#include "EVENT/LCCollection.h"
// #include "EVENT/LCRelation.h"
#include "EVENT/LCIO.h"
#include "LCIOTypes.h"
#include "AccessChecked.h"
#include "LCParametersImpl.h"

namespace IMPL{
  
  //class  EVENT::LCCollection ;
  
  //  typedef std::map<std::string,EVENT::LCCollection*> LCCollectionMap ; 
  typedef std::map<unsigned,EVENT::LCCollection*> LCCollectionMap ; 

  typedef std::set<EVENT::LCCollection*> LCCollectionSet ;
  //   typedef std::map<std::string,EVENT::LCRelation*> LCRelationMap ; 
  
  /**Implementation of the main event class. This is used by  
   * LCIO for reading events from file.
   * 
   * @author gaede
   * @version Mar 5, 2003
   * @see LCEvent
   * @see LCCollection
   */
  class LCEventImpl : public EVENT::LCEvent, public AccessChecked {
    
  public: 
    LCEventImpl() ;

    /** Copy contructor only copies the event header - the collectionds have to be copied manually as needed.
     */
    LCEventImpl(const LCEventImpl& evt) ;  
    
    /**Destructor.
     */
    virtual ~LCEventImpl() ; 
    
    /** Return the run number off this event.
     */
    virtual int getRunNumber() const ;
    
    /** Returns this event's number .
     */
    virtual int getEventNumber() const ;
    
    /** Returns the name of the detector setup used in the simulation.
     */
    virtual const std::string & getDetectorName() const ;
    
    /** Returns the time stamp of the event.
     */
    virtual EVENT::long64 getTimeStamp() const ;
    
    /** Returns the event weight.
     */
    virtual double getWeight() const  ;

    /** Returns the names of the collections in the  event.
     */
    virtual const std::vector<std::string>  * getCollectionNames() const;
    
    virtual const std::vector<std::string>  * getCollectionNames(bool refresh) const;

    /** Returns the collection for the given name.
     *
     * @throws DataNotAvailableException
     */
    virtual EVENT::LCCollection * getCollection(const std::string & name) const 
     throw (EVENT::DataNotAvailableException, std::exception) ;

    /** Returns the collection for the given name and transfers the ownership of the collection
     *  to the caller. The caller is responsible for deleting the collection _after_ the Event is 
     *  deleted. The collection is still in the event but flagged as transient 
     *  after the call returns.<br>
     *  This is usefull when you want to keep the collection for the next events.<br>
     *  Use with care!
     * @throws DataNotAvailableException
     */
    virtual EVENT::LCCollection * takeCollection(const std::string & name) const 
      throw (EVENT::DataNotAvailableException, std::exception ) ;

    /** Adds a collection with the given name (has to be a valid C/C++ variable name). 
     *	Throws an exception if the name already
     *  exists in the event. NB: Adding collections is allowed even when the event is 'read only'.
     *
     *@see validateCollectionName
     *@throws EventException
     */ 
    virtual void addCollection(EVENT::LCCollection * col, const std::string & name)  
      throw (EVENT::EventException, std::exception) ;
    
    /** Removes (and deletes) the collection with name (if it exists in the event). 
     * Throws an exception if the event is 'read only' as defined by the read mode in LCReader.
     *
     *@throws ReadOnlyException
     */ 
    virtual void removeCollection(const std::string & name) throw (EVENT::ReadOnlyException, std::exception)  ;
    

    /** Parameters defined for this event.
     */
    virtual const EVENT::LCParameters & getParameters() const { return _params ; }

    /** Parameters defined for this run.
     */
    virtual EVENT::LCParameters & parameters() { return _params ; }

    //---- set methods -----
    /** Sets the run number.
     */
    void setRunNumber( int rn ) ;
    
    /** Sets the event number.
     */
    void setEventNumber( int en ) ;
    
    /** Sets the detector name.
     */
    void setDetectorName(const std::string& dn ) ;
    
    /** Sets the event time stamp.
     */
    void setTimeStamp(EVENT::long64 ts) ;
       
    /** Set the event weight.
     */
    void setWeight(double w) ;
     
    static LCEventImpl* getCurrentEvent() { return _current ; }
    EVENT::LCObject* getObjectForIndex(EVENT::long64 index) ;
    void setCurrentEvent( LCEventImpl* evt) { _current = evt ; } 
    void ptrToIndex() ;


  protected:
    void setAccessMode( int accessMode ) ;
    
    /**Tests the validity of a collection name. LCIO only accepts collection names starting with 
     * (regular expression) [A-Za-z_] and continuing with [A-Za-z0-9_] (C/C++ variable name).
     */
    bool validateCollectionName( const char* name ) ;
    
  // data members - declared protected to be accessible 
    // for friends of sub classes ...
    
  protected:  
    int _runNumber ;
    int _eventNumber ;
    EVENT::long64 _timeStamp ; 
    std::string _detectorName ;
    
    // map has to be defined mutable in order to use _map[]  for const methods ...
    mutable LCCollectionMap _colMap ;         //! no RIO
    mutable std::vector<std::string> _colNames ;
    
    LCParametersImpl _params ;
    
    // set of collections that are not owned by the event anymore
    mutable LCCollectionSet _notOwned ;   //! no RIO
    
    std::pair< unsigned , EVENT::LCCollection* > _cache ; //! no RIO

    static LCEventImpl* _current ;
    
  }; // class

} // namespace IMPL

#endif /* ifndef EVENT_LCEVENTIMPL_H */