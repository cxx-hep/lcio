// -*- C++ -*-
#ifndef EVENT_LCWGTRELATIONIMPL_H
#define EVENT_LCWGTRELATIONIMPL_H 1

#include <vector>
#include <map>

#include "EVENT/LCWgtRelation.h"
#include "EVENT/LCObject.h"
#include "AccessChecked.h"



namespace IMPL {
  
  /** Implementation of the LCWgtRelation.
   * 
   * @author gaede 
   * @version $id: $
   */
  class LCWgtRelationImpl : public EVENT::LCWgtRelation, public AccessChecked {
    
    public:

    LCWgtRelationImpl() : _from(0) , _to(0) , _weight(1.0f) { }
    LCWgtRelationImpl( EVENT::LCObject* from, EVENT::LCObject* to , float weight=1.0f ) : _from(from),
											  _to(to),
											  _weight(weight) {}
    ~LCWgtRelationImpl(){}

    int id() { return simpleUID() ; }

    EVENT::LCObject * getFrom() const { return _from ; }
    EVENT::LCObject * getTo() const { return _to ; } 
    float getWeight() const { return _weight ; } 
    
    void setFrom( EVENT::LCObject* from ) { _from = from ; }
    void setTo( EVENT::LCObject* to ) { _to = to ; }
    void setWeight( float weight ) { _weight = weight ; }
    

  protected:
    EVENT::LCObject* _from ;
    EVENT::LCObject* _to ;
    float _weight ;

}; // class
}; // namespace IMPL
#endif /* ifndef EVENT_LCRELATIONIMPL_H */
