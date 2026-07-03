#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"

class MyTauGraphValidator : public DQMEDAnalyzer {
public:
  explicit MyTauGraphValidator(edm::ParameterSet const&);
  void bookHistograms(DQMStore::IBooker&, edm::Run const&, edm::EventSetup const&) override;
  void analyze(edm::Event const&, edm::EventSetup const&) override;
  static void fillDescriptions(edm::ConfigurationDescriptions&);

private:
};

MyTauGraphValidator::MyTauGraphValidator(edm::ParameterSet const& cfg)
    : graphToken_(consumes<truth::Graph>(cfg.getParameter<edm::InputTag>("src"))) {
}

void MyTauGraphValidator::bookHistograms(DQMStore::IBooker& ib, edm::Run const&, edm::EventSetup const&) {
    ib.setCurrentFolder("BushraFolder");
    nTaus = ib.book1D("nTau", "", 100, 0, 100);
}

void MyTauGraphValidator::analyze(edm::Event const& event, edm::EventSetup const&) {
    nTaus->Fill(1);
}

void MyTauGraphValidator::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("src", edm::InputTag("truthLogicalGraphProducer"));
  descriptions.addWithDefaultLabel(desc);
}

DEFINE_FWK_MODULE(MyTauGraphValidator);