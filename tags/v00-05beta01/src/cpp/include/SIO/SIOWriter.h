// -*- C++ -*-
/** Concrete implementation of LCWriter using SIO.
 * 
 * @author gaede
 * @version Mar 6, 2003
 */
#ifndef SIO_SIOWRITER_H
#define SIO_SIOWRITER_H 1

#include <string>
#include <vector>
#include "IO/LCWriter.h"
#include "EVENT/LCEvent.h"
#include "EVENT/LCRunHeader.h"

class SIO_record ;
class SIO_stream ;    


namespace SIO {

  class SIOEventHandler ;
  class SIORunHeaderHandler ;
  class SIOCollectionHandler ;

    
  class SIOWriter : public IO::LCWriter {


  public:

    /**Default constructor.
     */
    SIOWriter() ;
    /**
     * Destructor
     */
    virtual ~SIOWriter() ;

    /** Opens a file for writing and returns LCIO::SUCCESS if 
     * no error occured. If file with given name exists, it will be opened in
     * append-mode.
     */
    virtual int open(const std::string & filename) throw (IO::IOException) ;

    /** Opens a file for writing and returns LCIO::SUCCESS if 
     *  no error occured. Possible write modes are: LCIO::WRITE_NEW
     * (existing files are replaced) and LCIO::WRITE_APPEND. 
     */
    virtual int open(const std::string & filename, int writeMode) ;
    
    /** Writes the given run header to file and returns LCIO::SUCCESS if no error occured.
     */
    virtual int writeRunHeader(const EVENT::LCRunHeader * hdr) ;

    /** Writes the given event to file.
     */
    virtual int writeEvent(const EVENT::LCEvent * evt) ;

    /** Closes the output file/stream etc.
     */
    virtual int close() ;

  protected:

    /** Sets up the handlers for writing the current event.
     */
    void setUpHandlers(const EVENT::LCEvent * evt)  ;
    
    /** Creates a proper filename with extension 'slcio' 
     * in sioFilename.
     */
    void getSIOFileName(const std::string& filename, 
			std::string& sioFilename)  ; 
    
  protected:
    
    SIO_record *_evtRecord ;
    SIO_record *_hdrRecord ;
    SIO_record *_runRecord ;
    SIO_stream *_stream ;

  private:

     //fg20030514 obsolete    SIOEventHandler *_evtHandler ;
    SIOEventHandler *_hdrHandler ;
    SIORunHeaderHandler *_runHandler ;
    std::vector<SIOCollectionHandler*> _colVector ;

    //fg20030514 obsolete EVENT::LCEvent **evtP ;

  }; // class

}; // namespace

#endif /* ifndef SIO_SIOWRITER_H */
