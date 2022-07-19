#ifndef DSelector_ggkpkm__B4_h
#define DSelector_ggkpkm__B4_h

#include <iostream>

#include "DSelector/DSelector.h"
#include "DSelector/DHistogramActions.h"
#include "DSelector/DCutActions.h"

#include "TH1I.h"
#include "TH2I.h"

class DSelector_ggkpkm__B4 : public DSelector
{
	public:

		DSelector_ggkpkm__B4(TTree* locTree = NULL) : DSelector(locTree){}
		virtual ~DSelector_ggkpkm__B4(){}

		void Init(TTree *tree);
		Bool_t Process(Long64_t entry);

	private:

		void Get_ComboWrappers(void);
		void Finalize(void);

		// BEAM POLARIZATION INFORMATION
		UInt_t dPreviousRunNumber;
		bool dIsPolarizedFlag; //else is AMO
		bool dIsPARAFlag; //else is PERP or AMO

		bool dIsMC;

		// ANALYZE CUT ACTIONS
		// // Automatically makes mass histograms where one cut is missing
		DHistogramAction_AnalyzeCutActions* dAnalyzeCutActions;

		//CREATE REACTION-SPECIFIC PARTICLE ARRAYS

		//Step 0
		DParticleComboStep* dStep0Wrapper;
		DBeamParticle* dComboBeamWrapper;
		DNeutralParticleHypothesis* dPhoton1Wrapper;
		DNeutralParticleHypothesis* dPhoton2Wrapper;
		DChargedTrackHypothesis* dKPlusWrapper;
		DChargedTrackHypothesis* dKMinusWrapper;
		DChargedTrackHypothesis* dProtonWrapper;

		// DEFINE YOUR HISTOGRAMS HERE
		// EXAMPLES:
		TH1I* dHist_MissingMassSquared;
		TH1F* dHist_MissingMassSquared_KinFit;
		
		TH1I* dHist_BeamEnergy;

		TH1F* dHist_Vertex_Z;

		TH1F* dHist_InvariantMass_ggKpKm_KinFit[1];
		TH1F* dHist_InvariantMass_g1g2_KinFit[1];
		TH1F* dHist_InvariantMass_KpKm_KinFit[1];

		TH1F* dHist_InvariantMass_g1Kp_KinFit[1];
		TH1F* dHist_InvariantMass_g1Km_KinFit[1];
		TH1F* dHist_InvariantMass_g2Kp_KinFit[1];
		TH1F* dHist_InvariantMass_g2Km_KinFit[1];

		TH1F* dHist_InvariantMass_Kp_Measured[1];
		TH1F* dHist_InvariantMass_Km_Measured[1];

		TH1F* dHist_InvariantMass_g1g2Kp_KinFit[1];
		TH1F* dHist_InvariantMass_g1g2Km_KinFit[1];

		TH1F* dHist_InvariantMass_KpP_KinFit[1];
		TH1F* dHist_InvariantMass_KmP_KinFit[1];
		TH1F* dHist_InvariantMass_g1g2P_KinFit[1];

		TH2F* dHist_InvariantMass_KpKm_g1g2_KinFit[1];
		TH2F* dHist_InvariantMass_KpKm_g1g2KpKm_KinFit[1];
		TH2F* dHist_InvariantMass_g1g1Kp_g1g2Km_KinFit[1];
		
		TH2F* dHist_KpKm_BeamEnergy_KinFit[1];
		TH2F* dHist_g1g2_BeamEnergy_KinFit[1];

		TH2F* dHist_InvariantMass_g1g2_Chisq_NDF_KinFit[1];
		TH2F* dHist_InvariantMass_KpKm_Chisq_NDF_KinFit[1];


		TH2F* dHist_InvariantMass_ggKpKm_ggPipPim_MissID_KinFit[1];
		TH1F* dHist_InvariantMass_ggPipPim_MissID_KinFit[1];
		TH1F* dHist_InvariantMass_PipPim_MissID_KinFit[1];


		TH1F* dHist_Vertex_X_KinFit[4];
		TH1F* dHist_Vertex_Y_KinFit[4];
		TH1F* dHist_Vertex_Z_KinFit[4];

		TH1F* dHist_Momentum_KinFit[4];


