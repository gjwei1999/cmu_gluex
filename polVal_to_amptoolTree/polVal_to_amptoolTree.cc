#include <iostream>
#include <fstream>
#include <cmath>

#include "TROOT.h"
#include "TTree.h"
#include "TString.h"
#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TF1.h"
#include "TH2F.h"
#include "TH3F.h"

using namespace std;

int main(const int argc, const char * argv[]){

    if(argc < 6){
        std::cout << "./polVal_to_amptoolTree <amptoolTree_file_path> <amptoolTree_file_name> <polAng> <pol_Ebeam_hist_path> <hist_name>" << std::endl;
        return 0;
    }
    TString file_path = argv[1];
    TString file_name = argv[2];
    float polAng_deg = atof(argv[3]);
    TString pol_Ebeam_hist_path = argv[4];
    TString hist_name = argv[5];
    

    // Amptool Tree file
    TFile * AmptoolTree_file = new TFile(file_path+"/"+file_name, "READ"); 
    TTree * amptooltree = (TTree*)AmptoolTree_file->Get("kin");

    TFile * AmptoolTree_file_new = new TFile(file_path+"/PolInTree_"+file_name, "RECREATE"); 
    TTree * amptooltree_new = amptooltree->CloneTree(0);

    // polarization histogram file
    TFile * polarization_hist_path = new TFile(pol_Ebeam_hist_path, "r"); 
    TH1F* dHist_polarization_EBeam = (TH1F *)polarization_hist_path->Get(hist_name);

    Float_t Px_Beam, Py_Beam, Pz_Beam, E_Beam;
    amptooltree->SetBranchAddress("Px_Beam", &Px_Beam);
    amptooltree->SetBranchAddress("Py_Beam", &Py_Beam);
    amptooltree->SetBranchAddress("Pz_Beam", &Pz_Beam);
    amptooltree->SetBranchAddress("E_Beam", &E_Beam);


    int bin_num = 0;
    float pol_value = 0.0;
    Long64_t NEntries = amptooltree->GetEntries();
    // Shows how many events to be processed 
    cout<<"There are "<< NEntries <<" entries"<<endl;
    cout<< "____________________________" << endl; 

    // Loop all events
    for (Long64_t i_entry=0; i_entry<NEntries; i_entry++){
        amptooltree->GetEntry(i_entry);

        bin_num = dHist_polarization_EBeam->FindBin(E_Beam);

        if((bin_num!=0) && (bin_num < dHist_polarization_EBeam->GetNbinsX())){
            pol_value = dHist_polarization_EBeam->GetBinContent(bin_num);
        }
        Px_Beam = pol_value * TMath::Cos(polAng_deg * TMath::Pi()/ 180.0);
        Py_Beam = pol_value * TMath::Sin(polAng_deg * TMath::Pi()/ 180.0);
        Pz_Beam = 0.0;

        amptooltree_new->Fill();
    }

    AmptoolTree_file_new->Write();
    AmptoolTree_file_new->Close();

    return 1;
}