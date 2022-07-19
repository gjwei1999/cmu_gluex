#include "DSelector_ggkpkm__B4.h"

void DSelector_ggkpkm__B4::Init(TTree *locTree)
{
	// USERS: IN THIS FUNCTION, ONLY MODIFY SECTIONS WITH A "USER" OR "EXAMPLE" LABEL. LEAVE THE REST ALONE.

	// The Init() function is called when the selector needs to initialize a new tree or chain.
	// Typically here the branch addresses and branch pointers of the tree will be set.
	// Init() will be called many times when running on PROOF (once per file to be processed).

	//USERS: SET OUTPUT FILE NAME //can be overriden by user in PROOF
	dOutputFileName = "hist_ggkpkm__B4.root"; //"" for none
	dOutputTreeFileName = "tree_ggkpkm__B4.root"; //"" for none
	dFlatTreeFileName = ""; //output flat tree (one combo per tree entry), "" for none
	dFlatTreeName = ""; //if blank, default name will be chosen
	//dSaveDefaultFlatBranches = true; // False: don't save default branches, reduce disk footprint.
	//dSaveTLorentzVectorsAsFundamentaFlatTree = false; // Default (or false): save particles as TLorentzVector objects. True: save as four doubles instead.

	//Because this function gets called for each TTree in the TChain, we must be careful:
		//We need to re-initialize the tree interface & branch wrappers, but don't want to recreate histograms
	bool locInitializedPriorFlag = dInitializedFlag; //save whether have been initialized previously
	DSelector::Init(locTree); //This must be called to initialize wrappers for each new TTree
	//gDirectory now points to the output file with name dOutputFileName (if any)
	if(locInitializedPriorFlag)
		return; //have already created histograms, etc. below: exit

	Get_ComboWrappers();
	dPreviousRunNumber = 0;

	/*********************************** EXAMPLE USER INITIALIZATION: ANALYSIS ACTIONS **********************************/

	// EXAMPLE: Create deque for histogramming particle masses:
	// // For histogramming the phi mass in phi -> K+ K-
	// // Be sure to change this and dAnalyzeCutActions to match reaction
	std::deque<Particle_t> MyPhi;
	MyPhi.push_back(KPlus); MyPhi.push_back(KMinus);

	//ANALYSIS ACTIONS: //Executed in order if added to dAnalysisActions
	//false/true below: use measured/kinfit data

	
	dAnalysisActions.push_back(new DHistogramAction_KinFitResults(dComboWrapper, "Before_Chi2cut"));
	dAnalysisActions.push_back(new DHistogramAction_ParticleID(dComboWrapper, true, "Before_Chi2cut"));
	dAnalysisActions.push_back(new DHistogramAction_ParticleComboKinematics(dComboWrapper, true, "Before_Chi2cut"));
	dAnalysisActions.push_back(new DHistogramAction_MissingMassSquared(dComboWrapper, false, 1000, -0.1, 0.1, "Before_Chi2cut"));
	dAnalysisActions.push_back(new DHistogramAction_PIDFOM(dComboWrapper, "Before_Chi2cut"));

	dAnalysisActions.push_back(new DCutAction_KinFitChiSq(dComboWrapper, 3.5, ""));

	dAnalysisActions.push_back(new DHistogramAction_KinFitResults(dComboWrapper, "After_Chi2cut"));
	dAnalysisActions.push_back(new DHistogramAction_ParticleID(dComboWrapper, true, "After_Chi2cut"));
	dAnalysisActions.push_back(new DHistogramAction_ParticleComboKinematics(dComboWrapper, true, "After_Chi2cut"));
	dAnalysisActions.push_back(new DHistogramAction_MissingMassSquared(dComboWrapper, false, 1000, -0.1, 0.1, "After_Chi2cut"));
	dAnalysisActions.push_back(new DHistogramAction_PIDFOM(dComboWrapper, "After_Chi2cut"));



	dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, true, 0.5, KPlus, SYS_BCAL));
	dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, true, 0.5, KMinus, SYS_BCAL));
	dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, true, 1.0, Proton, SYS_BCAL));


	dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, true, 2.0, KPlus, SYS_FCAL));
	dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, true, 2.0, KMinus, SYS_FCAL));
	dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, true, 1.0, Proton, SYS_FCAL));


	dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, true, 0.3, KPlus, SYS_TOF));
	dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, true, 0.3, KMinus, SYS_TOF));
	dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, true, 0.3, Proton, SYS_TOF));


	
	//TOF Beta
	BetaMax = new TF1("betap1","[0] + [1] * exp([2]*x)",0,10);
	BetaMin = new TF1("betap2","[0] + [1] * exp([2]*x)",0,10);

	BetaMin->SetParameters(0.95,-1,-2);
	BetaMax->SetParameters(1.05,-1,-2.5);

	DCutAction_PIDBeta *myBetaCutKp = new DCutAction_PIDBeta(dComboWrapper, true, 0, 2, KPlus, SYS_TOF);
	myBetaCutKp->dFunc_BetaCut_SelectPositive = BetaMax; // this removes anything above the cut line
	myBetaCutKp->dFunc_BetaCut_SelectNegative = BetaMin; // this removes anything above the cut line
	dAnalysisActions.push_back(myBetaCutKp);

	DCutAction_PIDBeta *myBetaCutKm = new DCutAction_PIDBeta(dComboWrapper, true, 0, 2, KMinus, SYS_TOF);
	myBetaCutKm->dFunc_BetaCut_SelectPositive = BetaMax; // this removes anything above the cut line
	myBetaCutKm->dFunc_BetaCut_SelectNegative = BetaMin; // this removes anything below the cut line
	dAnalysisActions.push_back(myBetaCutKm);



	//ST dEdx
	//STdEdx_Min = new TF1("STdEdx_min","exp(-1.0*[0]*x + [1]) + [2]", 0.0, 1.0);

	//STdEdx_Min->SetParameters(5, 2.4, 0.6);

	DCutAction_dEdx_withP *dCutSTdedxKp = new DCutAction_dEdx_withP(dComboWrapper, true, KPlus, SYS_START,"dCutSTdedxKp");
	//dCutSTdedxKp->dFunc_dEdxCut_Min = STdEdx_Min;
	dCutSTdedxKp->Set_Min(5, 2.4, 0.6);   //(5.0, 3.5, 0.1, 1.2);
	dCutSTdedxKp->Set_Max(9, 4.9, 2.5); //Max is set as (9, 4.7, 3.3) by defaults for Kaons
	dCutSTdedxKp->Set_Momentum_Max(1.0);
	dCutSTdedxKp->Set_Momentum_Min(0.0);
	dAnalysisActions.push_back(dCutSTdedxKp);

	DCutAction_dEdx_withP *dCutSTdedxKm = new DCutAction_dEdx_withP(dComboWrapper, true, KMinus, SYS_START,"dCutSTdedxKm");
	//dCutSTdedxKm->dFunc_dEdxCut_Min = STdEdx_Min;
	dCutSTdedxKm->Set_Min(5, 2.4, 0.6);   //(5.0, 3.5, 0.1, 1.2);
	dCutSTdedxKm->Set_Max(9, 4.9, 2.5); //Max is set as (9, 4.7, 3.3) by defaults for Kaons
	dCutSTdedxKm->Set_Momentum_Max(1.0);
	dCutSTdedxKm->Set_Momentum_Min(0.0);
	dAnalysisActions.push_back(dCutSTdedxKm);

	DCutAction_dEdx *dCutSTdedxP = new DCutAction_dEdx(dComboWrapper, true, Proton, SYS_START,"dCutSTdedxP");
	dCutSTdedxP->Set_Min(3.5, 2.3, 0.9);
	dAnalysisActions.push_back(dCutSTdedxP);




	//CDC dEdx
	//CDCMax = new TF1("cdc_max", "exp(-1.0*[0]*x + [1]) + [2]", 0.0, 1.0);
	//CDCMin = new TF1("cdc_min", "exp(-1.0*[0]*x + [1]) + [2]", 0.0, 1.0);

	//CDCMax->SetParameters(9, 4.7, 3.3);
	//CDCMin->SetParameters(5, 2.6, 0.5);

	DCutAction_dEdx_withP *dCutCDCdedxKp = new DCutAction_dEdx_withP(dComboWrapper, true, KPlus, SYS_CDC,"dCutCDCdedxKp");
	//dCutCDCdedxKp->dFunc_dEdxCut_Max = CDCMax;
	//dCutCDCdedxKp->dFunc_dEdxCut_Min = CDCMin;
	dCutCDCdedxKp->Set_Min(5, 2.6, 0.5);
	dCutCDCdedxKp->Set_Max(9, 4.7, 3.3);
	dCutCDCdedxKp->Set_Momentum_Min(0.0);
	dCutCDCdedxKp->Set_Momentum_Max(1.0);
	dAnalysisActions.push_back(dCutCDCdedxKp);

	DCutAction_dEdx_withP *dCutCDCdedxKm = new DCutAction_dEdx_withP(dComboWrapper, true, KMinus, SYS_CDC,"dCutCDCdedxKm");
	//dCutCDCdedxKm->dFunc_dEdxCut_Max = CDCMax;
	//dCutCDCdedxKm->dFunc_dEdxCut_Min = CDCMin;
	dCutCDCdedxKm->Set_Min(5, 2.6, 0.5);
	dCutCDCdedxKm->Set_Max(9, 4.7, 3.3);
	dCutCDCdedxKm->Set_Momentum_Min(0.0);
	dCutCDCdedxKm->Set_Momentum_Max(1.0);
	dAnalysisActions.push_back(dCutCDCdedxKm);

	DCutAction_dEdx *dCutCDCdedxP = new DCutAction_dEdx(dComboWrapper, true, Proton, SYS_CDC,"dCutCDCdedxP");
	dCutCDCdedxP->Set_Min(4.0, 3.2, 1.0);
	dAnalysisActions.push_back(dCutCDCdedxP);



	//FDC dEdx
	DCutAction_dEdx *dCutFDCdedx = new DCutAction_dEdx(dComboWrapper, true, Proton, SYS_FDC,"dCutFDCdedx");
	dCutFDCdedx->Set_Min(5.2, 3.3, 0.6);
	dAnalysisActions.push_back(dCutFDCdedx);


	dAnalysisActions.push_back(new DHistogramAction_KinFitResults(dComboWrapper, "After_cut"));
	dAnalysisActions.push_back(new DHistogramAction_ParticleID(dComboWrapper, true, "After_cut"));
	dAnalysisActions.push_back(new DHistogramAction_ParticleComboKinematics(dComboWrapper, true, "After_cut"));
	dAnalysisActions.push_back(new DHistogramAction_MissingMassSquared(dComboWrapper, false, 1000, -0.1, 0.1, "After_cut"));
	dAnalysisActions.push_back(new DHistogramAction_PIDFOM(dComboWrapper, "After_cut"));




	//PID
	//dAnalysisActions.push_back(new DHistogramAction_ParticleID(dComboWrapper, false));
	//below: value: +/- N ns, Unknown: All PIDs, SYS_NULL: all timing systems
	//dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, false, 0.5, KPlus, SYS_BCAL));

	//PIDFOM (for charged tracks)
	//dAnalysisActions.push_back(new DHistogramAction_PIDFOM(dComboWrapper));
	//dAnalysisActions.push_back(new DCutAction_PIDFOM(dComboWrapper, KPlus, 0.1));
	//dAnalysisActions.push_back(new DCutAction_EachPIDFOM(dComboWrapper, 0.1));

	//MASSES
	//dAnalysisActions.push_back(new DHistogramAction_InvariantMass(dComboWrapper, false, Lambda, 1000, 1.0, 1.2, "Lambda"));
	//dAnalysisActions.push_back(new DHistogramAction_MissingMassSquared(dComboWrapper, false, 1000, -0.1, 0.1));

	//KINFIT RESULTS
	//dAnalysisActions.push_back(new DHistogramAction_KinFitResults(dComboWrapper));

	//CUT MISSING MASS
	//dAnalysisActions.push_back(new DCutAction_MissingMassSquared(dComboWrapper, false, -0.03, 0.02));

	//CUT ON SHOWER QUALITY
	//dAnalysisActions.push_back(new DCutAction_ShowerQuality(dComboWrapper, SYS_FCAL, 0.5));

	//BEAM ENERGY
	//dAnalysisActions.push_back(new DHistogramAction_BeamEnergy(dComboWrapper, false));
	//dAnalysisActions.push_back(new DCutAction_BeamEnergy(dComboWrapper, false, 8.2, 8.8));  // Coherent peak for runs in the range 30000-59999

	//KINEMATICS
	//dAnalysisActions.push_back(new DHistogramAction_ParticleComboKinematics(dComboWrapper, false));

	// ANALYZE CUT ACTIONS
	// // Change MyPhi to match reaction
	dAnalyzeCutActions = new DHistogramAction_AnalyzeCutActions( dAnalysisActions, dComboWrapper, false, 0, MyPhi, 1000, 0.9, 2.4, "CutActionEffect" );

	//INITIALIZE ACTIONS
	//If you create any actions that you want to run manually (i.e. don't add to dAnalysisActions), be sure to initialize them here as well
	Initialize_Actions();
	dAnalyzeCutActions->Initialize(); // manual action, must call Initialize()

	/******************************** EXAMPLE USER INITIALIZATION: STAND-ALONE HISTOGRAMS *******************************/

	//EXAMPLE MANUAL HISTOGRAMS:
	dHist_MissingMassSquared = new TH1I("MissingMassSquared", ";Missing Mass Squared (GeV/c^{2})^{2}", 600, -0.06, 0.06);
	dHist_BeamEnergy = new TH1I("BeamEnergy", ";Beam Energy (GeV)", 600, 0.0, 12.0);

	/************************** EXAMPLE USER INITIALIZATION: CUSTOM OUTPUT BRANCHES - MAIN TREE *************************/

	//EXAMPLE MAIN TREE CUSTOM BRANCHES (OUTPUT ROOT FILE NAME MUST FIRST BE GIVEN!!!! (ABOVE: TOP)):
	//The type for the branch must be included in the brackets
	//1st function argument is the name of the branch
	//2nd function argument is the name of the branch that contains the size of the array (for fundamentals only)
	/*
	dTreeInterface->Create_Branch_Fundamental<Int_t>("my_int"); //fundamental = char, int, float, double, etc.
	dTreeInterface->Create_Branch_FundamentalArray<Int_t>("my_int_array", "my_int");
	dTreeInterface->Create_Branch_FundamentalArray<Float_t>("my_combo_array", "NumCombos");
	dTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("my_p4");
	dTreeInterface->Create_Branch_ClonesArray<TLorentzVector>("my_p4_array");
	*/

	/************************** EXAMPLE USER INITIALIZATION: CUSTOM OUTPUT BRANCHES - FLAT TREE *************************/

	// RECOMMENDED: CREATE ACCIDENTAL WEIGHT BRANCH
	// dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("accidweight");

	//EXAMPLE FLAT TREE CUSTOM BRANCHES (OUTPUT ROOT FILE NAME MUST FIRST BE GIVEN!!!! (ABOVE: TOP)):
	//The type for the branch must be included in the brackets
	//1st function argument is the name of the branch
	//2nd function argument is the name of the branch that contains the size of the array (for fundamentals only)
	/*
	dFlatTreeInterface->Create_Branch_Fundamental<Int_t>("flat_my_int"); //fundamental = char, int, float, double, etc.
	dFlatTreeInterface->Create_Branch_FundamentalArray<Int_t>("flat_my_int_array", "flat_my_int");
	dFlatTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("flat_my_p4");
	dFlatTreeInterface->Create_Branch_ClonesArray<TLorentzVector>("flat_my_p4_array");
	*/


	/************************************* ADVANCED EXAMPLE: CHOOSE BRANCHES TO READ ************************************/

	//TO SAVE PROCESSING TIME
		//If you know you don't need all of the branches/data, but just a subset of it, you can speed things up
		//By default, for each event, the data is retrieved for all branches
		//If you know you only need data for some branches, you can skip grabbing data from the branches you don't need
		//Do this by doing something similar to the commented code below

	//dTreeInterface->Clear_GetEntryBranches(); //now get none
	//dTreeInterface->Register_GetEntryBranch("Proton__P4"); //manually set the branches you want

	/************************************** DETERMINE IF ANALYZING SIMULATED DATA *************************************/

	dIsMC = (dTreeInterface->Get_Branch("MCWeight") != NULL);

}

