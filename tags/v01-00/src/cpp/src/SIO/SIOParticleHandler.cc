#include "SIO/SIOParticleHandler.h"
#include "SIO/LCSIO.h"

#include "DATA/MCParticleData.h"
#include "IOIMPL/MCParticleIOImpl.h"

#include "SIO_functions.h"
#include "SIO_block.h"
//#include <iostream>

#include "EVENT/LCCollection.h"

using namespace EVENT ;
using namespace DATA ;
using namespace IOIMPL ;

namespace SIO {
  
  unsigned int SIOParticleHandler::read(SIO_stream* stream, 
					LCObject** objP, 
					unsigned int flag,  
					unsigned int vers ){

    if ( SIO_VERSION_MAJOR(vers)==0 && SIO_VERSION_MINOR(vers)==8)  
      return readv00_08( stream, objP, flag, vers ) ;
    
    
    unsigned int status ; 
    
    // create a new object :
    MCParticleIOImpl* particle  = new MCParticleIOImpl ;
    *objP = particle ;
    
    // tell SIO the address of particle as an abstract  MCParticle ...
    // this is important, as SIO takes the bare address 
    // (int)(MCParticle*) particle != (int)particle !!!!
    SIO_PTAG( stream , dynamic_cast<MCParticle*>(particle) ) ;
    //     SIO_PNTR( stream , &(particle->_mother0) ) ;
    //     SIO_PNTR( stream , &(particle->_mother1) ) ;
    
    // parents
    int numberOfParents ; 
    SIO_DATA( stream ,  &numberOfParents , 1  ) ;
    
    //     //    particle->prepareArrayOfParents( numberOfParents ) ;
    
    for(int i=0;i<numberOfParents;i++){
      
      // create a pointer to a pointer to a MCParticle 
      // as SIO need the address of the pointer for pointer reallocation....
      MCParticle** pD = new (MCParticle*) ;
      SIO_PNTR( stream , pD ) ;
      particle->_parentsP.push_back( pD ) ;
    }
    
    SIO_DATA( stream ,  &(particle->_pdg) , 1  ) ;
    SIO_DATA( stream ,  &(particle->_genstatus) , 1  ) ;
    SIO_DATA( stream ,  &(particle->_simstatus) , 1  ) ;
    SIO_DATA( stream ,  particle->_vertex  , 3 ) ;
    SIO_DATA( stream ,  particle->_p  , 3 ) ;
    SIO_DATA( stream ,  &(particle->_mass) , 1  ) ;
    SIO_DATA( stream ,  &(particle->_charge) , 1  ) ;
    
    if( particle->_simstatus & 0x80000000 )  // bit 31
      SIO_DATA( stream ,  particle->_endpoint  , 3 ) ;
    
    return ( SIO_BLOCK_SUCCESS ) ;
  }
  
  unsigned int SIOParticleHandler::write(SIO_stream* stream, 
					 const LCObject* obj, 
					 unsigned int flag ){

    //fg 20030609 changed to use MCParticleData    
    unsigned int status ; 
    
    const MCParticleData* particle  = dynamic_cast<const MCParticleData*>(obj)  ;
    SIO_PTAG( stream , particle ) ;
    
//     const MCParticleData* myMom0 = particle->getParentData() ;
//     const MCParticleData* myMom1 = particle->getSecondParentData() ;
//     SIO_PNTR( stream , &myMom0 ) ;
//     SIO_PNTR( stream , &myMom1 ) ;

    int numberOfParents = particle->getNumberOfParents() ;
    
    SIO_DATA( stream , &numberOfParents  , 1 ) ;
    
    for(int i=0;i<numberOfParents;i++){
      const MCParticleData* part = particle->getParentData(i) ;
      SIO_PNTR( stream ,  &part  ); 
    }

    LCSIO_WRITE( stream, particle->getPDG() ) ;
    LCSIO_WRITE( stream, particle->getGeneratorStatus() ) ;

    if( particle->getEndpoint() != 0 ) {
      int simstatus = particle->getSimulatorStatus()  ;
      simstatus |= 0x80000000 ; // set bit 31
      LCSIO_WRITE( stream, simstatus ) ;
    } else {
      LCSIO_WRITE( stream, particle->getSimulatorStatus() ) ;
    }


    SIO_DATA( stream, const_cast<double*>( particle->getVertex() ) , 3 ) ;
    SIO_DATA( stream, const_cast<float*>( particle->getMomentum()), 3 ) ;
    LCSIO_WRITE( stream, particle->getMass() ) ;
    LCSIO_WRITE( stream, particle->getCharge() ) ;


    if( particle->getEndpoint() != 0 ) 
      SIO_DATA( stream, const_cast<double*>( particle->getEndpoint() ) , 3 ) ;


    return ( SIO_BLOCK_SUCCESS ) ;

  }

