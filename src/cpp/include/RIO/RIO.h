#ifndef RIO_defines_h
#define RIO_defines_h 1


/** Global defines for ROOT I/O
 */


#define RIO_FILE_EXTENSION     ".rlcio"

#define RIO_LCIO_TREENAME         "LCIO"
#define RIO_LCIO_TREENAME_CYCLE_1 "LCIO;1"   // root way of versioning 

#define RIO_LCEVENT_BRANCHNAME      "LCEvent"
#define RIO_LCRUNHEADER_BRANCHNAME  "LCRunHeader"


#define RIO_RECORD_SIZE    16000

#define RIO_SPLIT_LEVEL        99

#define RIO_DEFAULT_COMPRESS   1
  // 1 is TFile default ?

#endif
