using namespace std;

#include <map>
#include <string>

#include "TH1.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h" 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "SimDataFormats/TruthInfo/interface/Graph.h"

class MyTruthGraphAnalyzer : public edm::one::EDAnalyzer<> {
public:
  explicit MyTruthGraphAnalyzer(const edm::ParameterSet&);
  ~MyTruthGraphAnalyzer() override;
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void beginJob() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override;
  
  std::map<std::string, TH1F*> histContainer_;
  const edm::EDGetTokenT<truth::Graph> graphToken_;
};

MyTruthGraphAnalyzer::MyTruthGraphAnalyzer(const edm::ParameterSet& iConfig)
    : histContainer_(),
  graphToken_(consumes<truth::Graph>(iConfig.getParameter<edm::InputTag>("src"))){}
    

MyTruthGraphAnalyzer::~MyTruthGraphAnalyzer() {}

void MyTruthGraphAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("src", edm::InputTag("truthLogicalGraphProducer"));
  descriptions.addWithDefaultLabel(desc);
}


void MyTruthGraphAnalyzer::beginJob() {
  // register to the TFileService
  edm::Service<TFileService> fs;

  //Electron
  histContainer_["numGenElectron"] = fs->make<TH1F>("numGenElectron", "numGenElectron", 100, 0, 10);
  histContainer_["GenElectronPt"] = fs->make<TH1F>("GenElectronPt", "GenElectronPt", 100, 0, 100);
  histContainer_["GenElectronEta"] = fs->make<TH1F>("GenElectronEta", "GenElectronEta", 500, -10, 10);

  //Photon
  histContainer_["numGenPhoton"] = fs->make<TH1F>("numGenPhoton", "numGenPhoton", 100, 0, 30);
  histContainer_["GenPhotonPt"] = fs->make<TH1F>("GenPhotonPt", "GenPhotonPt", 500, -100, 100);
  histContainer_["GenPhotonEta"] = fs->make<TH1F>("GenPhotonEta", "GenPhotonEta", 500, -10, 10);
  histContainer_["numGenPhotonFromTau"] = fs->make<TH1F>("numGenPhotonFromTau", "numGenPhotonFromTau", 100, 0, 30);

  //Tau
  histContainer_["numGenTau"] = fs->make<TH1F>("numGenTau", "numGenTau", 10, 0, 20);
  histContainer_["GenTauPt"] = fs->make<TH1F>("GenTauPt", "GenTauPt", 100, 0, 100);
  histContainer_["GenTauEta"] = fs->make<TH1F>("GenTauEta", "GenTauEta", 100, -10, 10);
  histContainer_["numTauChildern"] = fs->make<TH1F>("numTauChildern", "numTauChildern", 10, 0, 50);

  //Muon
  histContainer_["numGenMuon"] = fs->make<TH1F>("numGenMuon", "numGenMuon", 100, 0, 10);
  histContainer_["GenMuonPt"] = fs->make<TH1F>("GenMuonPt", "GenMuonPt", 100, -100, 100);
  histContainer_["GenMuonEta"] = fs->make<TH1F>("GenMuonEta", "GenMuonEta", 100, -5, 5);

}

void MyTruthGraphAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) { 
  auto const& graph = iEvent.get(graphToken_);

  int numGenElectron = 0, numGenPhoton = 0, numGenPhotonFromTau = 0, numGenTau = 0, numGenMuon = 0, numTauChildern = 0;
  int TauElectron = 0, TauMuon = 0, TauHadron = 0;
  
  for (const auto& particle : graph.particleViews()){

    if (!particle.valid())
      continue;

    const auto pID = particle.pdgId();
    switch (std::abs(pID)){
      case 11: {//electron
        if ( particle.hasGen()){
          numGenElectron++;
          histContainer_["GenElectronPt"]->Fill(particle.momentum().pt());
          histContainer_["GenElectronEta"]->Fill(particle.momentum().eta());
        }
        break;
      }

      case 22: {// photon
        if (particle.hasGen()){
          numGenPhoton++;
          histContainer_["GenPhotonPt"]->Fill(particle.momentum().pt());
          histContainer_["GenPhotonEta"]->Fill(particle.momentum().eta());

          if (particle.hasAncestorPdgId(15) || particle.hasAncestorPdgId(-15)){
            numGenPhotonFromTau++;
          }
            
        }
        break;
      } 

      case 13: {//muon
        if (particle.hasGen()){
          numGenMuon++;
          histContainer_["GenMuonPt"]->Fill(particle.momentum().pt());
         histContainer_["GenMuonEta"]->Fill(particle.momentum().eta());
        }
        break;
      } 

      case 15: {//Tau
        if (particle.hasGen()){
          numGenTau++;
          histContainer_["GenTauPt"]->Fill(particle.momentum().pt());
          histContainer_["GenTauEta"]->Fill(particle.momentum().eta());
          
          //Tau Decay
          const auto children = particle.children();
          numTauChildern = children.size();
          histContainer_["numTauChildern"]->Fill(numTauChildern);

          for( const auto& child : children){
            const auto childID = std::abs(child.pdgId());

            if (childID == 11){
              TauElectron++;
            } else if (childID == 13){
              TauMuon++;
            } 
            //pion = 111, charged pion = 211, k0l = 130, k0s = 310, charged kaon = 321
            else if( childID == 111 || childID == 211 || childID == 130 || childID == 310 || childID == 321){
              TauHadron++;
            }
          }
        }
        break;
      } 

      default:
        break;
    }
  }

  histContainer_["numGenElectron"]->Fill(numGenElectron);
  histContainer_["numGenPhotonFromTau"]->Fill(numGenPhotonFromTau);
  histContainer_["numGenPhoton"]->Fill(numGenPhoton);
  histContainer_["numGenTau"]->Fill(numGenTau);
  histContainer_["numGenMuon"]->Fill(numGenMuon);

  double electronFraction = TauElectron / numGenTau;
  double muonFraction = TauMuon / numGenTau;
  double hadronFraction = TauHadron / numGenTau;

  std::cout << "Fractions of tau decays: " << endl;
  std::cout << "Electron Fraction: " << electronFraction << endl;
  std::cout << "Muon Fraction: " << muonFraction << endl;
  std::cout << "Hadron Fraction: " << hadronFraction << endl;
}

void MyTruthGraphAnalyzer::endJob() {}

DEFINE_FWK_MODULE(MyTruthGraphAnalyzer);