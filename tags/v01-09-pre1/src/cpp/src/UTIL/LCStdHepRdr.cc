#include "UTIL/LCStdHepRdr.h"
#include "EVENT/MCParticle.h"
#include "IMPL/MCParticleImpl.h"
#include "lcio.h"
#include "EVENT/LCIO.h"
#include <sstream>
//#include <stdlib.h>
#include <cmath>	// for pow()
#include "Exceptions.h"

using namespace EVENT ;
using namespace IMPL ;


namespace UTIL{

  LCStdHepRdr::LCStdHepRdr(const char* evfile){
    //
    //   Use Willie's reader from LELAPS, and open the input file
    //
    _reader = new lStdHep(evfile,false);
    if(_reader->getError()) {
      std::stringstream description ; 
      description << "LCStdHepRdr: no stdhep file: " << evfile << std::ends ;
      throw IO::IOException( description.str() );
    }

    _reader->printFileHeader() ;

  }
  LCStdHepRdr::~LCStdHepRdr(){}
  
  //
  // Read an event and return a LCCollectionVec of MCParticles
  //
  IMPL::LCCollectionVec * LCStdHepRdr::readEvent()
  {
    IMPL::LCCollectionVec * mcVec = 0;
    double c_light = 299.792;// mm/ns
    //
    //  Read the event, check for errors
    //
    if(_reader->more()){

      if( int errorcode = _reader->readEvent() ) {
	std::stringstream description ; 
	description << "LCStdHepRdr::readEvent: error when reading event: " << errorcode << std::ends ;
	throw IO::IOException( description.str() );
	return mcVec;
      }

    } else {
      //
      // End of File :: ??? Exception ???
      //   -> FG:   EOF is not an exception as it happens for every file at the end !
      //
      return mcVec;
    }
    //
    //  Create a Collection Vector
    //
    mcVec = new IMPL::LCCollectionVec(LCIO::MCPARTICLE);
    MCParticleImpl* p;
    MCParticleImpl* d;
    //
    //  Loop over particles
    //
    int NHEP = _reader->nTracks();

    for( int IHEP=0; IHEP<NHEP; IHEP++ )
      {
	//
	//  Create a MCParticle and fill it from stdhep info
	//
	MCParticleImpl* mcp = new MCParticleImpl();
	//
	//  PDGID
	//
	mcp->setPDG(_reader->pid(IHEP));


	//
	//  charge
	// 
	mcp->setCharge( threeCharge( mcp->getPDG() ) / 3. ) ;

	//
	//  Momentum vector
	//
	float p0[3] = {_reader->Px(IHEP),_reader->Py(IHEP),_reader->Pz(IHEP)};
	mcp->setMomentum(p0);
	//
	//  Mass
	//
	mcp->setMass(_reader->M(IHEP));
	//
	//  Vertex
	//
	double v0[3] = {_reader->X(IHEP),_reader->Y(IHEP),_reader->Z(IHEP)};
	mcp->setVertex(v0);
	//
	//  Generator status
	//
	mcp->setGeneratorStatus(_reader->status(IHEP));
	//
	//  Simulator status 0 until simulator acts on it
	//
	mcp->setSimulatorStatus(0);
	//
	//  Creation time (note the units)
	//
	mcp->setTime(_reader->T(IHEP)/c_light);
	//
	//  Add the particle to the collection vector
	//
	mcVec->push_back(mcp);
	//
	// Add the parent information. The implicit assumption here is that
	// no particle is read in before its parents.
	//
	int fp = _reader->mother1(IHEP) - 1;
	int lp = _reader->mother2(IHEP) - 1;
	//
	//  If both first parent and second parent > 0, and second parent >
	//     first parent, assume a range
	//
	if( (fp > -1) && (lp > -1) )
	  {
	    if(lp >= fp)
	      {
		for(int ip=fp;ip<lp+1;ip++)
		  {
		    p = dynamic_cast<MCParticleImpl*>
		      (mcVec->getElementAt(ip));
		    mcp->addParent(p);
		  }
	      }
	    //
	    //  If first parent < second parent, assume 2 discreet parents
	    //
	    else
	      {
		p = dynamic_cast<MCParticleImpl*>
		  (mcVec->getElementAt(fp));
		mcp->addParent(p);
		p = dynamic_cast<MCParticleImpl*>
		  (mcVec->getElementAt(lp));
		mcp->addParent(p);
	      }
	  }
	//
	//  Only 1 parent > 0, set it
	//
	else if(fp > -1)
	  {
	    p = dynamic_cast<MCParticleImpl*>
	      (mcVec->getElementAt(fp));
	    mcp->addParent(p);
	  }
	else if(lp > -1)
	  {
	    p = dynamic_cast<MCParticleImpl*>
	      (mcVec->getElementAt(lp));
	    mcp->addParent(p);
	  }
      }// End loop over particles
    //
    //  Now make a second loop over the particles, checking the daughter
    //  information. This is not always consistent with parent 
    //  information, and this utility assumes all parents listed are
    //  parents and all daughters listed are daughters
    //
    for( int IHEP=0; IHEP<NHEP; IHEP++ )
      {
	//
	//  Get the MCParticle
	//
	MCParticleImpl* mcp = 
	  dynamic_cast<MCParticleImpl*>
	  (mcVec->getElementAt(IHEP));
	//
	//  Get the daughter information, discarding extra information
	//  sometimes stored in daughter variables.
	//
	int fd = _reader->daughter1(IHEP)%10000 - 1;
	int ld = _reader->daughter2(IHEP)%10000 - 1;
	//
	//  As with the parents, look for range, 2 discreet or 1 discreet 
	//  daughter.
	//
	if( (fd > -1) && (ld > -1) )
	  {
	    if(ld >= fd)
	      {
		for(int id=fd;id<ld+1;id++)
		  {
		    //
		    //  Get the daughter, and see if it already lists this particle as
		    //    a parent.
		    //
		    d = dynamic_cast<MCParticleImpl*>
		      (mcVec->getElementAt(id));
		    int np = d->getParents().size();
		    bool gotit = false;
		    for(int ip=0;ip < np;ip++)
		      {
			p = dynamic_cast<MCParticleImpl*>
			  (d->getParents()[ip]);
			if(p == mcp)gotit = true;
		      }
		    //
		    //  If not already listed, add this particle as a parent
		    //
		    if(!gotit)d->addParent(mcp);
		  }
	      }
	    //
	    //  Same logic, discreet cases
	    //
	    else
	      {
		d = dynamic_cast<MCParticleImpl*>
		  (mcVec->getElementAt(fd));
		int np = d->getParents().size();
		bool gotit = false;
		for(int ip=0;ip < np;ip++)
		  {
		    p = dynamic_cast<MCParticleImpl*>
		      (d->getParents()[ip]);
		    if(p == mcp)gotit = true;
		  }
		if(!gotit)d->addParent(mcp);
		d = dynamic_cast<MCParticleImpl*>
		  (mcVec->getElementAt(ld));
		np = d->getParents().size();
		gotit = false;
		for(int ip=0;ip < np;ip++)
		  {
		    p = dynamic_cast<MCParticleImpl*>
		      (d->getParents()[ip]);
		    if(p == mcp)gotit = true;
		  }
		if(!gotit)d->addParent(mcp);
	      }
	  }
	else if(fd > -1)
	  {
	    d = dynamic_cast<MCParticleImpl*>
	      (mcVec->getElementAt(fd));
	    int np = d->getParents().size();
	    bool gotit = false;
	    for(int ip=0;ip < np;ip++)
	      {
		p = dynamic_cast<MCParticleImpl*>
		  (d->getParents()[ip]);
		if(p == mcp)gotit = true;
	      }
	    if(!gotit)d->addParent(mcp);
	  }
	else if(ld > -1)
	  {
	    d = dynamic_cast<MCParticleImpl*>
	      (mcVec->getElementAt(ld));
	    int np = d->getParents().size();
	    bool gotit = false;
	    for(int ip=0;ip < np;ip++)
	      {
		p = dynamic_cast<MCParticleImpl*>
		  (d->getParents()[ip]);
		if(p == mcp)gotit = true;
	      }
	    if(!gotit)d->addParent(mcp);
	  }
      }// End second loop over particles
    //
    //  Return the collection
    //
    return mcVec;    
  }


