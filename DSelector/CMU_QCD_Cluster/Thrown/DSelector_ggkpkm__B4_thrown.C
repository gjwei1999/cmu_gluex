#include "DSelector_ggkpkm__B4_thrown.h"

void DSelector_ggkpkm__B4_thrown::Init(TTree *locTree)
{
	// USERS: IN THIS FUNCTION, ONLY MODIFY SECTIONS WITH A "USER" OR "EXAMPLE" LABEL. LEAVE THE REST ALONE.

	// The Init() function is called when the selector needs to initialize a new tree or chain.
	// Typically here the branch addresses and branch pointers of the tree will be set.
	// Init() will be called many times when running on PROOF (once per file to be processed).

	//USERS: SET OUTPUT FILE NAME //can be overriden by user in PROOF
	dOutputFileName = "hist_ggkpkm__B4_thrown.root"; //"" for none
	dFlatTreeFileName = "tree_ggkpkm__B4_thrown.root"; //output flat tree (one combo per tree entry), "" for none
	//USERS: SET OUTPUT TREE FILES/NAMES //e.g. binning into separate files for AmpTools
	//dOutputTreeFileNameMap["Bin1"] = "mcgen_bin1.root"; //key is user-defined, value is output file name
	//dOutputTreeFileNameMap["Bin2"] = "mcgen_bin2.root"; //key is user-defined, value is output file name
	//dOutputTreeFileNameMap["Bin3"] = "mcgen_bin3.root"; //key is user-defined, value is output file name

	//Because this function gets called for each TTree in the TChain, we must be careful:
		//We need to re-initialize the tree interface & branch wrappers, but don't want to recreate histograms
	bool locInitializedPriorFlag = dInitializedFlag; //save whether have been initialized previously
	DSelector::Init(locTree); //This must be called to initialize wrappers for each new TTree
	//gDirectory now points to the output file with name dOutputFileName (if any)
	if(locInitializedPriorFlag)
		return; //have already created histograms, etc. below: exit

	dPreviousRunNumber = 0;


	TDirectory *savdir = gDirectory;
    TDirectory *adir = savdir->mkdir("InvariantMass"); //Subdirectory name
	
	TString locBeamEBinLabel[1] = {"Egamma"}; //Naming label

    Int_t a;
    TDirectory *directory[1];
   	for(a=0; a<1; a++){
        TString dirname = locBeamEBinLabel[a];
        directory[a] = adir->mkdir(dirname.Data());
	}




	/******************************** EXAMPLE USER INITIALIZATION: STAND-ALONE HISTOGRAMS *******************************/
	for(int locBeamEBin=0; locBeamEBin<1; locBeamEBin++){
		directory[locBeamEBin]->cd();

		dHist_InvariantMass_ggKpKm_thrown[locBeamEBin] = new TH1F("InvariantMass_ggKpKm_thrown"+locBeamEBinLabel[locBeamEBin],"M(ggK^{+}K^{-}) (GeV/c^{2})", 350, 0.0, 3.5);
		dHist_InvariantMass_g1g2_thrown[locBeamEBin] = new TH1F("InvariantMass_g1g2_thrown"+locBeamEBinLabel[locBeamEBin],"M(#gamma_{1}#gamma_{2}) (GeV/c^{2})", 350, 0.0, 0.3);
                dHist_InvariantMass_KpKm_thrown[locBeamEBin] = new TH1F("InvariantMass_KpKm_thrown"+locBeamEBinLabel[locBeamEBin],"M(K^{+}K^{-}) (GeV/c^{2})", 350, 0.9, 1.5);

		dHist_Dalitz_g1g2Kp_KpKm_thrown[locBeamEBin] =  new TH2F("Dalitz_g1g2Kp_KpKm_thrown"+locBeamEBinLabel[locBeamEBin], "M^{2}(#gamma_{1}#gamma_{2}K^{+}) vs M^{2}(K^{+}K^{-})", 200, 0.9, 1.5, 200, 0.3, 1.5);
        dHist_Dalitz_g1g2Kp_g1g2Km_thrown[locBeamEBin] =  new TH2F("Dalitz_g1g2Kp_g1g2Km_thrown"+locBeamEBinLabel[locBeamEBin], "M^{2}(#gamma_{1}#gamma_{2}K^{+}) vs M^{2}(#gamma_{1}#gamma_{2}K^{-})", 200, 0.3, 1.5, 200, 0.3, 1.5);
	}


	//Flat Tree
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Beam_px");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Beam_py");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Beam_pz");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Beam_E");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Target_px");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Target_py");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Target_pz");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Target_E");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("KPlus_px");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("KPlus_py");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("KPlus_pz");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("KPlus_E");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("KMinus_px");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("KMinus_py");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("KMinus_pz");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("KMinus_E");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Proton_px");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Proton_py");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Proton_pz");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Proton_E");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Phi_px");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Phi_py");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Phi_pz");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Phi_E");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Pi0_px");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Pi0_py");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Pi0_pz");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Pi0_E");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Gamma1_px");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Gamma1_py");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Gamma1_pz");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Gamma1_E");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Gamma2_px");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Gamma2_py");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Gamma2_pz");
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Gamma2_E");

	//This branch fundamental will be used to check final state invariant mass (in order to make sure no mistakes
	//were made in filling out information
	dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("InvariantMass_ggkpkm");

	/************************************* ADVANCED EXAMPLE: CHOOSE BRANCHES TO READ ************************************/

	//TO SAVE PROCESSING TIME
		//If you know you don't need all of the branches/data, but just a subset of it, you can speed things up
		//By default, for each event, the data is retrieved for all branches
		//If you know you only need data for some branches, you can skip grabbing data from the branches you don't need
		//Do this by doing something similar to the commented code below

	//dTreeInterface->Clear_GetEntryBranches(); //now get none
	//dTreeInterface->Register_GetEntryBranch("Proton__P4"); //manually set the branches you want
}

