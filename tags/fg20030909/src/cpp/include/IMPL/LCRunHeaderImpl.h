#ifndef EVENT_LCRUNHEADERIMPL_H
#define EVENT_LCRUNHEADERIMPL_H 1

#include <string>

#include "EVENT/LCIO.h"
#include "EVENT/LCRunHeader.h"
#include "AccessChecked.h"


namespace IMPL {

/** Implementation of LCRunHeader.
 * 
 * @author gaede
 * @version Mar 11, 2003
 */
  class LCRunHeaderImpl : public EVENT::LCRunHeader, public AccessChecked {

  public: 

    LCRunHeaderImpl() ;

    /// Destructor.
    virtual ~LCRunHeaderImpl() ; 

    /** Returns the run number.
     */
    virtual int getRunNumber() const ;

    /** Returns the name of the detector setup used in the simulation.
     */
    virtual const std::string & getDetectorName() const ;

    /** Description of the simulation conditions, e.g. physics channels.
     */		 
    virtual const std::string & getDescription() const ;

    /** Returns the names of the active subdetectors
     *  used in the simulation.
     */ 
    virtual const std::vector<std::string> * getActiveSubdetectors() const ;

    // set methods

    /** Sets the run number.
     */
    void setRunNumber( int rn) ;

    /** Sets the detector name.
     */
    void setDetectorName(const std::string& dn) ;

    /** Sets the description of the run.
     */
    void setDescription(const std::string& dsc) ;

    /** Adds an active sub detector name.
     */
    void addActiveSubdetector(const std::string&  adn) ;


  protected:
    
    int _runNumber ;
    std::string _detectorName ;
    std::string _description ;
    std::vector<std::string> _activeSubdetectors ;

  }; // class
}; // namespace IMPL
#endif /* ifndef IMPL_LCRUNHEADERIMPL_H */
