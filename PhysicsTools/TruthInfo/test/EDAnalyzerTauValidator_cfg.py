import FWCore.ParameterSet.Config as cms

process = cms.Process("Test")

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
      # Multiple file should be comma separated
      # This is the format for using a remote file
      'file:/afs/cern.ch/cms/Tutorials/CMSDASatCERN24/2024/pre_exercises/Set4/Input/Muon/slimMiniAOD_data_MuEle_1.root',
      # The format for using a local file can be found in the commented line below
      # 'file:slimMiniAOD_data_MuEle_1.root'
  )
)

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.maxEvents = cms.untracked.PSet( 
    input = cms.untracked.int32(-1) 
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('EDAnalyzerTauValidator.root')
                                   )

process.EDAnalyzerTauValidator = cms.EDAnalyzer(
    "EDAnalyzerTauValidator",
    src=cms.InputTag("truthLogicalGraphProducer"),
)

process.p = cms.Path(process.EDAnalyzerTauValidator)
