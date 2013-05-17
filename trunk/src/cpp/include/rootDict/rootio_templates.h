#ifndef rootio_templates_h
#define rootio_templates_h 1

#include "UTIL/LCTypedVector.h"

#include "IMPL/AccessChecked.h"
#include "IMPL/CalorimeterHitImpl.h"
#include "IMPL/ClusterImpl.h"
#include "IMPL/LCCollectionVec.h"
#include "IMPL/LCEventImpl.h"
#include "IMPL/LCFlagImpl.h"
#include "IMPL/LCGenericObjectImpl.h"
#include "IMPL/LCIOExceptionHandler.h"
#include "IMPL/LCParametersImpl.h"
#include "IMPL/LCRelationImpl.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "IMPL/LCTOOLS.h"
#include "IMPL/MCParticleImpl.h"
#include "IMPL/ParticleIDImpl.h"
#include "IMPL/RawCalorimeterHitImpl.h"
#include "IMPL/ReconstructedParticleImpl.h"
#include "IMPL/SimCalorimeterHitImpl.h"
#include "IMPL/SimTrackerHitImpl.h"
#include "IMPL/TPCHitImpl.h"
#include "IMPL/TrackImpl.h"
#include "IMPL/TrackStateImpl.h"
#include "IMPL/TrackerDataImpl.h"
#include "IMPL/TrackerHitImpl.h"
#include "IMPL/TrackerHitPlaneImpl.h"
#include "IMPL/TrackerHitZCylinderImpl.h"
#include "IMPL/TrackerPulseImpl.h"
#include "IMPL/TrackerRawDataImpl.h"
#include "IMPL/VertexImpl.h"

#include "EVENT/LCIO.h"
#include "UTIL/LCStdHepRdr.h"
#include "UTIL/LCTOOLS.h"
#include "UTIL/CellIDEncoder.h"
//#include "UTIL/CellIDDecoder.h"

#include <string>
#include <vector>

// ---- tell ROOT about the templates instantiations that we are going to use ...

#ifdef __MAKECINT__
#pragma link C++ class std::vector<std::string>+;
#pragma link C++ class std::vector<int>+;
#pragma link C++ class std::vector<float>+;

#pragma link C++ class std::map<std::string,EVENT::LCCollection*>+;
#pragma link C++ class std::pair<std::string,EVENT::LCCollection*>+;


#pragma link C++ class std::vector<EVENT::CalorimeterHit*>+;
#pragma link C++ class std::vector<EVENT::Cluster*>+;
#pragma link C++ class std::vector<EVENT::LCGenericObject*>+;
#pragma link C++ class std::vector<EVENT::LCObject*>+;
#pragma link C++ class std::vector<EVENT::LCRelation*>+;
#pragma link C++ class std::vector<EVENT::MCParticle*>+;
#pragma link C++ class std::vector<EVENT::ParticleID*>+;
#pragma link C++ class std::vector<EVENT::RawCalorimeterHit*>+;
#pragma link C++ class std::vector<EVENT::ReconstructedParticle*>+;
#pragma link C++ class std::vector<EVENT::SimCalorimeterHit*>+;
#pragma link C++ class std::vector<EVENT::SimTrackerHit*>+;
#pragma link C++ class std::vector<EVENT::TrackState*>+;
#pragma link C++ class std::vector<EVENT::TrackerData*>+;
#pragma link C++ class std::vector<EVENT::TrackerHit*>+;
#pragma link C++ class std::vector<EVENT::TrackerHitPlane*>+;
#pragma link C++ class std::vector<EVENT::TrackerHitZCylinder*>+;
#pragma link C++ class std::vector<EVENT::TrackerPulse*>+;
#pragma link C++ class std::vector<EVENT::TrackerRawData*>+;
#pragma link C++ class std::vector<EVENT::Vertex*>+;


#pragma link C++ class std::vector<IMPL::CalorimeterHitImpl*>+;
#pragma link C++ class std::vector<IMPL::ClusterImpl*>+;
#pragma link C++ class std::vector<IMPL::LCGenericObjectImpl*>+;

#pragma link C++ class std::vector<IMPL::LCRelationImpl*>+;
#pragma link C++ class std::vector<IMPL::MCParticleImpl*>+;
#pragma link C++ class std::vector<IMPL::ParticleIDImpl*>+;
#pragma link C++ class std::vector<IMPL::RawCalorimeterHitImpl*>+;
#pragma link C++ class std::vector<IMPL::ReconstructedParticleImpl*>+;
#pragma link C++ class std::vector<IMPL::SimCalorimeterHitImpl*>+;
#pragma link C++ class std::vector<IMPL::SimTrackerHitImpl*>+;
#pragma link C++ class std::vector<IMPL::TrackStateImpl*>+;
#pragma link C++ class std::vector<IMPL::TrackerDataImpl*>+;
#pragma link C++ class std::vector<IMPL::TrackerHitImpl*>+;
#pragma link C++ class std::vector<IMPL::TrackerHitPlaneImpl*>+;
#pragma link C++ class std::vector<IMPL::TrackerHitZCylinderImpl*>+;
#pragma link C++ class std::vector<IMPL::TrackerPulseImpl*>+;
#pragma link C++ class std::vector<IMPL::TrackerRawDataImpl*>+;
#pragma link C++ class std::vector<IMPL::VertexImpl*>+;

#pragma link C++ class UTIL::CellIDEncoder<IMPL::CalorimeterHitImpl>+;
#pragma link C++ class UTIL::CellIDEncoder<IMPL::RawCalorimeterHitImpl>+;
#pragma link C++ class UTIL::CellIDEncoder<IMPL::SimCalorimeterHitImpl>+;
#pragma link C++ class UTIL::CellIDEncoder<IMPL::TrackerDataImpl>+;
#pragma link C++ class UTIL::CellIDEncoder<IMPL::TrackerHitImpl>+;
#pragma link C++ class UTIL::CellIDEncoder<IMPL::TrackerHitPlaneImpl>+;
#pragma link C++ class UTIL::CellIDEncoder<IMPL::TrackerHitZCylinderImpl>+;
#pragma link C++ class UTIL::CellIDEncoder<IMPL::TrackerPulseImpl>+;
#pragma link C++ class UTIL::CellIDEncoder<IMPL::TrackerRawDataImpl>+;
#pragma link C++ class UTIL::CellIDEncoder<IMPL::SimTrackerHitImpl>+;

/*
#pragma link C++ class UTIL::CellIDDecoder<EVENT::CalorimeterHit>+;
#pragma link C++ class UTIL::CellIDDecoder<EVENT::RawCalorimeterHit>+;
#pragma link C++ class UTIL::CellIDDecoder<EVENT::SimCalorimeterHit>+;
#pragma link C++ class UTIL::CellIDDecoder<EVENT::TrackerData>+;
#pragma link C++ class UTIL::CellIDDecoder<EVENT::TrackerHit>+;
#pragma link C++ class UTIL::CellIDDecoder<EVENT::TrackerHitPlane>+;
#pragma link C++ class UTIL::CellIDDecoder<EVENT::TrackerHitZCylinder>+;
#pragma link C++ class UTIL::CellIDDecoder<EVENT::TrackerPulse>+;
#pragma link C++ class UTIL::CellIDDecoder<EVENT::TrackerRawData>+;
#pragma link C++ class UTIL::CellIDDecoder<EVENT::SimTrackerHit>+;
*/

#endif

#endif
