#include <map>
#include <string>

#include "TH1.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h" 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

class EDAnalyzerTauValidator : public edm::one::EDAnalyzer<> {
public:
  explicit EDAnalyzerTauValidator(const edm::ParameterSet&);
  ~EDAnalyzerTauValidator() override;
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void beginJob() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override;
  
  TH1F* histContainer_;
  // simple map to contain all histograms;
  // histograms are booked in the beginJob()
  // method
};

EDAnalyzerTauValidator::EDAnalyzerTauValidator(const edm::ParameterSet& iConfig)
    //: histContainer_() {}
    : histContainer_(){}

EDAnalyzerTauValidator::~EDAnalyzerTauValidator() {}

void EDAnalyzerTauValidator::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("src", edm::InputTag("truthLogicalGraphProducer"));
  descriptions.addWithDefaultLabel(desc);
}

void EDAnalyzerTauValidator::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) { 
  histContainer_->Fill(1);

}

void EDAnalyzerTauValidator::beginJob() {
  // register to the TFileService
  edm::Service<TFileService> fs;

  histContainer_ = fs->make<TH1F>("nTau", "", 100, 0, 100);

}

void EDAnalyzerTauValidator::endJob() {}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(EDAnalyzerTauValidator);