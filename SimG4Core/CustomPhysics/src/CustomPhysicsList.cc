#include "SimG4Core/CustomPhysics/interface/CustomPhysicsList.h"
#include "SimG4Core/CustomPhysics/interface/CustomParticleFactory.h"
#include "SimG4Core/CustomPhysics/interface/CustomParticle.h"
#include "SimG4Core/CustomPhysics/interface/DummyChargeFlipProcess.h"
#include "SimG4Core/CustomPhysics/interface/G4ProcessHelper.h"
#include "SimG4Core/CustomPhysics/interface/CustomPDGParser.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "G4Decay.hh"
#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4ProcessManager.hh"

#include "SimG4Core/CustomPhysics/interface/FullModelHadronicProcess.h"
#include "SimG4Core/CustomPhysics/interface/CMSDarkPairProductionProcess.h"

using namespace CLHEP;

G4ThreadLocal std::unique_ptr<G4Decay> CustomPhysicsList::fDecayProcess;
G4ThreadLocal std::unique_ptr<G4ProcessHelper> CustomPhysicsList::myHelper;

CustomPhysicsList::CustomPhysicsList(const std::string& name, const edm::ParameterSet & p)  
  :  G4VPhysicsConstructor(name) 
{  
  myConfig = p;
  dfactor = p.getParameter<double>("dark_factor");
  edm::FileInPath fp = p.getParameter<edm::FileInPath>("particlesDef");
  fHadronicInteraction = p.getParameter<bool>("rhadronPhysics");

  particleDefFilePath = fp.fullPath();

  fParticleFactory.reset(new CustomParticleFactory());
  fDecayProcess.reset(nullptr);
  myHelper.reset(nullptr);

  edm::LogInfo("SimG4CoreCustomPhysics") 
    << "CustomPhysicsList: Path for custom particle definition file: \n"
    <<particleDefFilePath << "\n" << "      dark_factor= " << dfactor;
}

CustomPhysicsList::~CustomPhysicsList() {
}
 
void CustomPhysicsList::ConstructParticle(){
  edm::LogInfo("SimG4CoreCustomPhysics") 
    << "===== CustomPhysicsList::ConstructParticle ";
  fParticleFactory.get()->loadCustomParticles(particleDefFilePath);
}
 
void CustomPhysicsList::ConstructProcess() {
  
  edm::LogInfo("SimG4CoreCustomPhysics") 
    <<"CustomPhysicsList: adding CustomPhysics processes "
    << "for the list of particles";

  fDecayProcess.reset(new G4Decay());
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

  for(auto particle : fParticleFactory.get()->GetCustomParticles()) {
    CustomParticle* cp = dynamic_cast<CustomParticle*>(particle);
    if(cp) {
      G4ProcessManager* pmanager = particle->GetProcessManager();
      edm::LogInfo("SimG4CoreCustomPhysics") 
	<<"CustomPhysicsList: " << particle->GetParticleName()
	<<"  PDGcode= " << particle->GetPDGEncoding()
	<< "  Mass= " << particle->GetPDGMass()/GeV  <<" GeV.";
      if(pmanager) {
	if(particle->GetPDGCharge() != 0.0) {
	  ph->RegisterProcess(new G4hMultipleScattering, particle);
	  ph->RegisterProcess(new G4hIonisation, particle);
	}
	if(fDecayProcess.get()->IsApplicable(*particle)) {
	  ph->RegisterProcess(fDecayProcess.get(), particle);
	}
	if(cp->GetCloud() && fHadronicInteraction && 
	   CustomPDGParser::s_isRHadron(particle->GetPDGEncoding())) {
	  edm::LogInfo("SimG4CoreCustomPhysics") 
	    <<"CustomPhysicsList: " << particle->GetParticleName()
	    <<" CloudMass= " <<cp->GetCloud()->GetPDGMass()/GeV
	    <<" GeV; SpectatorMass= " << cp->GetSpectator()->GetPDGMass()/GeV<<" GeV.";
       
          if(!myHelper.get()) { myHelper.reset(new G4ProcessHelper(myConfig, fParticleFactory.get())); }
	  pmanager->AddDiscreteProcess(new FullModelHadronicProcess(myHelper.get()));
	}
        if(particle->GetParticleType()=="darkpho"){
          CMSDarkPairProductionProcess * darkGamma = new CMSDarkPairProductionProcess(dfactor);
          pmanager->AddDiscreteProcess(darkGamma);
        }
      }
    }
  }
}
