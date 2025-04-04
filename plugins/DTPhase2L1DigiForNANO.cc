// -*- C++ -*-
//
// Package:    L1Trigger/DTTriggerPhase2
// Class:      DTPhase2L1DigiForNANO
//
/**\class DTPhase2L1DigiForNANO DTPhase2L1DigiForNANO.cc L1Trigger/DTTriggerPhase2/plugins/DTPhase2L1DigiForNANO.cc

Description: Conversor to facilitate adding the DT Phase2 Digis to L1NANO

Implementation:
[Notes on implementation]
*/
//

// system include files
#include <memory>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "CalibMuon/DTDigiSync/interface/DTTTrigBaseSync.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/L1DTTrackFinder/interface/L1Phase2MuDTPhDigi.h"
#include "DataFormats/L1DTTrackFinder/interface/L1Phase2MuDTThDigi.h"
#include "DataFormats/L1DTTrackFinder/interface/L1Phase2MuDTThContainer.h"
#include "DataFormats/L1DTTrackFinder/interface/L1Phase2MuDTPhContainer.h"

#include "DataFormats/L1DTTrackFinder/interface/L1Phase2MuDTExtPhDigi.h"
#include "DataFormats/L1DTTrackFinder/interface/L1Phase2MuDTExtThDigi.h"
#include "DataFormats/L1DTTrackFinder/interface/L1Phase2MuDTExtThContainer.h"
#include "DataFormats/L1DTTrackFinder/interface/L1Phase2MuDTExtPhContainer.h"

#include "L1Trigger/DTTriggerPhase2/interface/constants.h"

namespace edm {
	class ParameterSet;
	class EventSetup;
}  // namespace edm

using namespace std;
using namespace edm;
using namespace cmsdt;
//
// class declaration
//

class DTPhase2L1DigiForNANO : public edm::stream::EDProducer<> {
	public:
		explicit DTPhase2L1DigiForNANO(const edm::ParameterSet&);
		~DTPhase2L1DigiForNANO() override;

	private:
		void produce(edm::Event&, const edm::EventSetup&) override;
		// ----------member data ---------------------------
		edm::EDGetTokenT<L1Phase2MuDTPhContainer> dtPhiToken_;
		edm::EDGetTokenT<L1Phase2MuDTThContainer> dtThetaToken_;
		edm::EDGetTokenT<L1Phase2MuDTExtPhContainer> dtExtPhiToken_;
		edm::EDGetTokenT<L1Phase2MuDTExtThContainer> dtExtThetaToken_;

};

//
// constructors and destructor
//
DTPhase2L1DigiForNANO::DTPhase2L1DigiForNANO(const edm::ParameterSet& iConfig) {
	//register your products
	dtPhiToken_   = consumes<L1Phase2MuDTPhContainer>(iConfig.getParameter<edm::InputTag>("muDTPhiDigiToken"));
	dtThetaToken_ = consumes<L1Phase2MuDTThContainer>(iConfig.getParameter<edm::InputTag>("muDTThetaDigiToken"));
	dtExtPhiToken_   = consumes<L1Phase2MuDTExtPhContainer>(iConfig.getParameter<edm::InputTag>("muDTExtPhiDigiToken"));
	dtExtThetaToken_ = consumes<L1Phase2MuDTExtThContainer>(iConfig.getParameter<edm::InputTag>("muDTExtThetaDigiToken"));

	produces< std::vector<L1Phase2MuDTPhDigi> >("muL1P2DTDigiPhis");
	produces< std::vector<L1Phase2MuDTThDigi> >("muL1P2DTDigiThetas");
	produces< std::vector<L1Phase2MuDTExtPhDigi> >("muL1P2DTExtDigiPhis");
        produces< std::vector<L1Phase2MuDTExtThDigi> >("muL1P2DTExtDigiThetas");

	//now do what ever other initialization is needed
}

DTPhase2L1DigiForNANO::~DTPhase2L1DigiForNANO() {
	// do anything here that needs to be done at destruction time
	// (e.g. close files, deallocate resources etc.)
}

//
// member functions
//

// ------------ method called to produce the data  ------------
void DTPhase2L1DigiForNANO::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

	// Input format of Primitives:
	edm::Handle<L1Phase2MuDTPhContainer> muDTPhis;
	iEvent.getByToken(dtPhiToken_, muDTPhis);

	edm::Handle<L1Phase2MuDTThContainer> muDTThetas;
	iEvent.getByToken(dtThetaToken_, muDTThetas);

	edm::Handle<L1Phase2MuDTExtPhContainer> muDTExtPhis;
        iEvent.getByToken(dtExtPhiToken_, muDTExtPhis);

        edm::Handle<L1Phase2MuDTExtThContainer> muDTExtThetas;
        iEvent.getByToken(dtExtThetaToken_, muDTExtThetas);

        // Output format of Primitives, so that NANOAOD reads them easily
	std::vector<L1Phase2MuDTPhDigi> dtPhis;
	std::vector<L1Phase2MuDTThDigi> dtThetas;
	std::vector<L1Phase2MuDTExtPhDigi> dtExtPhis;
        std::vector<L1Phase2MuDTExtThDigi> dtExtThetas;


        // Copy...
	if (muDTPhis.isValid()){

		const auto trigs = muDTPhis->getContainer();
		for(const auto & trig : (*trigs)) {
			dtPhis.push_back(trig);
		}
	}
	if (muDTThetas.isValid()){

		const auto trigs = muDTThetas->getContainer();
		for(const auto & trig : (*trigs)) {
			dtThetas.push_back(trig);
		}
	}
	if (muDTExtPhis.isValid()){

                const auto trigs = muDTExtPhis->getContainer();
                for(const auto & trig : (*trigs)) {
                        dtExtPhis.push_back(trig);
                }
        }
        if (muDTExtThetas.isValid()){

                const auto trigs = muDTExtThetas->getContainer();
                for(const auto & trig : (*trigs)) {
                        dtExtThetas.push_back(trig);
                }
        }

        // Put in the event 
	std::unique_ptr<std::vector<L1Phase2MuDTPhDigi> > outPhi_ptr = std::make_unique<std::vector<L1Phase2MuDTPhDigi> >(dtPhis);
	iEvent.put(std::move(outPhi_ptr), "muL1P2DTDigiPhis");

	std::unique_ptr<std::vector<L1Phase2MuDTThDigi> > outTheta_ptr = std::make_unique<std::vector<L1Phase2MuDTThDigi> >(dtThetas);
	iEvent.put(std::move(outTheta_ptr), "muL1P2DTDigiThetas");

	std::unique_ptr<std::vector<L1Phase2MuDTExtPhDigi> > outExtPhi_ptr = std::make_unique<std::vector<L1Phase2MuDTExtPhDigi> >(dtExtPhis);
        iEvent.put(std::move(outExtPhi_ptr), "muL1P2DTExtDigiPhis");

        std::unique_ptr<std::vector<L1Phase2MuDTExtThDigi> > outExtTheta_ptr = std::make_unique<std::vector<L1Phase2MuDTExtThDigi> >(dtExtThetas);
        iEvent.put(std::move(outExtTheta_ptr), "muL1P2DTExtDigiThetas");

}

//define this as a plug-in
DEFINE_FWK_MODULE(DTPhase2L1DigiForNANO);
