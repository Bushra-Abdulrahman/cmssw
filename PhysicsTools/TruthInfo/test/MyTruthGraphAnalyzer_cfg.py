import FWCore.ParameterSet.Config as cms
from Validation.Configuration.truthPrevalidation_cff import truthGraphProducer

process = cms.Process("Test")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Validation.Configuration.truthPrevalidation_cff")


process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
      # Multiple file should be comma separated
      # This is the format for using a remote file
      'file:/eos/user/b/balmutra/CMSSW_20_0_0_pre1/src/step3_yufei.root',
      # The format for using a local file can be found in the commented line below
      # 'file:slimMiniAOD_data_MuEle_1.root'
  )
)

#process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.maxEvents = cms.untracked.PSet( 
    input = cms.untracked.int32(-1) 
)

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string('file:/eos/user/b/balmutra/TenTauTruthGraph/MyTruthGraphAnalyzer.root')
)


process.MyTruthGraphAnalyzer = cms.EDAnalyzer(
    "MyTruthGraphAnalyzer",
    src=cms.InputTag("truthLogicalGraphProducer")
)

process.p = cms.Path(process.truthGraphPrevalidation + process.MyTruthGraphAnalyzer)