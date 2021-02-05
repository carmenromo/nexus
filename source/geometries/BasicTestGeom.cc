// ----------------------------------------------------------------------------
//  $Id: BasicTestGeom.cc  $
//
//  Author:  <carmen.romo@ific.uv.es>
//  Created: May 2020
//
// ----------------------------------------------------------------------------

#include "BasicTestGeom.h"
#include "MaterialsList.h"
#include "Visibilities.h"
#include "SiPMHamamatsuVUV_Test.h"
#include "IonizationSD.h"
#include "OpticalMaterialProperties.h"

#include <G4GenericMessenger.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include <G4NistManager.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4SDManager.hh>

namespace nexus {

  BasicTestGeom::BasicTestGeom(): BaseGeometry(),
                            visibility_(0),
                            lab_size_(10.*cm),
                            source_pos_x_(0.*mm),
                            source_pos_y_(0.*mm),
                            source_pos_z_(0.*mm),
                            panel_size_(10.*cm)
  {
    // Messenger
    msg_ = new G4GenericMessenger(this, "/Geometry/BasicTestGeom/",
                                  "Control commands of geometry BasicTestGeom.");
    msg_->DeclareProperty("visibility", visibility_, "Visibility");

    G4GenericMessenger::Command& lab_size_cmd =
      msg_->DeclareProperty("lab_size", lab_size_, "Size of the lab");
    lab_size_cmd.SetUnitCategory("Length");
    lab_size_cmd.SetParameterName("lab_size", false);
    lab_size_cmd.SetRange("lab_size>0.");

    G4GenericMessenger::Command& source_pos_x_cmd =
      msg_->DeclareProperty("source_pos_x", source_pos_x_, "X position of the source");
    source_pos_x_cmd.SetUnitCategory("Length");
    source_pos_x_cmd.SetParameterName("source_pos_x", false);

    G4GenericMessenger::Command& source_pos_y_cmd =
      msg_->DeclareProperty("source_pos_y", source_pos_y_, "Y position of the source");
    source_pos_y_cmd.SetUnitCategory("Length");
    source_pos_y_cmd.SetParameterName("source_pos_y", false);

    G4GenericMessenger::Command& source_pos_z_cmd =
      msg_->DeclareProperty("source_pos_z", source_pos_z_, "Z position of the source");
    source_pos_z_cmd.SetUnitCategory("Length");
    source_pos_z_cmd.SetParameterName("source_pos_z", false);

    G4GenericMessenger::Command& panel_size_cmd =
      msg_->DeclareProperty("panel_size", panel_size_, "Size of the pyrex panel");
    panel_size_cmd.SetUnitCategory("Length");
    panel_size_cmd.SetParameterName("panel_size", false);
    panel_size_cmd.SetRange("panel_size>0.");

    sipm_ = new SiPMHamamatsuVUV_Test();
  }

  BasicTestGeom::~BasicTestGeom()
  {
  }

  void BasicTestGeom::Construct()
  {
    // LAB. This is just a volume of air surrounding the detector
    G4Box* lab_solid = new G4Box("LAB", lab_size_/2., lab_size_/2., lab_size_/2.);

    G4Material* vacuum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
    vacuum->SetMaterialPropertiesTable(OpticalMaterialProperties::Vacuum());
    lab_logic_ =
      new G4LogicalVolume(lab_solid, vacuum, "LAB");
    //lab_logic_->SetVisAttributes(G4VisAttributes::CopperBrown);

    G4VisAttributes lab_col = nexus::Blue();
      //lab_col.SetForceSolid(true);
      lab_logic_->SetVisAttributes(lab_col);

    this->SetLogicalVolume(lab_logic_);


    // // Mini Box to see the gammas
    // G4double mini_box_size = 1.*mm;
    // G4Box* mini_box_solid = new G4Box("MINIBOX", mini_box_size/2., mini_box_size/2., mini_box_size/2.);
    //
    // G4Material* air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
    // G4LogicalVolume* mini_box_logic = new G4LogicalVolume(mini_box_solid, air, "MINI_BOX");
    // G4VisAttributes mini_box_col = nexus::Blue();
    // mini_box_logic->SetVisAttributes(mini_box_col);
    // 
    // new G4PVPlacement(0, G4ThreeVector(source_pos_x_, source_pos_y_, source_pos_z_), mini_box_logic,
    //                   "MINI_BOX", lab_logic_, false, 0, true);


    // PYREX PANEL
    G4double panel_thickness = 2.*mm;
    G4Box* panel_solid =
      new G4Box("PANEL_SiPMs", panel_size_/2., panel_size_/2., panel_thickness/2.);

    G4Material* pyrex = G4NistManager::Instance()->FindOrBuildMaterial("G4_Pyrex_Glass");
    pyrex->SetMaterialPropertiesTable(OpticalMaterialProperties::Pyrex_vidrasa(panel_thickness));
    G4LogicalVolume* panel_logic =
      new G4LogicalVolume(panel_solid, pyrex, "PANEL_SiPMs");

    new G4PVPlacement(0, G4ThreeVector(0., 0., (-lab_size_*2)/5.),
                      panel_logic, "PANEL", lab_logic_, false, 1, true);

    BuildSensor();
  }


  void BasicTestGeom::BuildSensor()
  {
    sipm_->SetSensorDepth(0);
    sipm_->SetMotherDepth(1);
    sipm_->Construct();

    G4double sipm_thickn = sipm_->GetDimensions().z();
    G4LogicalVolume* sipm_logic = sipm_->GetLogicalVolume();

    new G4PVPlacement(0, G4ThreeVector(0., 0., (-lab_size_+sipm_thickn)/2.), sipm_logic,
                      "SiPM", lab_logic_, false, 0, true);
  }


  G4ThreeVector BasicTestGeom::GenerateVertex(const G4String& region) const
  {
    G4ThreeVector vertex(0., 0., 0.);

    if (region == "CENTER") {
        return vertex;

    } else if (region == "AD_HOC") {
        vertex = G4ThreeVector(source_pos_x_, source_pos_y_, source_pos_z_);
    } else {
      G4Exception("[BasicTestGeom]", "GenerateVertex()", FatalException,
                  "Unknown vertex generation region!");
    }
    return vertex;
  }

 }