		TH2F* dHist_Dalitz_g1g2Kp_KpKm_KinFit[1];
		TH2F* dHist_Dalitz_g1g2Kp_g1g2Km_KinFit[1];

		TF1 *BetaMax;
                TF1 *BetaMin;


	ClassDef(DSelector_ggkpkm__B4, 0);
};

void DSelector_ggkpkm__B4::Get_ComboWrappers(void)
{
	//Step 0
	dStep0Wrapper = dComboWrapper->Get_ParticleComboStep(0);
	dComboBeamWrapper = static_cast<DBeamParticle*>(dStep0Wrapper->Get_InitialParticle());
	dPhoton1Wrapper = static_cast<DNeutralParticleHypothesis*>(dStep0Wrapper->Get_FinalParticle(0));
	dPhoton2Wrapper = static_cast<DNeutralParticleHypothesis*>(dStep0Wrapper->Get_FinalParticle(1));
	dKPlusWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(2));
	dKMinusWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(3));
	dProtonWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(4));
}



class DCutAction_dEdx_withP : public DAnalysisAction
{
        public:
                DCutAction_dEdx_withP(const DParticleCombo* locParticleComboWrapper, bool locUseKinFitFlag, Particle_t locPID, DetectorSystem_t locSystem = SYS_CDC, string locActionUniqueString = "") :
                        DAnalysisAction(locParticleComboWrapper, "Cut_dEdx", locUseKinFitFlag, locActionUniqueString),
                        dFunc_dEdxCut_Max(NULL), dFunc_dEdxCut_Min(NULL), dPID(locPID), dSystem(locSystem),
                        dMax_c0(0), dMax_c1(0), dMax_c2(999), dMin_c0(0), dMin_c1(0), dMin_c2(-1),
                        dUser_set_max(false), dUser_set_min(false){}

                string Get_ActionName(void) const;
                void Initialize(void);
                void Reset_NewEvent(void){}
                bool Perform_Action(void);

                void Set_Max( double c0, double c1, double c2){
                    if (dFunc_dEdxCut_Max != NULL) cout << " DCutAction_dEdx_withP::Initialize() has already been called! These settings will not take effect." << endl;
                    dMax_c0 = c0, dMax_c1 = c1, dMax_c2 = c2;
                    dUser_set_max = true;
                    return;
                }
                void Set_Min( double c0, double c1, double c2){
                    if (dFunc_dEdxCut_Min != NULL) cout << " DCutAction_dEdx_withP::Initialize() has already been called! These settings will not take effect." << endl;
                    dMin_c0 = c0, dMin_c1 = c1, dMin_c2 = c2;
                    dUser_set_min = true;
                    return;
                }

                void Set_Momentum_Max(double P_max){
                        dMomentum_Max = P_max;
                        dUser_momentum_max = true;
                        return;
                }
                void Set_Momentum_Min(double P_min){
                        dMomentum_Min = P_min;
                        dUser_momentum_min = true;
                        return;
                }

                TF1 *dFunc_dEdxCut_Max;
                TF1 *dFunc_dEdxCut_Min;

        private:

                Particle_t dPID;
                DetectorSystem_t dSystem;
                double dMax_c0, dMax_c1, dMax_c2;
                double dMin_c0, dMin_c1, dMin_c2;
                bool dUser_set_max;
                bool dUser_set_min;

                double dMomentum_Max = 1.0;
                double dMomentum_Min = 0.0;
                bool dUser_momentum_max = false;
                bool dUser_momentum_min = false;
};


string DCutAction_dEdx_withP::Get_ActionName(void) const
{
        ostringstream locStream;
        locStream << DAnalysisAction::Get_ActionName() << "_" << dPID << "_" << dSystem;
        return locStream.str();
}