Bool_t DSelector_ggkpkm__B4_thrown::Process(Long64_t locEntry)
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

	//INSERT USER ANALYSIS UNIQUENESS TRACKING HERE

	/******************************************* LOOP OVER THROWN DATA ***************************************/

	//Thrown beam: just use directly
	double locBeamEnergyUsedForBinning = 0.0;
	TLorentzVector dBeamP4;

	if(dThrownBeam != NULL){
		locBeamEnergyUsedForBinning = dThrownBeam->Get_P4().E();
		dBeamP4 = dThrownBeam->Get_P4();
		
		//cout << "Beam Px "  << dBeamP4.Px() << endl;
		//cout << "Beam Py "  << dBeamP4.Py() << endl;
	       	//cout << "Beam Pz "  << dBeamP4.Pz() << endl; 
		//cout << "Beam E  "  << dBeamP4.E() << endl;
	}


	TLorentzVector locBeamP4 = dBeamP4;
	TLorentzVector locTargetP4;
	locTargetP4.SetPxPyPzE(0.0, 0.0, 0.0, 0.938);//proton mass

	UInt_t loc_i = 0;
	dThrownWrapper->Set_ArrayIndex(loc_i);
	TLorentzVector locProtonP4 = dThrownWrapper->Get_P4();

	loc_i = 1;
	dThrownWrapper->Set_ArrayIndex(loc_i);
	TLorentzVector locKPlusP4 = dThrownWrapper->Get_P4();

	loc_i = 2;
	dThrownWrapper->Set_ArrayIndex(loc_i);
	TLorentzVector locKMinusP4 = dThrownWrapper->Get_P4();

	loc_i = 3;
	dThrownWrapper->Set_ArrayIndex(loc_i);
	TLorentzVector locPhoton1P4 = dThrownWrapper->Get_P4();

	loc_i = 4;
	dThrownWrapper->Set_ArrayIndex(loc_i);
	TLorentzVector locPhoton2P4 = dThrownWrapper->Get_P4();

	//combination
	TLorentzVector locPi0P4 = locPhoton1P4 + locPhoton2P4;
	TLorentzVector locPhiP4 = locKPlusP4 + locKMinusP4;

	TLorentzVector locg1g2kp = locPhoton1P4 + locPhoton2P4 + locKPlusP4;
	TLorentzVector locg1g2km = locPhoton1P4 + locPhoton2P4 + locKMinusP4;

	//FinalState
	TLorentzVector locggkpkmP4 =  locPhoton1P4 + locPhoton2P4 + locKPlusP4 + locKMinusP4;


	Double_t InvariantMass_ggkpkm_thrown = locggkpkmP4.M();


	Double_t InvariantMass_Phi_thrown = locPhiP4.M();
	Double_t InvariantMass_Pi0_thrown = locPi0P4.M();

	Double_t InvariantMassg01g02Kp_thrown = locg1g2kp.M();
	Double_t InvariantMassg01g02Km_thrown = locg1g2kp.M();

	/******************************************* Cuts here ***************************************/
	bool IsCut = false;
	

	Int_t locBeamEBin = 0;
	if(IsCut == false){
	
		dHist_InvariantMass_ggKpKm_thrown[locBeamEBin]->Fill(InvariantMass_ggkpkm_thrown);
		dHist_InvariantMass_g1g2_thrown[locBeamEBin]->Fill(locPi0P4.M());
       	dHist_InvariantMass_KpKm_thrown[locBeamEBin]->Fill(locPhiP4.M());

		dHist_Dalitz_g1g2Kp_KpKm_thrown[locBeamEBin]->Fill(locPhiP4.M2(), locg1g2kp.M2());
		dHist_Dalitz_g1g2Kp_g1g2Km_thrown[locBeamEBin]->Fill(locg1g2km.M2(), locg1g2kp.M2());

		//Beam Information
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Beam_E", locBeamP4.E());
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Beam_px", locBeamP4.Px());
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Beam_py", locBeamP4.Py());
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Beam_pz", locBeamP4.Pz());

		//Target Information
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Target_E", locTargetP4.E());
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Target_px", locTargetP4.Px());
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Target_py", locTargetP4.Py());
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Target_pz", locTargetP4.Pz());

		//KPlus Information
		dFlatTreeInterface->Fill_Fundamental<Double_t>("KPlus_E", locKPlusP4.E());
		dFlatTreeInterface->Fill_Fundamental<Double_t>("KPlus_px", locKPlusP4.Px());
		dFlatTreeInterface->Fill_Fundamental<Double_t>("KPlus_py", locKPlusP4.Py());
		dFlatTreeInterface->Fill_Fundamental<Double_t>("KPlus_pz", locKPlusP4.Pz());

		//KMinus Information
		dFlatTreeInterface->Fill_Fundamental<Double_t>("KMinus_E", locKMinusP4.E());
		dFlatTreeInterface->Fill_Fundamental<Double_t>("KMinus_px", locKMinusP4.Px());
		dFlatTreeInterface->Fill_Fundamental<Double_t>("KMinus_py", locKMinusP4.Py());
		dFlatTreeInterface->Fill_Fundamental<Double_t>("KMinus_pz", locKMinusP4.Pz());

		//Proton Information
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Proton_E", locProtonP4.E());
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Proton_px", locProtonP4.Px());
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Proton_py", locProtonP4.Py());
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Proton_pz", locProtonP4.Pz());

		//Phi Information
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Phi_E", locPhiP4.E());
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Phi_px", locPhiP4.Px());
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Phi_py", locPhiP4.Py());
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Phi_pz", locPhiP4.Pz());

		//Pi0 Information
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Pi0_E", locPi0P4.E());
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Pi0_px", locPi0P4.Px());
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Pi0_py", locPi0P4.Py());
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Pi0_pz", locPi0P4.Pz());

		//Gamma1 Information
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Gamma1_E", locPhoton1P4.E());
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Gamma1_px", locPhoton1P4.Px());
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Gamma1_py", locPhoton1P4.Py());
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Gamma1_pz", locPhoton1P4.Pz());

		//Gamma2 Information
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Gamma2_E", locPhoton2P4.E());
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Gamma2_px", locPhoton2P4.Px());
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Gamma2_py", locPhoton2P4.Py());
		dFlatTreeInterface->Fill_Fundamental<Double_t>("Gamma2_pz", locPhoton2P4.Pz());

	
		//Main Invariant Mass Information
		dFlatTreeInterface->Fill_Fundamental<Double_t>("InvariantMass_ggkpkm", InvariantMass_ggkpkm_thrown);

		//FILL FLAT TREE
		Fill_FlatTree(); //for the active combo

	}

