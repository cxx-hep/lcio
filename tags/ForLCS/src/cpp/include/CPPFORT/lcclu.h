/**Header file for the f77-wrapper functions of the Cluster Class.
 * 
 * @author H. Vogt
 * @version May 10, 2004
 */
#include "cfortran.h"
#include "cpointer.h"

// Warning: dont use "_" in function names as this causes two many
// trailing underscores on Linux

PTRTYPE lcclucreate() ;
int     lccludelete( PTRTYPE clu ) ;

int     lcclugetid( PTRTYPE clu ) ;
int     lcclugettype( PTRTYPE clu ) ;
float   lcclugetenergy( PTRTYPE clu ) ;
int     lcclugetposition( PTRTYPE clu, float* pos ) ;
int     lcclugetpositionerror( PTRTYPE clu, float* poserr ) ;
float   lcclugettheta( PTRTYPE clu ) ;
float   lcclugetphi( PTRTYPE clu ) ;
int     lcclugetdirectionerror( PTRTYPE clu, float* direrr ) ;
int     lcclugetshape( PTRTYPE clu, float* shape ) ;
int     lcclugetparticletype( PTRTYPE clu, float* weights) ;
PTRTYPE lcclugetclusters( PTRTYPE clu ) ;
PTRTYPE lcclugetcalorimeterhits( PTRTYPE clu ) ;
int     lcclugethitcontributions( PTRTYPE clu, float* ener, int* nener ) ;

int     lcclusettype( PTRTYPE clu, int type ) ;
int     lcclusetenergy( PTRTYPE clu, float energy ) ;
int     lcclusetposition( PTRTYPE clu, float* refpoint ) ;
int     lcclusetpositionerror( PTRTYPE clu, float* poserr ) ;
int     lcclusettheta( PTRTYPE clu, float theta ) ;
int     lcclusetphi( PTRTYPE clu, float phi ) ;
int     lcclusetdirectionerror( PTRTYPE clu, float* direrr ) ;
int     lcclusetshape( PTRTYPE clu, float* shape ) ;
int     lcclusetemweight(  PTRTYPE clu, float weight ) ;
int     lcclusethadweight(PTRTYPE clu, float weight ) ;
int     lcclusetmuonweight( PTRTYPE clu, float weight ) ;
int     lccluaddcluster( PTRTYPE clu, PTRTYPE clus ) ;
int     lccluaddhit( PTRTYPE clu, PTRTYPE calohit, float weigth ) ;

// now the fortran wrappers from cfortran.h
extern "C"{
  
FCALLSCFUN0(CFORTRANPNTR, lcclucreate, LCCLUCREATE, lcclucreate ) ;
FCALLSCFUN1(INT, lccludelete, LCCLUDELETE, lccludelete, CFORTRANPNTR ) ;

FCALLSCFUN1(INT, lcclugetid, LCCLUGETID, lcclugetid, CFORTRANPNTR) ;
FCALLSCFUN1(INT, lcclugettype, LCCLUGETTYPE, lcclugettype, CFORTRANPNTR) ;
FCALLSCFUN1(FLOAT, lcclugetenergy, LCCLUGETENERGY, lcclugetenergy, CFORTRANPNTR) ;
FCALLSCFUN2(INT, lcclugetposition, LCCLUGETPOSITION, lcclugetposition, CFORTRANPNTR, FLOATV) ;
FCALLSCFUN2(INT, lcclugetpositionerror, LCCLUGETPOSITIONERROR, lcclugetpositionerror, CFORTRANPNTR, FLOATV) ;
FCALLSCFUN1(FLOAT, lcclugettheta, LCCLUGETTHETA, lcclugettheta, CFORTRANPNTR) ;
FCALLSCFUN1(FLOAT, lcclugetphi, LCCLUGETPHI, lcclugetphi, CFORTRANPNTR) ;
FCALLSCFUN2(INT, lcclugetdirectionerror, LCCLUGETDIRECTIONERROR, lcclugetdirectionerror, CFORTRANPNTR, FLOATV) ;
FCALLSCFUN2(INT, lcclugetshape, LCCLUGETSHAPE, lcclugetshape, CFORTRANPNTR, FLOATV) ;
FCALLSCFUN2(INT, lcclugetparticletype, LCCLUGETPARTICLETYPE, lcclugetparticletype, CFORTRANPNTR, FLOATV) ;
FCALLSCFUN1(CFORTRANPNTR, lcclugetclusters, LCCLUGETCLUSTERS, lcclugetclusters, CFORTRANPNTR) ;
FCALLSCFUN1(CFORTRANPNTR, lcclugetcalorimeterhits, LCCLUGETCALORIMETERHITS, lcclugetcalorimeterhits, CFORTRANPNTR) ;
FCALLSCFUN3(INT, lcclugethitcontributions, LCCLUGETHITCONTRIBUTIONS, lcclugethitcontributions, CFORTRANPNTR, FLOATV, INTV) ;

FCALLSCFUN2(INT, lcclusettype, LCCLUSETTYPE, lcclusettype, CFORTRANPNTR, INT) ;
FCALLSCFUN2(INT, lcclusetenergy, LCCLUSETENERGY, lcclusetenergy, CFORTRANPNTR, FLOAT) ;
FCALLSCFUN2(INT, lcclusetposition, LCCLUSETPOSITION, lcclusetposition, CFORTRANPNTR, FLOATV) ;
FCALLSCFUN2(INT, lcclusetpositionerror, LCCLUSETPOSITIONERROR, lcclusetpositionerror, CFORTRANPNTR, FLOATV) ;
FCALLSCFUN2(INT, lcclusettheta, LCCLUSETTHETA, lcclusettheta, CFORTRANPNTR, FLOAT) ;
FCALLSCFUN2(INT, lcclusetphi, LCCLUSETPHI, lcclusetphi, CFORTRANPNTR, FLOAT) ;
FCALLSCFUN2(INT, lcclusetdirectionerror, LCCLUSETDIRECTIONERROR, lcclusetdirectionerror, CFORTRANPNTR, FLOATV) ;
FCALLSCFUN2(INT, lcclusetshape, LCCLUSETSHAPE, lcclusetshape, CFORTRANPNTR, FLOATV) ;
FCALLSCFUN2(INT, lcclusetemweight, LCCLUSETEMWEIGHT, lcclusetemweight, CFORTRANPNTR, FLOAT) ;
FCALLSCFUN2(INT, lcclusethadweight, LCCLUSETHADWEIGHT, lcclusethadweight, CFORTRANPNTR, FLOAT) ;
FCALLSCFUN2(INT, lcclusetmuonweight, LCCLUSETMUONWEIGHT, lcclusetmuonweight, CFORTRANPNTR, FLOAT) ;
FCALLSCFUN2(INT, lccluaddcluster, LCCLUADDCLUSTER, lccluaddcluster, CFORTRANPNTR, CFORTRANPNTR) ;
FCALLSCFUN3(INT, lccluaddhit, LCCLUADDHIT, lccluaddhit, CFORTRANPNTR, CFORTRANPNTR, FLOAT) ;

}
  