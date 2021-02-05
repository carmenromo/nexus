#ifndef BASICTESTGEOM_
#define BASICTESTGEOM_

#include "BaseGeometry.h"

class G4GenericMessenger;
class G4LogicalVolume;
class G4Material;
namespace nexus {
    class SiPMHamamatsuVUV_Test;
}

namespace nexus {
  class BasicTestGeom : public BaseGeometry {

  public:
    // Constructor
    BasicTestGeom();
    //Destructor
    ~BasicTestGeom();

    /// Generate a vertex within a given region of the geometry
    G4ThreeVector GenerateVertex(const G4String& region) const;

    private:
    void Construct();
    void BuildSensor();
    //void BuildLXe();

    G4LogicalVolume* lab_logic_;
    G4bool visibility_;
    G4double lab_size_;
    G4double source_pos_x_, source_pos_y_, source_pos_z_;
    G4double panel_size_;

    SiPMHamamatsuVUV_Test* sipm_;

    /// Messenger for the definition of control commands
    G4GenericMessenger* msg_;

    //G4LogicalVolume* active_logic_;

  };
}
#endif
