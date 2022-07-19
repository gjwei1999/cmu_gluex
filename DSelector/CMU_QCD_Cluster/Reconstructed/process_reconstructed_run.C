// set proof_Nthreads to 0 to run without using proof
// to run from the terminal: root 'process_this_run.C(31049,0,"/raid2/nsj/phi/kinfit100/tree_kinfit100_31049.root","kpkm__B4_Tree","kinfit100")'


#include <iostream>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TProof.h"
#include "TProofDebug.h"

R__LOAD_LIBRARY(libDSelector)     

void process_reconstructed_run(TString run, int proof_Nthreads=0, TString fileName="/raid4/zbaldwin/forJiawei/tree_ggkpkm__B4_030274.root", TString treeName="ggkpkm__B4_Tree", TString dsel="ggkpkm__B4" ) {

  printf("Requested DSelector %s for tree %s inside file %s with %i PROOF threads\n",dsel.Data(),treeName.Data(),fileName.Data(),proof_Nthreads);

    gEnv->SetValue("ProofLite.Sandbox", "$PWD/.proof/");

    gROOT->ProcessLine( ".x $ROOT_ANALYSIS_HOME/scripts/Load_DSelector.C" );
    
    char outputHistfile[300], outputTreefile[300], dselector[100];
    sprintf(dselector,"DSelector_%s.C++",dsel.Data());

    if (gSystem->AccessPathName(fileName)) {

        printf("Cannot find file %s\n",fileName.Data());

    } else {

        TChain *chain = new TChain(treeName);
        chain->Add(fileName);

        if (!chain) cout << " Cannot find tree " << treeName << " in " << fileName << endl;
        cout << "Processing " << fileName << " with " << proof_Nthreads << " PROOF worker threads " <<endl; 

        //sprintf(outputHistfile,"hist_%s_%.5i.root", dsel.Data(), run);    
        sprintf(outputHistfile,"hist_%s.root", dsel.Data()); 
	//sprintf(outputTreefile,"tree_%s_%.5i.root", dsel.Data(), run);    
	sprintf(outputTreefile,"tree_%s.root", dsel.Data()); 

        if (proof_Nthreads) {

	     DPROOFLiteManager *dproof = new DPROOFLiteManager();
             dproof->Process_Chain(chain, dselector, proof_Nthreads, outputHistfile, outputTreefile, "");
		//  DPROOFLiteManager::Process_Chain(chain, dselector, proof_Nthreads, outputHistFileName, outputTreeFileName, "");
			
        } else {


	    if (dsel.Contains("amptools")) {
                chain->Process(dselector,"DEFAULTFLATOFF");
            } else {
		chain->Process(dselector);
            }

            TString thistreefile = Form("tree_%s.root",dsel.Data());	
  	    TString thishistfile = Form("hist_%s.root",dsel.Data());
	    //TString thishistfile = Form("%s.root",dsel.Data());

            gSystem->Rename(thistreefile, outputTreefile);
            gSystem->Rename(thishistfile ,outputHistfile);

        }

        cout << "Run " << run << " processed: " << outputHistfile << " " << outputTreefile << endl;

        //check all entries were processed
        TFile *f = new TFile(fileName);
        TTree *t = (TTree*)f->Get(treeName);

        Long64_t entries_orig;

        if (dsel.Contains("intime")) {
            entries_orig = t->GetEntries("RelBeamBucket==0&&!IsComboCut");
	} else if (dsel.Contains("outoftime")) {
            entries_orig = t->GetEntries("RelBeamBucket!=0&&!IsComboCut");
	} else if (dsel.Contains("beam_info")) {
            entries_orig = t->GetEntries("!IsComboCut&&fabs(RelBeamBucket)!=1");
        } else { 
            entries_orig = t->GetEntries();
        }
        f->Close();

        TFile *ff = new TFile(outputHistfile);
        TH1D *hevts = (TH1D*)gDirectory->Get("NumEventsSurvivedAction");
        Long64_t entries_proc =  hevts->GetBinContent(1);
        ff->Close();

        cout << "Original tree events: " << entries_orig << endl;
        cout << "Events processed: " << entries_proc << endl;

        if (entries_proc != entries_orig) {
 	   cout << "Problem! These do not match!" << endl;
           FILE *errfile = fopen("error.txt","w");
           fprintf(errfile,"%lli original events %lli processed\n",entries_orig,entries_proc);
           fclose(errfile);
        }

	}  //end if file found

}
