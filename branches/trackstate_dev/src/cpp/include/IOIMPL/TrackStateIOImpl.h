#ifndef SIO_TRACKSTATEIOIMPL_H
#define SIO_TRACKSTATEIOIMPL_H 1

#include "IMPL/TrackStateImpl.h"

// forward declaration
namespace SIO{ 
  class SIOTrackStateHandler ;
}

namespace IOIMPL {

  class SIOTrackStateHandler ;
  
/** Adding stuff needed for io (friend declarations, etc.)
 * 
 * @author gaede, engels
 * @version Mar 15, 2004
 */
  class TrackStateIOImpl : public IMPL::TrackStateImpl {
    
    friend class SIO::SIOTrackStateHandler ;
    
  }; // class

} // namespace

#endif /* ifndef SIO_TRACKSTATEIOIMPL_H */