/*	//Loop over throwns
	for(UInt_t loc_i = 0; loc_i < Get_NumThrown(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dThrownWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
		Particle_t locPID = dThrownWrapper->Get_PID();
		TLorentzVector locThrownP4 = dThrownWrapper->Get_P4();
		
		//cout << "Thrown " << loc_i << ": " << locPID << ", " << locThrownP4.Px() << ", " << locThrownP4.Py() << ", " << locThrownP4.Pz() << ", " << locThrownP4.E() << endl;
	}	
	//cout << "" << endl;
*/	

	//OR Manually:
	//BEWARE: Do not expect the particles to be at the same array indices from one event to the next!!!!
	//Why? Because while your channel may be the same, the pions/kaons/etc. will decay differently each event.

	//BRANCHES: https://halldweb.jlab.org/wiki/index.php/Analysis_TTreeFormat#TTree_Format:_Simulated_Data
/*	TClonesArray** locP4Array = dTreeInterface->Get_PointerToPointerTo_TClonesArray("Thrown__P4");
	TBranch* locPIDBranch = dTreeInterface->Get_Branch("Thrown__PID");

	Particle_t locThrown1PID = PDGtoPType(((Int_t*)locPIDBranch->GetAddress())[0]);
	TLorentzVector locThrown1P4 = *((TLorentzVector*)(*locP4Array)->At(0));
	//cout << "Particle 1: " << locThrown1PID << ", " << locThrown1P4.Px() << ", " << locThrown1P4.Py() << ", " << locThrown1P4.Pz() << ", " << locThrown1P4.E() << endl;
	
	Particle_t locThrown2PID = PDGtoPType(((Int_t*)locPIDBranch->GetAddress())[1]);
	TLorentzVector locThrown2P4 = *((TLorentzVector*)(*locP4Array)->At(1));
	//cout << "Particle 2: " << locThrown2PID << ", " << locThrown2P4.Px() << ", " << locThrown2P4.Py() << ", " << locThrown2P4.Pz() << ", " << locThrown2P4.E() << endl;
	
	Particle_t locThrown3PID = PDGtoPType(((Int_t*)locPIDBranch->GetAddress())[2]);
	TLorentzVector locThrown3P4 = *((TLorentzVector*)(*locP4Array)->At(2));
        //cout << "Particle 3: " << locThrown3PID << ", " << locThrown3P4.Px() << ", " << locThrown3P4.Py() << ", " << locThrown3P4.Pz() << ", " << locThrown3P4.E() << endl;
	
	Particle_t locThrown4PID = PDGtoPType(((Int_t*)locPIDBranch->GetAddress())[3]);
	TLorentzVector locThrown4P4 = *((TLorentzVector*)(*locP4Array)->At(3));
        //cout << "Particle 4: " << locThrown4PID << ", " << locThrown4P4.Px() << ", " << locThrown4P4.Py() << ", " << locThrown4P4.Pz() << ", " << locThrown4P4.E() << endl;
	
	Particle_t locThrown5PID = PDGtoPType(((Int_t*)locPIDBranch->GetAddress())[4]);
        TLorentzVector locThrown5P4 = *((TLorentzVector*)(*locP4Array)->At(4));
        //cout << "Particle 5: " << locThrown5PID << ", " << locThrown5P4.Px() << ", " << locThrown5P4.Py() << ", " << locThrown5P4.Pz() << ", " << locThrown5P4.E() << endl;

	Particle_t locThrown6PID = PDGtoPType(((Int_t*)locPIDBranch->GetAddress())[5]);
        TLorentzVector locThrown6P4 = *((TLorentzVector*)(*locP4Array)->At(5));
        cout << "Particle 6: " << locThrown6PID << ", " << locThrown6P4.Px() << ", " << locThrown6P4.Py() << ", " << locThrown6P4.Pz() << ", " << locThrown6P4.E() << endl;

	cout << "Particle 4 5: " << ", " << (locThrown4P4+locThrown5P4).Px() << ", " << (locThrown4P4+locThrown5P4).Py() << ", " << (locThrown4P4+locThrown5P4).Pz() << ", " << (locThrown4P4+locThrown5P4).E() << endl;

	cout << "" << endl;
*/	
	//if(locThrown6P4==(locThrown5P4+locThrown4P4)){

		
	//}


	/******************************************* BIN THROWN DATA INTO SEPARATE TREES FOR AMPTOOLS ***************************************/

/*
	//THESE KEYS MUST BE DEFINED IN THE INIT SECTION (along with the output file names)
	if((locBeamEnergyUsedForBinning >= 8.0) && (locBeamEnergyUsedForBinning < 9.0))
		Fill_OutputTree("Bin1"); //your user-defined key
	else if((locBeamEnergyUsedForBinning >= 9.0) && (locBeamEnergyUsedForBinning < 10.0))
		Fill_OutputTree("Bin2"); //your user-defined key
	else if((locBeamEnergyUsedForBinning >= 10.0) && (locBeamEnergyUsedForBinning < 11.0))
		Fill_OutputTree("Bin3"); //your user-defined key
*/

	return kTRUE;
}

void DSelector_ggkpkm__B4_thrown::Finalize(void)
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
