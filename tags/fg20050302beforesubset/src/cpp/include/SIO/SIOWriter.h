#ifndef SIO_SIOWRITER_H
#define SIO_SIOWRITER_H 1

#include <string>
#include <vector>
#include "IO/LCWriter.h"
#include "EVENT/LCEvent.h"
#include "EVENT/LCRunHeader.h"

#include "SIO_block.h"

class SIO_record ;
class SIO_stream ;    


namespace SIO {

  class SIOEventHandler ;
  class SIORunHeaderHandler ;
  class SIOCollectionHandler ;

  class SIOReader ;
  class SIOUnpack ;

/** Concrete implementation of LCWriter using SIO.
 * Use LCFactory to instantiate.
 *
 * @see LCFactory
 * @author gaede
 * @version Mar 6, 2003
 */
  class SIOWriter : public IO::LCWriter {


    friend class SIOReader ; // SIO Reader uses SAME SIO records !!
    friend class SIOUnpack ;

  public:

    /**Default constructor.
     */
    SIOWriter() ;
    /**
     * Destructor
     */
    virtual ~SIOWriter() ;

    /** Opens a file for writing. If file with given name exists, 
     * an exception is thrown. Use append or new mode instead.
     *
     *@throws IOException
     */
    virtual void open(const std::string & filename) throw (IO::IOException, std::exception) ;

    /** Opens a file for writing.
     * Possible write modes are: LCIO::WRITE_NEW
     * (existing files are replaced) and LCIO::WRITE_APPEND. 
     *
     *@throws IOException
     */
    virtual void open(const std::string & filename, int writeMode)throw (IO::IOException, std::exception) ;
    
    /** Writes the given run header to file.
     *
     *@throws IOException
     */
    virtual void writeRunHeader(const EVENT::LCRunHeader * hdr)throw (IO::IOException, std::exception) ;

    /** Writes the given event to file.
     *
     *@throws IOException
     */
    virtual void writeEvent(const EVENT::LCEvent * evt) throw (IO::IOException, std::exception) ;

    /** Closes the output file/stream etc.
     *
     *@throws IOException
     */
    virtual void close() throw (IO::IOException, std::exception) ;

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
    
    static SIO_record *_evtRecord ;
    static SIO_record *_hdrRecord ;
    static SIO_record *_runRecord ;
    SIO_stream *_stream ;

  private:

    SIOEventHandler *_hdrHandler ;
    SIORunHeaderHandler *_runHandler ;
    std::vector<SIO_block*> _connectedBlocks ;

  }; // class

}; // namespace.

#endif /* ifndef SIO_SIOWRITER_H */