  unsigned int SIOParticleHandler::readv00_08(SIO_stream* stream, 
					LCObject** objP, 
					unsigned int flag,  
					unsigned int vers ){
    unsigned int status ; 
    
    // create a new object :
    MCParticleIOImpl* particle  = new MCParticleIOImpl ;
    *objP = particle ;
    
    // tell SIO the address of particle as an abstract  MCParticle ...
    // this is important, as SIO takes the bare address 
    // (int)(MCParticle*) particle != (int)particle !!!!
    SIO_PTAG( stream , dynamic_cast<MCParticle*>(particle) ) ;

    int ignorePointer ;
//     SIO_PNTR( stream , &(particle->_mother0) ) ;
//     SIO_PNTR( stream , &(particle->_mother1) ) ;
    SIO_DATA( stream , &ignorePointer  , 1  ) ;
    SIO_DATA( stream , &ignorePointer  , 1  ) ;


    // daughters
    int numberOfDaughters ; 
    SIO_DATA( stream ,  &numberOfDaughters , 1  ) ;

    //    particle->prepareArrayOfDaughters( numberOfDaughters ) ;

    for(int i=0;i<numberOfDaughters;i++){

      // create a pointer to a pointer to a MCParticle 
      // as SIO need the address of the pointer for pointer reallocation....
      MCParticle** pD = new (MCParticle*) ;
      SIO_PNTR( stream , pD ) ;
      particle->_daughtersP.push_back( pD ) ;
    }

    SIO_DATA( stream ,  &(particle->_pdg) , 1  ) ;
    SIO_DATA( stream ,  &(particle->_genstatus) , 1  ) ;
    particle->_genstatus = 0 ;   // default value
    SIO_DATA( stream ,  particle->_vertex  , 3 ) ;
    SIO_DATA( stream ,  particle->_p  , 3 ) ;
    SIO_DATA( stream ,  &(particle->_mass) , 1  ) ;
    SIO_DATA( stream ,  &(particle->_charge) , 1  ) ;
    
    // if the particles doesn't have daughters we read its endpoint
    if( numberOfDaughters == 0 ) {
      SIO_DATA( stream ,  particle->_endpoint  , 3 ) ;
      particle->_genstatus = 0x80000000 ; // set bit31
    }
    return ( SIO_BLOCK_SUCCESS ) ;
  }


  void SIOParticleHandler::restoreParentDaughterRelations( EVENT::LCEvent* evt){
    
    const std::vector< std::string >* strVec = evt->getCollectionNames() ;
    std::vector< std::string >::const_iterator name ;
    
    for( name = strVec->begin() ; name != strVec->end() ; name++){
    
      LCCollection* col ;
      if( (col = evt->getCollection( *name ))->getTypeName() == LCIO::MCPARTICLE ){
	
	int nDaughtersTotal = 0 ;
	int nParentsTotal = 0 ; 
	for(int i=0; i < col->getNumberOfElements() ; i++){
	  MCParticleIOImpl* mcp = dynamic_cast<MCParticleIOImpl*>( col->getElementAt(i) ) ;
	  nDaughtersTotal += mcp->getNumberOfDaughters()  ;
	  nParentsTotal += mcp->getNumberOfParents() ;
	}

	for(int i=0; i < col->getNumberOfElements() ; i++){
	  
	  MCParticleIOImpl* mcp = dynamic_cast<MCParticleIOImpl*>( col->getElementAt(i) ) ;
	  
	  // for version v00-08 we restore parents from daughters
	  if ( nParentsTotal == 0 &&  nDaughtersTotal > 0 ){
	    
	    int nDaughters = mcp->getNumberOfDaughters() ;
	    for( int j=0; j<nDaughters; j++){
	      MCParticleIOImpl* dgh = dynamic_cast<MCParticleIOImpl*>( mcp->getDaughter(j) ) ;
	      
	      MCParticle ** mcpP = new (MCParticle*) ;
	      *mcpP = mcp ;
	      dgh->_parentsP.push_back( mcpP ) ;
	    }
	    
	  } 
	  else if ( nParentsTotal > 0 && nDaughtersTotal == 0 ) {
	    
	    int nParents = mcp->getNumberOfParents() ;
	    for( int j=0; j<nParents; j++){
	      MCParticleIOImpl* mom = dynamic_cast<MCParticleIOImpl*>( mcp->getParent(j) ) ;
	      MCParticle ** mcpP = new (MCParticle*) ;
	      *mcpP = mcp ;
	      mom->_daughtersP.push_back( mcpP ) ;
	      
	    }
	  }

	} // loop over particles

      } // if( MCPARTICLE ) 
    } // loop over collections
  }
}; // namespace