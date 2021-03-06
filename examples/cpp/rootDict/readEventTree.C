// #include "TTree.h"
// #include "TFile.h"
// #include "TBranch.h"
#ifndef __CINT__ 
#include "IO/LCReader.h"
#include "IOIMPL/LCFactory.h"
#include "EVENT/MCParticle.h"
#include "EVENT/LCCollection.h"
#include "IMPL/LCEventImpl.h"
#include "UTIL/LCTOOLS.h"
#endif


/** Example script for testing the ROOT LCIO dictionary.
 * 
 *  readEventTree: 
 *  read LCEvents from tree with branch holding complete event  e.g.
 *  created with writeEventTree.C
 *  and fill a histogram from MCParticle::getEnergy()
 */

void readEventTree(const char* FILEN) {
  
  //just in case this script is executed multiple times
  delete gROOT->GetListOfFiles()->FindObject( FILEN );
  delete gROOT->GetListOfCanvases()->FindObject("c1");
  
  if (!TClassTable::GetDict("IMPL::ReconstructedParticleImpl")) {
    unsigned res ;
    
    res = gSystem->Load("$LCIO/lib/liblcio.so"); 
    res = gSystem->Load("$LCIO/lib/liblcioDict.so"); 
  }
  
  
  std::cout << " loaded LCIO library and dictionary ... " << std::endl ;
  

  //======== open file and get tree and branch =====================
  
  TFile* f = new TFile( FILEN, "READ") ;
  if (!f) { return; }
  
  TTree *t; f->GetObject("LCIO",t);
  
  IMPL::LCEventImpl* evt=0 ;
  TBranch* bevt = t->GetBranch("LCEvent") ;
  
  if( bevt ){
    
    bevt->SetAddress( &evt  ) ; 
    
  } else {

    std::cout << " --- branch 'LCEvent' not found ;-( "  <<  std::endl ;
  }


  // define a simple histogram

  TH1F* h_mcpEner = new TH1F("h_mcpEner","MCParticles E(GeV) ", 100, 0.  , 100.  ) ;

 std::string mcpName("MCParticlesSkimmed") ;


  int nEvents = 0  ;
  
  int nevt = t->GetEntries();

  for (Int_t i = 0; i < nevt ; i++) {

    Long64_t tentry = t->LoadTree(i);

    int nbyte  = bevt->GetEntry(tentry);

    
    //========  LCIO event loop below this line ============================


    //    UTIL::LCTOOLS::dumpEventDetailed( evt ) ;
    UTIL::LCTOOLS::dumpEvent( evt ) ;


    EVENT::LCCollection* col =  evt->getCollection( mcpName  ) ;

    int nMcp = col->getNumberOfElements() ;
    for( int j = 0 ; j < nMcp ; ++j ) { 

      EVENT::MCParticle* mcp = (EVENT::MCParticle*) col->getElementAt(j) ;

      h_mcpEner->Fill( mcp->getEnergy() ) ; 

    }

    nEvents ++ ;

  }

  // -------- end of event loop -----------
  
  std::cout << std::endl 
	    <<  "  " <<  nEvents 
	    << " events read from file: " 
	    << FILEN << std::endl  ;
  
  


  //========   draw histogram(s)  ============================

  // -------------------------
  TCanvas *c1 = new TCanvas("c1","LCIO root example",200,10,700,500);
  //  c1->Divide(2,2);
  c1->cd(1)  ;
  h_mcpEner->Draw();
  
}
