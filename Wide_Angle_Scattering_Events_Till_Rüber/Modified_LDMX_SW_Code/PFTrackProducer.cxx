#include "Recon/PFTrackProducer.h"

#include "SimCore/Event/SimParticle.h"
#include "SimCore/Event/SimTrackerHit.h"

namespace recon {

void PFTrackProducer::configure(framework::config::Parameters& ps) {
  inputTrackCollName_ = ps.getParameter<std::string>("inputTrackCollName");
  input_pass_name_ = ps.getParameter<std::string>("input_pass_name");
  outputTrackCollName_ = ps.getParameter<std::string>("outputTrackCollName");
}

double getP(const ldmx::SimTrackerHit& tk) {
  std::vector<double> pxyz = tk.getMomentum();
  return sqrt(pow(pxyz[0], 2) + pow(pxyz[1], 2) + pow(pxyz[2], 2));
}

void PFTrackProducer::produce(framework::Event& event) {
  if (!event.exists(inputTrackCollName_)) {
    ldmx_log(fatal) << "Input track collection not found";
    return;
  }
  const auto ecalSpHits = event.getCollection<ldmx::SimTrackerHit>(
      inputTrackCollName_, input_pass_name_);

  const auto hcalSpHits = event.getCollection<ldmx::SimTrackerHit>(
      "HcalScoringPlaneHits", input_pass_name_);

  std::vector<ldmx::SimTrackerHit> pfTracks;
  if (truthTracking_) {
    for (const auto& spHit : ecalSpHits) {
      if (spHit.getTrackID() != 1 || fabs(240 - spHit.getPosition()[2]) > 0.1 ||
          spHit.getMomentum()[2] <= 0)
        continue;
      if (spHit.getPdgID() == 22 || spHit.getPdgID() == 2112) continue;
      pfTracks.push_back(spHit);
      break;
    }
    
    for (const auto& spHit : hcalSpHits) {
      if (spHit.getTrackID() != 1 || fabs(240 - spHit.getPosition()[2]) > 0.1 ||
          spHit.getMomentum()[2] <= 0)
        continue;
      if (spHit.getPdgID() == 22 || spHit.getPdgID() == 2112) continue;
      pfTracks.push_back(spHit);
      break;
    }
  }
  std::sort(pfTracks.begin(), pfTracks.end(),
            [](ldmx::SimTrackerHit a, ldmx::SimTrackerHit b) {
              return getP(a) > getP(b);
            });
  event.add(outputTrackCollName_, pfTracks);
}
}  // namespace recon

DECLARE_PRODUCER_NS(recon, PFTrackProducer);