  int LCStdHepRdr::threeCharge( int pdgID ) const {
    //
    // code copied from HepPDT package, author L.Garren
    // modified to take pdg
    
    ///  PID digits (base 10) are: n nr nl nq1 nq2 nq3 nj
    ///  The location enum provides a convenient index into the PID.
    enum location { nj=1, nq3, nq2, nq1, nl, nr, n, n8, n9, n10 };

    int charge=0;
    int ida, sid;
    unsigned short q1, q2, q3;
    static int ch100[100] = { -1, 2,-1, 2,-1, 2,-1, 2, 0, 0,
			      -3, 0,-3, 0,-3, 0,-3, 0, 0, 0,
			      0, 0, 0, 3, 0, 0, 0, 0, 0, 0,
			      0, 0, 0, 3, 0, 0, 3, 0, 0, 0,
			      0, -1, 0, 0, 0, 0, 0, 0, 0, 0,
			      0, 6, 3, 6, 0, 0, 0, 0, 0, 0,
			      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			      0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    
    ida = (pdgID < 0) ? -pdgID : pdgID ;
    
    //     q1 = digit(nq1);
    //     q2 = digit(nq2);
    //     q3 = digit(nq3);

    q1 =  ( ida / ( (int) std::pow( 10.0, (nq1 -1) ) )  ) % 10 ;
    q2 =  ( ida / ( (int) std::pow( 10.0, (nq2 -1) ) )  ) % 10 ;
    q3 =  ( ida / ( (int) std::pow( 10.0, (nq3 -1) ) )  ) % 10 ;
    
//     sid = fundamentalID();
    //---- ParticleID::fundamentalID -------
    short dig_n9 =  ( ida / ( (int) std::pow( 10.0, (n9 -1) ) )  ) % 10 ;
    short dig_n10 =  ( ida / ( (int) std::pow( 10.0, (n10 -1) ) )  ) % 10 ;
    
    if( ( dig_n10 == 1 ) && ( dig_n9 == 0 ) ) {
      
      sid = 0 ;
    } 
    else if( q2 == 0 && q1 == 0) {
      
      sid = ida % 10000;
    } 
    else if( ida <= 102 ) {
      
      sid = ida ; 
    } 
    else {

      sid = 0;
    }
    //----------------

    int extraBits = ida / 10000000 ;
    // everything beyond the 7th digit (e.g. outside the numbering scheme)

    short dig_nj =  ( ida / ( (int) std::pow( 10.0, (nj -1) ) )  ) % 10 ;

    if( ida == 0 || extraBits > 0 ) {      // ion or illegal
      return 0;
    } else if( sid > 0 && sid <= 100 ) {	// use table
      charge = ch100[sid-1];
      if(ida==1000017 || ida==1000018) { charge = 0; }
      if(ida==1000034 || ida==1000052) { charge = 0; }
      if(ida==1000053 || ida==1000054) { charge = 0; }
      if(ida==5100061 || ida==5100062) { charge = 6; }
    } else if( dig_nj == 0 ) { 		// KL, Ks, or undefined
      return 0;
    } else if( q1 == 0 ) {			// mesons
      if( q2 == 3 || q2 == 5 ) {
	charge = ch100[q3-1] - ch100[q2-1];
      } else {
	charge = ch100[q2-1] - ch100[q3-1];
      }
    } else if( q3 == 0 ) {			// diquarks
      charge = ch100[q2-1] + ch100[q1-1];
    } else { 					// baryons
      charge = ch100[q3-1] + ch100[q2-1] + ch100[q1-1];
    }
    if( charge == 0 ) {
      return 0;
    } else if( pdgID < 0 ) {
      charge = -charge; 
    }
    return charge;
  }

} // namespace UTIL
