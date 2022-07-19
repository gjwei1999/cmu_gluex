#ifndef DSelector_ggkpkm__B4_thrown_h
#define DSelector_ggkpkm__B4_thrown_h

#include <iostream>

#include "DSelector/DSelector.h"

#include "TH1I.h"
#include "TH2I.h"

class DSelector_ggkpkm__B4_thrown : public DSelector
{
	public:

		DSelector_ggkpkm__B4_thrown(TTree* locTree = NULL) : DSelector(locTree){}
		virtual ~DSelector_ggkpkm__B4_thrown(){}

		void Init(TTree *tree);
		Bool_t Process(Long64_t entry);


	private:

		void Finalize(void);

		// BEAM POLARIZATION INFORMATION
		UInt_t dPreviousRunNumber;
		bool dIsPolarizedFlag; //else is AMO
		bool dIsPARAFlag; //else is PERP or AMO

		TH1F* dHist_InvariantMass_ggKpKm_thrown[1];
		TH1F* dHist_InvariantMass_g1g2_thrown[1];
                TH1F* dHist_InvariantMass_KpKm_thrown[1];

		TH2F* dHist_Dalitz_g1g2Kp_KpKm_thrown[1];
                TH2F* dHist_Dalitz_g1g2Kp_g1g2Km_thrown[1];

		//For angular distribution
		//f1285
		//primary HF
		TH1D* h_a0_cosTheta_HF_primary;
		TH1D* h_a0_phi_HF_primary;
		//a0 HF
		TH1D* h_Kp_cosTheta_HF_a0;
		TH1D* h_Kp_phi_HF_a0;
		TH1D* h_Km_cosTheta_HF_a0;
		TH1D* h_Km_phi_HF_a0;

		//f1420
		//primary HF; f1420 -> Kp + Km*, Km* -> K- + pi0
		TH1D* h_Km_star_cosTheta_HF_primary;
		TH1D* h_Km_star_phi_HF_primary;
		//Km* HF
		TH1D* h_Km_cosTheta_HF_Km_star;
		TH1D* h_Km_phi_HF_Km_star;
		TH1D* h_pi0_cosTheta_HF_Km_star;
		TH1D* h_pi0_phi_HF_Km_star;
		//primary HF; f1420 -> Km + Kp*, Kp* -> K+ + pi0
		TH1D* h_Kp_star_cosTheta_HF_primary;
		TH1D* h_Kp_star_phi_HF_primary;
		//Kp* HF
		TH1D* h_Kp_cosTheta_HF_Kp_star;
		TH1D* h_Kp_phi_HF_Kp_star;
		TH1D* h_pi0_cosTheta_HF_Kp_star;
		TH1D* h_pi0_phi_HF_Kp_star;

	ClassDef(DSelector_ggkpkm__B4_thrown, 0);
};

#endif // DSelector_ggkpkm__B4_thrown_h
