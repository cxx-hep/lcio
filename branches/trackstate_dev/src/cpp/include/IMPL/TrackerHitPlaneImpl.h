// -*- C++ -*-
#ifndef IMPL_TrackerHitPlaneImpl_H
#define IMPL_TrackerHitPlaneImpl_H 1

#include <string>

#include "EVENT/TrackerHitPlane.h"
#include "IMPL/AccessChecked.h"

#define TRKHITNCOVMATRIX 6

namespace IMPL {

/** Implementation of the planar tracker hit. 
 * 
 * @author gaede
 * @version $Id: $
 */

  class TrackerHitPlaneImpl : public EVENT::TrackerHitPlane , public AccessChecked {

  public: 
    // C'tor
    TrackerHitPlaneImpl() ;
    
    /// Destructor.
    virtual ~TrackerHitPlaneImpl() ; 


    virtual int id() const { return simpleUID() ; }

    /** The hit  position in [mm].	
     */
    virtual const double* getPosition() const  {  return _pos ; }  ;

    /** Direction of first measurement - given as (theta, phi).
     *  Defines spanning vector of measurement plane.	
     */
    virtual const float* getU() const { return _u ; }

    /** Direction of second measurement - given as (theta, phi).
     *  Defines spanning vector of measurement plane.	
     */
    virtual const float* getV() const  { return _v ; }

    /** Error along u */
    virtual float getdU() const { return _du ; }

    /** Error along v */
    virtual float getdV() const  { return _dv ; }


    /**Covariance of the position (x,y,z)
     */
    virtual const EVENT::FloatVec & getCovMatrix() const ;

    /** The dE/dx of the hit in [GeV/mm].
     *  @deprecated
     *  @see getEDep()
     */ 	
    virtual float getdEdx() const ;

    /** The deposited energy of the hit [GeV]
     */
    virtual float getEDep() const { return _EDep ; }

    /** The error measured on EDep [GeV]
     */
    virtual float getEDepError() const { return _EDepError ; }

    /** The  time of the hit in [ns]. Is this needed ?
     */
    virtual float getTime() const  { return _time ; } ;

    /** Type of hit. Mapping of integer types to type names
     * through collection parameters "TrackerHitTypeNames"
     * and "TrackerHitTypeValues".
     */
    virtual int getType() const { return _type ; }


    /** The quality bit flag of the hit.
     */
    virtual int getQuality() const { return _quality ; }

    /** The raw data hits. 
     * Check getType() to get actual data type.
     */
    virtual const EVENT::LCObjectVec & getRawHits() const { return _rawHits ; }


    /** Use to manipulate the raw hits.
     */
    virtual EVENT::LCObjectVec & rawHits() { return _rawHits ; }


    // setters 
    void setType(int type) ;
    void setPosition( double pos[3]) ;
    void setU( float u[2] ) { setU( u[0] , u[1] ) ; }
    void setU( float theta, float phi);
    void setV( float v[2] ) { setV( v[0] , v[1] ) ; };
    void setV( float theta, float phi);
    void setdU( float du );
    void setdV( float dv );
    void setEDep( float e ) ;
    void setEDepError( float e ) ;
    void setTime( float t ) ;
    void setQuality( int quality ) ;
    void setQualityBit( int bit , bool val=true ) ;


protected:
  
    int    _type ;
    double  _pos[3] ;
    float  _u[2] ;
    float  _v[2] ;
    float  _du ;
    float  _dv ;
    mutable EVENT::FloatVec _cov ;
    float _EDep ;
    float _EDepError ;
    float _time ;
    int _quality ;
    EVENT::LCObjectVec _rawHits ;
    

}; // class
} // namespace IMPL
#endif // #ifndef IMPL_TrackerHitPlaneImpl_H 
