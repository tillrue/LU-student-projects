/**
 * @file ParticleFlow.h
 * @brief Simple PFlow algorithm
 * @author Christian Herwig, Fermilab
 */

#ifndef PARTICLEFLOW_H
#define PARTICLEFLOW_H

// LDMX Framework
#include "Ecal/Event/EcalCluster.h"
#include "Framework/Configure/Parameters.h"  // Needed to import parameters from configuration file
#include "Framework/Event.h"
#include "Framework/EventProcessor.h"  //Needed to declare processor
#include "Hcal/Event/HcalCluster.h"
#include "Recon/Event/CaloCluster.h"
#include "Recon/Event/PFCandidate.h"
#include "SimCore/Event/SimParticle.h"
#include "SimCore/Event/SimTrackerHit.h"
#include "TGraph.h"

namespace recon {

/**
 * @class ParticleFlow
 * @brief
 */
class ParticleFlow : public framework::Producer {
 public:
  ParticleFlow(const std::string& name, framework::Process& process)
      : framework::Producer(name, process) {}

  virtual void configure(framework::config::Parameters& ps);

  virtual void produce(framework::Event& event);

  virtual void onProcessEnd();

  void fillCandTrack(ldmx::PFCandidate& cand, const ldmx::SimTrackerHit& tk);
  void fillCandEMCalo(ldmx::PFCandidate& cand, const ldmx::CaloCluster& em);
  void fillCandHadCalo(ldmx::PFCandidate& cand, const ldmx::CaloCluster& had);

 private:
  TGraph* eCorr_{0};
  TGraph* hCorr_{0};

  // name of collection for PF inputs to be passed
  std::string inputEcalCollName_;
  std::string inputHcalCollName_;
  std::string inputTrackCollName_;
  // name of collection for PF outputs
  std::string outputCollName_;
  // configuration
  bool singleParticle_;

  // matching criteria for Track + (side) HCal cluster matching
  double tkHadCaloMatchDist_;
  double tkHadCaloMinEnergyRatio_;
  double tkHadCaloMaxEnergyRatio_;

  double distTkHcalMatch;

};
}  // namespace recon

#endif /* PARTICLEFLOW_H */