Bool_t DSelector_ggkpkm__B4::Process(Long64_t locEntry)
{
	// The Process() function is called for each entry in the tree. The entry argument
	// specifies which entry in the currently loaded tree is to be processed.
	//
	// This function should contain the "body" of the analysis. It can contain
	// simple or elaborate selection criteria, run algorithms on the data
	// of the event and typically fill histograms.
	//
	// The processing can be stopped by calling Abort().
	// Use fStatus to set the return value of TTree::Process().
	// The return value is currently not used.

	//CALL THIS FIRST
	DSelector::Process(locEntry); //Gets the data from the tree for the entry
	//cout << "RUN " << Get_RunNumber() << ", EVENT " << Get_EventNumber() << endl;
	TLorentzVector locProductionX4 = Get_X4_Production();

	/******************************************** GET POLARIZATION ORIENTATION ******************************************/

	//Only if the run number changes
	//RCDB environment must be setup in order for this to work! (Will return false otherwise)
	UInt_t locRunNumber = Get_RunNumber();
	if(locRunNumber != dPreviousRunNumber)
	{
		dIsPolarizedFlag = dAnalysisUtilities.Get_IsPolarizedBeam(locRunNumber, dIsPARAFlag);
		dPreviousRunNumber = locRunNumber;
	}

	/********************************************* SETUP UNIQUENESS TRACKING ********************************************/

	//ANALYSIS ACTIONS: Reset uniqueness tracking for each action
	//For any actions that you are executing manually, be sure to call Reset_NewEvent() on them here
	Reset_Actions_NewEvent();
	dAnalyzeCutActions->Reset_NewEvent(); // manual action, must call Reset_NewEvent()

	//PREVENT-DOUBLE COUNTING WHEN HISTOGRAMMING
		//Sometimes, some content is the exact same between one combo and the next
			//e.g. maybe two combos have different beam particles, but the same data for the final-state
		//When histogramming, you don't want to double-count when this happens: artificially inflates your signal (or background)
		//So, for each quantity you histogram, keep track of what particles you used (for a given combo)
		//Then for each combo, just compare to what you used before, and make sure it's unique

	//EXAMPLE 1: Particle-specific info:
	set<Int_t> locUsedSoFar_BeamEnergy; //Int_t: Unique ID for beam particles. set: easy to use, fast to search

	//EXAMPLE 2: Combo-specific info:
		//In general: Could have multiple particles with the same PID: Use a set of Int_t's
		//In general: Multiple PIDs, so multiple sets: Contain within a map
		//Multiple combos: Contain maps within a set (easier, faster to search)
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_MissingMass;

	//INSERT USER ANALYSIS UNIQUENESS TRACKING HERE

	/**************************************** EXAMPLE: FILL CUSTOM OUTPUT BRANCHES **************************************/

	/*
	Int_t locMyInt = 7;
	dTreeInterface->Fill_Fundamental<Int_t>("my_int", locMyInt);

	TLorentzVector locMyP4(4.0, 3.0, 2.0, 1.0);
	dTreeInterface->Fill_TObject<TLorentzVector>("my_p4", locMyP4);

	for(int loc_i = 0; loc_i < locMyInt; ++loc_i)
		dTreeInterface->Fill_Fundamental<Int_t>("my_int_array", 3*loc_i, loc_i); //2nd argument = value, 3rd = array index
	*/

	double max_CL_combo = 0; //to select BestCombo

	/************************************************* LOOP OVER COMBOS *************************************************/

	//Loop over combos
	for(UInt_t loc_i = 0; loc_i < Get_NumCombos(); ++loc_i)
	{
		//Set branch array indices for combo and all combo particles
		dComboWrapper->Set_ComboIndex(loc_i);

		// Is used to indicate when combos have been cut
		if(dComboWrapper->Get_IsComboCut()) // Is false when tree originally created
			continue; // Combo has been cut previously


		//Compare Confidence Level
		double CL_this_combo = dComboWrapper->Get_ConfidenceLevel_KinFit( "" );
		if(max_CL_combo < CL_this_combo){
			max_CL_combo = CL_this_combo;
		}

		/********************************************** GET PARTICLE INDICES *********************************************/

		//Used for tracking uniqueness when filling histograms, and for determining unused particles

		//Step 0
		Int_t locBeamID = dComboBeamWrapper->Get_BeamID();
		Int_t locPhoton1NeutralID = dPhoton1Wrapper->Get_NeutralID();
		Int_t locPhoton2NeutralID = dPhoton2Wrapper->Get_NeutralID();
		Int_t locKPlusTrackID = dKPlusWrapper->Get_TrackID();
		Int_t locKMinusTrackID = dKMinusWrapper->Get_TrackID();
		Int_t locProtonTrackID = dProtonWrapper->Get_TrackID();

		/*********************************************** GET FOUR-MOMENTUM **********************************************/

		// Get P4's: //is kinfit if kinfit performed, else is measured
		//dTargetP4 is target p4
		//Step 0
		TLorentzVector locBeamP4 = dComboBeamWrapper->Get_P4();
		TLorentzVector locPhoton1P4 = dPhoton1Wrapper->Get_P4();
		TLorentzVector locPhoton2P4 = dPhoton2Wrapper->Get_P4();
		TLorentzVector locKPlusP4 = dKPlusWrapper->Get_P4();
		TLorentzVector locKMinusP4 = dKMinusWrapper->Get_P4();
		TLorentzVector locProtonP4 = dProtonWrapper->Get_P4();

		// Get Measured P4's:
		//Step 0
		TLorentzVector locBeamP4_Measured = dComboBeamWrapper->Get_P4_Measured();
		TLorentzVector locPhoton1P4_Measured = dPhoton1Wrapper->Get_P4_Measured();
		TLorentzVector locPhoton2P4_Measured = dPhoton2Wrapper->Get_P4_Measured();
		TLorentzVector locKPlusP4_Measured = dKPlusWrapper->Get_P4_Measured();
		TLorentzVector locKMinusP4_Measured = dKMinusWrapper->Get_P4_Measured();
		TLorentzVector locProtonP4_Measured = dProtonWrapper->Get_P4_Measured();

		/********************************************* GET COMBO RF TIMING INFO *****************************************/

		TLorentzVector locBeamX4_Measured = dComboBeamWrapper->Get_X4_Measured();
		//Double_t locBunchPeriod = dAnalysisUtilities.Get_BeamBunchPeriod(Get_RunNumber());
		// Double_t locDeltaT_RF = dAnalysisUtilities.Get_DeltaT_RF(Get_RunNumber(), locBeamX4_Measured, dComboWrapper);
		 Int_t locRelBeamBucket = dAnalysisUtilities.Get_RelativeBeamBucket(Get_RunNumber(), locBeamX4_Measured, dComboWrapper); // 0 for in-time events, non-zero integer for out-of-time photons
		// Int_t locNumOutOfTimeBunchesInTree = XXX; //YOU need to specify this number
			//Number of out-of-time beam bunches in tree (on a single side, so that total number out-of-time bunches accepted is 2 times this number for left + right bunches) 

		// Bool_t locSkipNearestOutOfTimeBunch = true; // True: skip events from nearest out-of-time bunch on either side (recommended).
		// Int_t locNumOutOfTimeBunchesToUse = locSkipNearestOutOfTimeBunch ? locNumOutOfTimeBunchesInTree-1:locNumOutOfTimeBunchesInTree; 
		// Double_t locAccidentalScalingFactor = dAnalysisUtilities.Get_AccidentalScalingFactor(Get_RunNumber(), locBeamP4.E(), dIsMC); // Ideal value would be 1, but deviations require added factor, which is different for data and MC.
		// Double_t locAccidentalScalingFactorError = dAnalysisUtilities.Get_AccidentalScalingFactorError(Get_RunNumber(), locBeamP4.E()); // Ideal value would be 1, but deviations observed, need added factor.
		// Double_t locHistAccidWeightFactor = locRelBeamBucket==0 ? 1 : -locAccidentalScalingFactor/(2*locNumOutOfTimeBunchesToUse) ; // Weight by 1 for in-time events, ScalingFactor*(1/NBunches) for out-of-time
		// if(locSkipNearestOutOfTimeBunch && abs(locRelBeamBucket)==1) { // Skip nearest out-of-time bunch: tails of in-time distribution also leak in
		// 	dComboWrapper->Set_IsComboCut(true); 
		// 	continue; 
		// } 


		 /********************************************* GET ChiSq INFO *****************************************/

		double locChi2_KinFit = dComboWrapper->Get_ChiSq_KinFit();
		double locNDF_KinFit = dComboWrapper->Get_NDF_KinFit();
		double locChi2_NDF_KinFit = locChi2_KinFit/locNDF_KinFit;

		/********************************************* COMBINE FOUR-MOMENTUM ********************************************/

		// DO YOUR STUFF HERE

		// Combine 4-vectors
		TLorentzVector locMissingP4_Measured = locBeamP4_Measured + dTargetP4;
		locMissingP4_Measured -= locPhoton1P4_Measured + locPhoton2P4_Measured + locKPlusP4_Measured + locKMinusP4_Measured + locProtonP4_Measured;



		TLorentzVector locPhoton1P4_KinFit = locPhoton1P4;
		TLorentzVector locPhoton2P4_KinFit = locPhoton2P4;

		TLorentzVector locKPlusP4_KinFit = locKPlusP4;
		TLorentzVector locKMinusP4_KinFit = locKMinusP4;
	
		TLorentzVector locProtonP4_KinFit = locProtonP4;	
		//Pi0
		TLorentzVector locPi0P4_KinFit = locPhoton1P4 + locPhoton2P4;
		TLorentzVector locPi0P4_Measured = locPhoton1P4_Measured + locPhoton2P4_Measured;
		//
		//
		//Phi
		TLorentzVector locPhiP4_KinFit = locKPlusP4 + locKMinusP4;
		TLorentzVector locPhiP4_Measured = locKPlusP4_Measured + locKMinusP4_Measured;



		//FinalState
		TLorentzVector locPi0_PhiP4_KinFit = locPhoton1P4 + locPhoton2P4 + locKPlusP4 + locKMinusP4;

		
	
		Double_t locMissingMassSquared_Measured = locMissingP4_Measured.M2();

		Double_t InvariantMassPhi_KinFit = locPhiP4_KinFit.M();
		Double_t InvariantMassPi0_KinFit = locPi0P4_KinFit.M();
		Double_t InvariantMassg01g02kpkm_KinFit = locPi0_PhiP4_KinFit.M();

		Double_t vertexZ = locProductionX4.Z();
		Double_t Momentum_Measured_Proton = locProtonP4_Measured.P();


		//Boost to CM frame
		TLorentzVector locInitialStateP4 = locBeamP4 + dTargetP4;
        TVector3 locBoostVector_CM = -1.0*(locInitialStateP4.BoostVector());

        TLorentzVector locBeamP4_CM(locBeamP4); locBeamP4_CM.Boost(locBoostVector_CM);
        TLorentzVector locProtonP4_CM(locProtonP4_KinFit); locProtonP4_CM.Boost(locBoostVector_CM);
        TLorentzVector locKPlusP4_CM(locKPlusP4_KinFit); locKPlusP4_CM.Boost(locBoostVector_CM);
        TLorentzVector locKMinusP4_CM(locKMinusP4_KinFit); locKMinusP4_CM.Boost(locBoostVector_CM);
        TLorentzVector locPhoton1P4_CM(locPhoton1P4_KinFit); locPhoton1P4_CM.Boost(locBoostVector_CM);
        TLorentzVector locPhoton2P4_CM(locPhoton2P4_KinFit); locPhoton2P4_CM.Boost(locBoostVector_CM);

        TLorentzVector locPhiP4_CM(locPhiP4_KinFit); locPhiP4_CM.Boost(locBoostVector_CM);
        TLorentzVector locPi0P4_CM(locPi0P4_KinFit); locPi0P4_CM.Boost(locBoostVector_CM);

		//project momentum to the Beam direction
        TVector3 locBeam_unit = locBeamP4_CM.Vect().Unit();
        double Pi0_l = locPi0P4_CM.Vect().Dot(locBeam_unit);
        double Proton_l = locProtonP4_CM.Vect().Dot(locBeam_unit);
        double Phi_l = locPhiP4_CM.Vect().Dot(locBeam_unit);

        //points (x,y)          
        double x_3D = TMath::Sqrt(3.0/2.0) * Proton_l;
        double y_3D = TMath::Sqrt(1.0/2.0) * (Phi_l - Pi0_l);
		/**************************************** Cut *****************************************/

		//in-time
		if (locRelBeamBucket !=0) {
				dComboWrapper->Set_IsComboCut(true);
				continue;
		}

		//vertexZ cut
		if (vertexZ<51.0 || vertexZ>78.0){
			dComboWrapper->Set_IsComboCut(true);
			continue;
		}
		//proton momentum cut
		if (Momentum_Measured_Proton <= 0.35){
			dComboWrapper->Set_IsComboCut(true);
			continue;
		}

		if(locBeamP4.E() <= 8.2 || locBeamP4.E() >= 8.8) {
			dComboWrapper->Set_IsComboCut(true);
			continue;
		}

		if((locMissingMassSquared_Measured < -0.01) || (locMissingMassSquared_Measured > 0.01))
		{
			dComboWrapper->Set_IsComboCut(true);
			continue;
		}
		if(locChi2_NDF_KinFit > 3.5){
			dComboWrapper->Set_IsComboCut(true);
			continue;
		}
		if(InvariantMassPi0_KinFit < 0.11){
			dComboWrapper->Set_IsComboCut(true);
			continue;
		}
		if(InvariantMassPi0_KinFit > 0.16){
			dComboWrapper->Set_IsComboCut(true);
			continue;
		}
		if(InvariantMassPhi_KinFit > 1.04){
			dComboWrapper->Set_IsComboCut(true);
			continue;
		}
		//vanHove cut
		//if(x_3D > 0.0){
        //    continue;
        //}
        //if((y_3D > -0.7*x_3D) || (y_3D < TMath::Sqrt(3.0)/3.0*x_3D)){
        //    continue;
        //}


		/******************************************** EXECUTE ANALYSIS ACTIONS *******************************************/

		// Loop through the analysis actions, executing them in order for the active particle combo
		dAnalyzeCutActions->Perform_Action(); // Must be executed before Execute_Actions()
		if(!Execute_Actions()) //if the active combo fails a cut, IsComboCutFlag automatically set
			continue;

		//if you manually execute any actions, and it fails a cut, be sure to call:
			//dComboWrapper->Set_IsComboCut(true);

		/**************************************** EXAMPLE: FILL CUSTOM OUTPUT BRANCHES **************************************/

		/*
		TLorentzVector locMyComboP4(8.0, 7.0, 6.0, 5.0);
		//for arrays below: 2nd argument is value, 3rd is array index
		//NOTE: By filling here, AFTER the cuts above, some indices won't be updated (and will be whatever they were from the last event)
			//So, when you draw the branch, be sure to cut on "IsComboCut" to avoid these.
		dTreeInterface->Fill_Fundamental<Float_t>("my_combo_array", -2*loc_i, loc_i);
		dTreeInterface->Fill_TObject<TLorentzVector>("my_p4_array", locMyComboP4, loc_i);
		*/

		/**************************************** EXAMPLE: HISTOGRAM BEAM ENERGY *****************************************/

		//Histogram beam energy (if haven't already)
		if(locUsedSoFar_BeamEnergy.find(locBeamID) == locUsedSoFar_BeamEnergy.end())
		{
			dHist_BeamEnergy->Fill(locBeamP4.E()); // Fills in-time and out-of-time beam photon combos
			//dHist_BeamEnergy->Fill(locBeamP4.E(),locHistAccidWeightFactor); // Alternate version with accidental subtraction

			locUsedSoFar_BeamEnergy.insert(locBeamID);
		}

		/************************************ EXAMPLE: HISTOGRAM MISSING MASS SQUARED ************************************/

		//Missing Mass Squared
		double locMissingMassSquared = locMissingP4_Measured.M2();

		//Uniqueness tracking: Build the map of particles used for the missing mass
			//For beam: Don't want to group with final-state photons. Instead use "Unknown" PID (not ideal, but it's easy).
		map<Particle_t, set<Int_t> > locUsedThisCombo_MissingMass;
		locUsedThisCombo_MissingMass[Unknown].insert(locBeamID); //beam
		locUsedThisCombo_MissingMass[Gamma].insert(locPhoton1NeutralID);
		locUsedThisCombo_MissingMass[Gamma].insert(locPhoton2NeutralID);
		locUsedThisCombo_MissingMass[KPlus].insert(locKPlusTrackID);
		locUsedThisCombo_MissingMass[KMinus].insert(locKMinusTrackID);
		locUsedThisCombo_MissingMass[Proton].insert(locProtonTrackID);

		//compare to what's been used so far
		if(locUsedSoFar_MissingMass.find(locUsedThisCombo_MissingMass) == locUsedSoFar_MissingMass.end())
		{
			//unique missing mass combo: histogram it, and register this combo of particles
			dHist_MissingMassSquared->Fill(locMissingMassSquared); // Fills in-time and out-of-time beam photon combos
			//dHist_MissingMassSquared->Fill(locMissingMassSquared,locHistAccidWeightFactor); // Alternate version with accidental subtraction

			locUsedSoFar_MissingMass.insert(locUsedThisCombo_MissingMass);
		}

		//E.g. Cut
		//if((locMissingMassSquared < -0.04) || (locMissingMassSquared > 0.04))
		//{
		//	dComboWrapper->Set_IsComboCut(true);
		//	continue;
		//}

		/****************************************** FILL FLAT TREE (IF DESIRED) ******************************************/

		// RECOMMENDED: FILL ACCIDENTAL WEIGHT
		// dFlatTreeInterface->Fill_Fundamental<Double_t>("accidweight",locHistAccidWeightFactor);

		/*
		//FILL ANY CUSTOM BRANCHES FIRST!!
		Int_t locMyInt_Flat = 7;
		dFlatTreeInterface->Fill_Fundamental<Int_t>("flat_my_int", locMyInt_Flat);

		TLorentzVector locMyP4_Flat(4.0, 3.0, 2.0, 1.0);
		dFlatTreeInterface->Fill_TObject<TLorentzVector>("flat_my_p4", locMyP4_Flat);

		for(int loc_j = 0; loc_j < locMyInt_Flat; ++loc_j)
		{
			dFlatTreeInterface->Fill_Fundamental<Int_t>("flat_my_int_array", 3*loc_j, loc_j); //2nd argument = value, 3rd = array index
			TLorentzVector locMyComboP4_Flat(8.0, 7.0, 6.0, 5.0);
			dFlatTreeInterface->Fill_TObject<TLorentzVector>("flat_my_p4_array", locMyComboP4_Flat, loc_j);
		}
		*/

		//FILL FLAT TREE
		//Fill_FlatTree(); //for the active combo
	} // end of combo loop



	//second combo loop
	for(UInt_t loc_i = 0; loc_i < Get_NumCombos(); ++loc_i){
	
		dComboWrapper->Set_ComboIndex(loc_i);

		if(dComboWrapper->Get_IsComboCut()){
			continue; 
		}

		//check combo confidence level
		double CL_this_combo = dComboWrapper->Get_ConfidenceLevel_KinFit( "" );
		
		if(max_CL_combo > CL_this_combo){
			dComboWrapper->Set_IsComboCut(true);
			continue;
		}

		
		/********************************************** GET PARTICLE INDICES *********************************************/

		//Used for tracking uniqueness when filling histograms, and for determining unused particles

		//Step 0
		Int_t locBeamID = dComboBeamWrapper->Get_BeamID();
		Int_t locPhoton1NeutralID = dPhoton1Wrapper->Get_NeutralID();
		Int_t locPhoton2NeutralID = dPhoton2Wrapper->Get_NeutralID();
		Int_t locKPlusTrackID = dKPlusWrapper->Get_TrackID();
		Int_t locKMinusTrackID = dKMinusWrapper->Get_TrackID();
		Int_t locProtonTrackID = dProtonWrapper->Get_TrackID();

		/*********************************************** GET FOUR-MOMENTUM **********************************************/

		// Get P4's: //is kinfit if kinfit performed, else is measured
		//dTargetP4 is target p4
		//Step 0
		TLorentzVector locBeamP4 = dComboBeamWrapper->Get_P4();
		TLorentzVector locPhoton1P4 = dPhoton1Wrapper->Get_P4();
		TLorentzVector locPhoton2P4 = dPhoton2Wrapper->Get_P4();
		TLorentzVector locKPlusP4 = dKPlusWrapper->Get_P4();
		TLorentzVector locKMinusP4 = dKMinusWrapper->Get_P4();
		TLorentzVector locProtonP4 = dProtonWrapper->Get_P4();



		TLorentzVector locPhoton1P4_KinFit = locPhoton1P4;
		TLorentzVector locPhoton2P4_KinFit = locPhoton2P4;
		TLorentzVector locKPlusP4_KinFit = locKPlusP4;
		TLorentzVector locKMinusP4_KinFit = locKMinusP4;

		//Pi0
		TLorentzVector locPi0P4_KinFit = locPhoton1P4 + locPhoton2P4;
		//
		//
		//Phi
		TLorentzVector locPhiP4_KinFit = locKPlusP4 + locKMinusP4;

		//FinalState
		TLorentzVector locPi0_PhiP4_KinFit = locPhoton1P4 + locPhoton2P4 + locKPlusP4 + locKMinusP4;

		Double_t M_FinalState = locPi0_PhiP4_KinFit.M();


	}

	//FILL HISTOGRAMS: Num combos / events surviving actions
	Fill_NumCombosSurvivedHists();

	/******************************************* LOOP OVER THROWN DATA (OPTIONAL) ***************************************/
/*
	//Thrown beam: just use directly
	if(dThrownBeam != NULL)
		double locEnergy = dThrownBeam->Get_P4().E();

	//Loop over throwns
	for(UInt_t loc_i = 0; loc_i < Get_NumThrown(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dThrownWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}
*/
	/****************************************** LOOP OVER OTHER ARRAYS (OPTIONAL) ***************************************/
/*
	//Loop over beam particles (note, only those appearing in combos are present)
	for(UInt_t loc_i = 0; loc_i < Get_NumBeam(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dBeamWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}

	//Loop over charged track hypotheses
	for(UInt_t loc_i = 0; loc_i < Get_NumChargedHypos(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dChargedHypoWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}

	//Loop over neutral particle hypotheses
	for(UInt_t loc_i = 0; loc_i < Get_NumNeutralHypos(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dNeutralHypoWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}
*/

	/************************************ EXAMPLE: FILL CLONE OF TTREE HERE WITH CUTS APPLIED ************************************/

	Bool_t locIsEventCut = true;
	for(UInt_t loc_i = 0; loc_i < Get_NumCombos(); ++loc_i) {
		//Set branch array indices for combo and all combo particles
		dComboWrapper->Set_ComboIndex(loc_i);
		// Is used to indicate when combos have been cut
		if(dComboWrapper->Get_IsComboCut())
			continue;
		locIsEventCut = false; // At least one combo succeeded
		break;
	}
	if(!locIsEventCut && dOutputTreeFileName != "")
		Fill_OutputTree();


	return kTRUE;
}

void DSelector_ggkpkm__B4::Finalize(void)
{
	//Save anything to output here that you do not want to be in the default DSelector output ROOT file.

	//Otherwise, don't do anything else (especially if you are using PROOF).
		//If you are using PROOF, this function is called on each thread,
		//so anything you do will not have the combined information from the various threads.
		//Besides, it is best-practice to do post-processing (e.g. fitting) separately, in case there is a problem.

	//DO YOUR STUFF HERE

	//CALL THIS LAST
	DSelector::Finalize(); //Saves results to the output file
}