void DCutAction_dEdx_withP::Initialize(void)
{

        if(dFunc_dEdxCut_Min == NULL)   // Set the minimum acceptable dE/dx, the defaults are for the CDC
        {
                string locFuncName = "df_dEdxCut_Min";
                dFunc_dEdxCut_Min = new TF1(locFuncName.c_str(), "exp(-1.0*[0]*x + [1]) + [2]", 0.0, 12.0);

                // Set default values for K and p, if the user did not use Set_Min.
                if (!dUser_set_min) {
                  if((dPID == KPlus) || (dPID==KMinus)) {
                    dMin_c0 = 5;
                    dMin_c1 = 2.6;
                    dMin_c2 = 0.5;
                  } else if(ParticleMass(dPID) > ParticleMass(KPlus)) {
                    dMin_c0 = 4;
                    dMin_c1 = 3.2;
                    dMin_c2 = 1.0;
                  }
                }

                dFunc_dEdxCut_Min->SetParameters(dMin_c0, dMin_c1, dMin_c2);
        }


        if(dFunc_dEdxCut_Max == NULL)   // Set the maximum acceptable dE/dx, the defaults are for the CDC
        {
                string locFuncName = "df_dEdxCut_Max";
                dFunc_dEdxCut_Max = new TF1(locFuncName.c_str(), "exp(-1.0*[0]*x + [1]) + [2]", 0.0, 12.0);

                // Set default values for pi and K, if the user did not use Set_Max.
                if (!dUser_set_max) {
                  if(ParticleMass(dPID) < ParticleMass(KPlus)) {
                    dMax_c0 = 15;
                    dMax_c1 = 3.5;
                    dMax_c2 = 3.3;
                  } else if((dPID == KPlus) || (dPID==KMinus)) {
                    dMax_c0 = 9;
                    dMax_c1 = 4.7;
                    dMax_c2 = 3.3;
                  }
                }

                dFunc_dEdxCut_Max->SetParameters(dMax_c0, dMax_c1, dMax_c2);
        }

        if(!dUser_momentum_max){
                dMomentum_Max = 12.0;
        }
        if(!dUser_momentum_min){
                dMomentum_Min = 0.0;
        }
}

bool DCutAction_dEdx_withP::Perform_Action(void)
{
        for(size_t loc_i = 0; loc_i < dParticleComboWrapper->Get_NumParticleComboSteps(); ++loc_i)
        {
                DParticleComboStep* locComboWrapperStep = dParticleComboWrapper->Get_ParticleComboStep(loc_i);

                //final particles
                for(size_t loc_j = 0; loc_j < locComboWrapperStep->Get_NumFinalParticles(); ++loc_j)
                {
                        DKinematicData* locKinematicData = locComboWrapperStep->Get_FinalParticle(loc_j);
                        if(locKinematicData == NULL)
                                continue; //e.g. a decaying or missing particle whose params aren't set yet

                        //-2 if detected, -1 if missing, > 0 if decaying (step where it is the parent)
                        int locDecayStepIndex = locComboWrapperStep->Get_DecayStepIndex(loc_j);
                        if(locDecayStepIndex != -2)
                                continue; //not measured

                        Particle_t locPID = locKinematicData->Get_PID();
                        if((dPID != Unknown) && (locPID != dPID))
                                continue;

                        const DChargedTrackHypothesis* locChargedTrackHypothesis = dynamic_cast<const DChargedTrackHypothesis*>(locKinematicData);
                        if(locChargedTrackHypothesis == NULL)
                                continue; //neutral!

                        Float_t locdEdx;
                        if(dSystem == SYS_CDC)
                                locdEdx = locChargedTrackHypothesis->Get_dEdx_CDC()*1.0E6;
                        else if(dSystem == SYS_FDC)
                                locdEdx = locChargedTrackHypothesis->Get_dEdx_FDC()*1.0E6;
                        else if(dSystem == SYS_START)
                                locdEdx = locChargedTrackHypothesis->Get_dEdx_ST()*1.0E3;
                        else if(dSystem == SYS_TOF)
                                locdEdx = locChargedTrackHypothesis->Get_dEdx_TOF()*1.0E3;
                        else
                                continue;

                        if(!(locdEdx > 0.0))
                                continue; // Not enough hits in the detector to report a dE/dx: Don't cut

                        //cut
                        double locP = dUseKinFitFlag ? locChargedTrackHypothesis->Get_P4().Vect().Mag() : locChargedTrackHypothesis->Get_P4_Measured().Vect().Mag();
                        if( (locP < dMomentum_Max)&& (locP > dMomentum_Min) ){
                                if(locdEdx < dFunc_dEdxCut_Min->Eval(locP))
                                        return false;

                                if(locdEdx > dFunc_dEdxCut_Max->Eval(locP))
                                        return false; //focus on keeping signal protons
                        }
                } //end of particle loop
        } //end of step loop

        return true;
}


#endif // DSelector_ggkpkm__B4_h
