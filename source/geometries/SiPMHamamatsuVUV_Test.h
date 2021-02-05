// ----------------------------------------------------------------------------
// nexus | SiPMHamamatsuVUV_Test.cc
//
// 6x6 mm2 VUV Hamamatsu SiPM geometry.
//
// The NEXT Collaboration
// ----------------------------------------------------------------------------

#ifndef SIPM_pet_VUV_new_H
#define SIPM_pet_VUV_new_H

#include "BaseGeometry.h"
#include <G4ThreeVector.hh>

class G4GenericMessenger;

namespace nexus {

  class SiPMHamamatsuVUV_Test: public BaseGeometry
  {
  public:
    /// Constructor
    SiPMHamamatsuVUV_Test();
    /// Destructor
    ~SiPMHamamatsuVUV_Test();

    /// Invoke this method to build the volumes of the geometry
    void Construct();

    void SetVisibility(G4bool vis);
    void SetTimeBinning (G4double time_binning);
    void SetSensorDepth (G4int sensor_depth);
    void SetMotherDepth (G4int mother_depth);
    void SetNamingOrder (G4int naming_order);


  private:
    G4bool visibility_;

    G4double time_binning_;
    G4double sipm_size_;
    G4int    sensor_depth_;
    G4int    mother_depth_;
    G4int    naming_order_;

    /// Messenger for the definition of control commands
    G4GenericMessenger* msg_;

  };

  inline void SiPMHamamatsuVUV_Test::SetVisibility(G4bool vis)
  { visibility_ = vis; }

  inline void SiPMHamamatsuVUV_Test::SetTimeBinning(G4double time_binning)
  { time_binning_ = time_binning; }

  inline void SiPMHamamatsuVUV_Test::SetSensorDepth(G4int sensor_depth)
  { sensor_depth_ = sensor_depth; }

  inline void SiPMHamamatsuVUV_Test::SetMotherDepth(G4int mother_depth)
  { mother_depth_ = mother_depth; }

  inline void SiPMHamamatsuVUV_Test::SetNamingOrder(G4int naming_order)
  { naming_order_ = naming_order; }


} // end namespace nexus

#endif
