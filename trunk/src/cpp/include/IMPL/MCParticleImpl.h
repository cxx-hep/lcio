// -*- C++ -*-
#ifndef EVENT_MCPARTICLEIMPL_H
#define EVENT_MCPARTICLEIMPL_H 1
#include <vector>

#include "EVENT/LCIO.h"
#include "AccessChecked.h"

#include "EVENT/MCParticle.h"


namespace IMPL {
  
  typedef std::vector<EVENT::MCParticle**> MCParticlePVec ;
  // use  pointers to pointer to MCParticle 
  // as SIO needs the address of the pointer for pointer reallocation....

  /** Implementation of MCParticle.
   * 
   * @author gaede
   * @version Mar 5, 2003
   */
  class MCParticleImpl : public EVENT::MCParticle, public AccessChecked {
    
  public: 

    MCParticleImpl() ;
    
    /// Destructor.
    virtual ~MCParticleImpl() ;

    /** Returns the parent of this particle. Null if the mother (beam particle).
     * Same as getParentData() except for type and exception.
     *
     * @throws DataNotAvailableException
     */
    virtual EVENT::MCParticle * getParent() const throw (EVENT::DataNotAvailableException, std::exception) ;

    /** Returns the second parent of this particle. 
     * Same as getSecondParentData() except for type and exception.
     *
     * @throws DataNotAvailableException
     */
    virtual EVENT::MCParticle * getSecondParent() const throw (EVENT::DataNotAvailableException, std::exception) ;

    /** Returns the i-th daughter of this particle.
     * Same as getDaughterData() except for type and exception - applies range check.
     *
     * @throws DataNotAvailableException
     * @see getNumberOfDaughters
     */
    virtual EVENT::MCParticle * getDaughter(int i) const throw (EVENT::DataNotAvailableException, std::exception) ;


    /** Returns the parent of this particle. Null if the mother (beam particle).
     */
    virtual DATA::MCParticleData * getParentData() const ;

    /** Returns the second parent of this particle, usually Null.
     */
    virtual DATA::MCParticleData * getSecondParentData() const ;


    /** Returns the i-th daughter of this particle.
     *  Unchecked access to vector holding daughters, thus faster than getDaughter(int i).
     */
    virtual DATA::MCParticleData * getDaughterData(int i) const ;


    /** Returns the endpoint of the particle in [mm].
     *  Definition of the enpoint depends on the application that created 
     *  the particle, e.g. the start point of the shower in a calorimeter.
     *  If the particle has daughters, the corresponding vertex is returned.
     *  Never returns NULL.
     */
    virtual const double* getEndpoint() const ;

    /** Returns the number of daughters of this particle.
     */
    virtual int getNumberOfDaughters() const ;


    /** Returns the PDG code of the particle.
     */
    virtual int getPDG() const ;

    /** Returns the status for particles from the generator (HepEvt)
     * and for particles added in simulation:
     * <br> 0  empty line
     * <br> 1  undecayed particle, stable in the generator
     * <br> 2  particle decayed in the generator
     * <br> 3  documentation line
     * <br> 201 stable state, but created in the tracking in the detector
     * <br> 202 particle decayed in the tracking in the detector
     */
    virtual int getHepEvtStatus() const ;

    /** Returns the production vertex of the particle.
     */
          
    virtual const double * getVertex() const ;

    /** Returns the particle momentum at the production vertex.
     */
          
    virtual const float * getMomentum() const ;

    /** Returns the mass of the particle in [GeV].
     */
    virtual float getMass() const ;

    /** Returns the particle's charge.
     */
    virtual float getCharge() const ;


    /** Returns the energy of the particle (at the vertex) in [GeV] computed from
     * the particle's momentum and mass.
     */
    virtual float getEnergy() const ;

    // set methods
    /** Sets the parent. 
     */
    void setParent(  EVENT::MCParticle *mom0 ) ;

    /** Sets a second parent.
     */
    void setSecondParent(  EVENT::MCParticle *mom1 ) ;

    /** Adds a daughter particle.
     */

    void addDaughter(  EVENT::MCParticle* daughter) ;

    /** Sets the PDG code.
     */
    void setPDG(int pdg ) ;

    /** Sets the HepEvt status.
     * @see MCParticle::getHepEvtStatus
     */
    void setHepEvtStatus( int status ) ;

     /** Sets the production vertex.
     */
    void setVertex( double vtx[3] ) ;
    
     /** Sets the particle endpoint.
      *  Will be ignored for particles that have daughters.
      */
    void setEndpoint( double pnt[3] ) ;
    
    /** Sets the momentum.
      */
    void setMomentum( float p[3] );

    /** Sets the mass.
     */
    void setMass( float m) ;

    /** Sets the charge.
     */
    void setCharge( float c ) ;


  protected:
    /** Initializes an array of pointers for reading daughters.
     * 
     */
    //void prepareArrayOfDaughters(int i) ;

    EVENT::MCParticle* _mother0 ;
    EVENT::MCParticle* _mother1 ;
    int _pdg ;
    int _status ;
    double _vertex[3] ;
    double _endpoint[3] ;
    float _p[3] ;
    float _mass ;
    float _charge ;
    MCParticlePVec _daughtersP ;
    
    //    MCParticle** _readDaughters ;

}; // class
}; // namespace IMPL
#endif /* ifndef EVENT_MCPARTICLEIMPL_H */
