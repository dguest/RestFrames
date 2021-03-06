/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2016, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   example_DiGluino_to_bbXbbX.C
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 April
//
//   This file is part of RestFrames.
//
//   RestFrames is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
// 
//   RestFrames is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with RestFrames. If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////

#include "RestFrames/RestFrames.hh"

using namespace std;
using namespace RestFrames;

void example_DiGluino_to_bbXbbX(string output_name =
				"output_DiGluino_to_bbXbbX.root"){
  SetLogPrint(LogVerbose,true);
  SetLogPrint(LogDebug,true);

  double mG = 1000.;
  double mX = 100.;
  int Ngen = 10000;

  //
  // Set up toy generation tree (not needed for reconstruction)
  LabGenFrame LAB_G("LAB_G","LAB");
  DecayGenFrame GG_G("GG_G","#tilde{g}#tilde{g}");
  DecayGenFrame Ga_G("Ga_G","#tilde{g}_{a}");
  DecayGenFrame Gb_G("Gb_G","#tilde{g}_{b}");
  VisibleGenFrame V1a_G("V1a_G","j_{1a}");
  VisibleGenFrame V2a_G("V2a_G","j_{2a}");
  VisibleGenFrame V3a_G("V3a_G","j_{3a}");
  InvisibleGenFrame Xa_G("Xa_G","#tilde{#chi}_{a}");
  VisibleGenFrame V1b_G("V1b_G","j_{1b}");
  VisibleGenFrame V2b_G("V2b_G","j_{2b}");
  InvisibleGenFrame Xb_G("Xb_G","#tilde{#chi}_{b}");
  LAB_G.SetChildFrame(GG_G);
  GG_G.AddChildFrame(Ga_G);
  GG_G.AddChildFrame(Gb_G);
  Ga_G.AddChildFrame(V1a_G);
  Ga_G.AddChildFrame(V2a_G);
  Ga_G.AddChildFrame(V3a_G);
  Ga_G.AddChildFrame(Xa_G);
  Gb_G.AddChildFrame(V1b_G);
  Gb_G.AddChildFrame(V2b_G);
  Gb_G.AddChildFrame(Xb_G);

  double mGG = 2.4*mG;// get a random di-gluino mass
  GG_G.SetMass(mGG);
  // set gluino masses
  Ga_G.SetMass(mG);
  Gb_G.SetMass(mG);
  // set X masses
  Xa_G.SetMass(mX);
  Xb_G.SetMass(mX);
  // V1a_G.SetMass(40.);
  // V3a_G.SetMass(80.);
  // V2a_G.SetMass(120.);

  V1b_G.SetMass(10.);
  V2b_G.SetMass(10.);

  if(!LAB_G.InitializeTree()) cout << "Problem with generator tree" << endl;
  if(!LAB_G.InitializeAnalysis()) cout << "Problem with generator tree" << endl;
  //
  //
									       

  // Set up 'signal-like' analysis tree
  LabRecoFrame LAB_R("LAB_R","LAB");
  DecayRecoFrame GG_R("GG_R","#tilde{g}#tilde{g}");
  DecayRecoFrame Ga_R("Ga_R","#tilde{g}_{a}");
  DecayRecoFrame Gb_R("Gb_R","#tilde{g}_{b}");
  DecayRecoFrame Ca_R("Ca_R","C_{a}");
  DecayRecoFrame Cb_R("Cb_R","C_{b}");
  VisibleRecoFrame V1a_R("V1a_R","j_{1a}");
  VisibleRecoFrame V2a_R("V2a_R","j_{2a}");
  InvisibleRecoFrame Xa_R("Xa_R","#tilde{#chi}_{a}");
  VisibleRecoFrame V1b_R("V1b_R","j_{1b}");
  VisibleRecoFrame V2b_R("V2b_R","j_{2b}");
  InvisibleRecoFrame Xb_R("Xb_R","#tilde{#chi}_{b}");
  LAB_R.SetChildFrame(GG_R);
  GG_R.AddChildFrame(Ga_R);
  GG_R.AddChildFrame(Gb_R);
  Ga_R.AddChildFrame(V1a_R);
  Ga_R.AddChildFrame(Ca_R);
  Ca_R.AddChildFrame(V2a_R);
  Ca_R.AddChildFrame(Xa_R);
  Gb_R.AddChildFrame(V1b_R);
  Gb_R.AddChildFrame(Cb_R);
  Cb_R.AddChildFrame(V2b_R);
  Cb_R.AddChildFrame(Xb_R);

  // Set up 'background-like' analysis tree
  LabRecoFrame LAB_B("LAB_B","LAB");
  SelfAssemblingRecoFrame CM_B("CM_B","CM");
  VisibleRecoFrame V_B("V_B","Vis");
  InvisibleRecoFrame I_B("I_B","Inv");
  LAB_B.SetChildFrame(CM_B);
  CM_B.AddChildFrame(V_B);
  CM_B.AddChildFrame(I_B);
 
  if(!LAB_R.InitializeTree()) cout << "Problem with signal-like reconstruction tree" << endl; 
  if(!LAB_B.InitializeTree()) cout << "Problem with background-like reconstruction tree" << endl; 

  // define groups for the reconstruction trees
  
  InvisibleGroup INV_R("INV_R","WIMP Jigsaws");
  INV_R.AddFrame(Xa_R);
  INV_R.AddFrame(Xb_R);
  CombinatoricGroup VIS_R("VIS","Visible Object Jigsaws");

  // visible frames in first decay step must always have at least one element
  VIS_R.AddFrame(V1a_R);
  VIS_R.AddFrame(V1b_R);
  VIS_R.SetNElementsForFrame(V1a_R,1,false);
  VIS_R.SetNElementsForFrame(V1b_R,1,false);
  // visible frames in second decay step can have zero elements
  VIS_R.AddFrame(V2a_R);
  VIS_R.AddFrame(V2b_R);
  VIS_R.SetNElementsForFrame(V2a_R,0,false);
  VIS_R.SetNElementsForFrame(V2b_R,0,false);

  InvisibleGroup INV_B("INV_B","Invisible State Jigsaws");
  INV_B.AddFrame(I_B);
  CombinatoricGroup VIS_B("VIS_B","Visible Object Jigsaws");
  VIS_B.AddFrame(V_B);
  VIS_B.SetNElementsForFrame(V_B,1,false);

  // define jigsaws for the reconstruction trees
  
  // signal-like jigsaws
  SetMassInvJigsaw MinMassJigsaw_R("MINMASS_R", "Invisible system mass Jigsaw");
  INV_R.AddJigsaw(MinMassJigsaw_R);
  SetRapidityInvJigsaw RapidityJigsaw_R("RAPIDITY_R", "Invisible system rapidity Jigsaw");
  INV_R.AddJigsaw(RapidityJigsaw_R);
  RapidityJigsaw_R.AddVisibleFrames((LAB_R.GetListVisibleFrames()));
  ContraBoostInvJigsaw ContraBoostJigsaw_R("CONTRA_R","Contraboost invariant Jigsaw");
  INV_R.AddJigsaw(ContraBoostJigsaw_R);
  ContraBoostJigsaw_R.AddVisibleFrames((Ga_R.GetListVisibleFrames()), 0);
  ContraBoostJigsaw_R.AddVisibleFrames((Gb_R.GetListVisibleFrames()), 1);
  ContraBoostJigsaw_R.AddInvisibleFrames((Ga_R.GetListInvisibleFrames()), 0);
  ContraBoostJigsaw_R.AddInvisibleFrames((Gb_R.GetListInvisibleFrames()), 1);
  MinMassesCombJigsaw HemiJigsaw_R("HEM_JIGSAW_R","Minimize m _{V_{a,b}} Jigsaw");
  VIS_R.AddJigsaw(HemiJigsaw_R);
  HemiJigsaw_R.AddFrame(V1a_R,0);
  HemiJigsaw_R.AddFrame(V1b_R,1);
  HemiJigsaw_R.AddFrame(V2a_R,0);
  HemiJigsaw_R.AddFrame(V2b_R,1);
  MinMassesCombJigsaw CaHemiJigsaw_R("CaHEM_JIGSAW_R","Minimize m _{C_{a}} Jigsaw");
  VIS_R.AddJigsaw(CaHemiJigsaw_R);
  CaHemiJigsaw_R.AddFrame(V1a_R,0);
  CaHemiJigsaw_R.AddFrame(V2a_R,1);
  CaHemiJigsaw_R.AddFrame(Xa_R,1);
  MinMassesCombJigsaw CbHemiJigsaw_R("CbHEM_JIGSAW_R","Minimize m _{C_{b}} Jigsaw");
  VIS_R.AddJigsaw(CbHemiJigsaw_R);
  CbHemiJigsaw_R.AddFrame(V1b_R,0);
  CbHemiJigsaw_R.AddFrame(V2b_R,1);
  CbHemiJigsaw_R.AddFrame(Xb_R,1);

  // background tree jigsaws
  SetMassInvJigsaw MinMassJigsaw_B("MINMASS_B","Zero Mass for invisible system");
  INV_B.AddJigsaw(MinMassJigsaw_B);
  SetRapidityInvJigsaw RapidityJigsaw_B("RAPIDITY_B","Invisible system rapidity Jigsaw");
  INV_B.AddJigsaw(RapidityJigsaw_B);
  RapidityJigsaw_B.AddVisibleFrames((LAB_B.GetListVisibleFrames()));

  // check reconstruction trees
  if(!LAB_R.InitializeAnalysis()) cout << "Problem with signal-tree jigsaws" << endl;
  if(!LAB_B.InitializeAnalysis()) cout << "Problem with background-tree jigsaws" << endl;

  //////////////////////////////////////////////////////////////
  // draw some pictures of our trees
  //////////////////////////////////////////////////////////////

  TreePlot* tree_plot = new TreePlot("TreePlot","TreePlot");
 
  // generator tree
  tree_plot->SetTree(LAB_G);
  tree_plot->Draw("GenTree", "Generator Tree");

  // signal reco tree
  tree_plot->SetTree(LAB_R);
  tree_plot->AddJigsaw(ContraBoostJigsaw_R);
  tree_plot->AddJigsaw(HemiJigsaw_R);
  tree_plot->AddJigsaw(CaHemiJigsaw_R);
  tree_plot->AddJigsaw(CbHemiJigsaw_R);
  tree_plot->Draw("SigRecoTree", "Signal Reconstruction Tree");
  tree_plot->Draw("SigRecoTree", "Signal Reconstruction Tree", true);

  // background reco tree
  tree_plot->SetTree(LAB_B);
  tree_plot->Draw("BkgRecoTree", "Background Reconstruction Tree");

  // Invisible Jigsaws
  tree_plot->SetTree(INV_R);
  tree_plot->Draw("InvTree", "Invisible Objects Jigsaws");

  // Visible Jigsaws
  tree_plot->SetTree(VIS_R);
  tree_plot->Draw("VisTree", "Visible Objects Jigsaws");

  TH2D* h_M12_v_M13 = new TH2D("h_M12_v_M13","h_M12_v_M13",50,0.,1.,50,0.,1.);

  // function for randomly determining di-gluino mass 
  // (relative to gluino mass via gamma)
  //TF1 f_gamma("f_gamma","(x-1)*exp(-2.*x)",1.,10.);
  for(int igen = 0; igen < Ngen; igen++){
    if(igen%(max(Ngen/10,1)) == 0) 
      g_Log << LogInfo << "Generating event " << igen << " of " << Ngen << LogEnd;

    // generate event
    LAB_G.ClearEvent();                             // clear the gen tree
    //double mGG = 2.*mG*f_gamma.GetRandom();  
 
    double PTGG = mGG*gRandom->Rndm();
    LAB_G.SetTransverseMomentum(PTGG);               // give the di-gluinos some Pt
    double PzGG = mGG*(2.*gRandom->Rndm()-1.);
    LAB_G.SetLongitudinalMomentum(PzGG);             // give the di-gluinos some Pz
    LAB_G.AnalyzeEvent();                           // generate a new event

   
    TLorentzVector vj1 = V1a_G.GetFourVector();
    TLorentzVector vj2 = V2a_G.GetFourVector();
    TLorentzVector vj3 = V3a_G.GetFourVector();
    TLorentzVector vX  = Xa_G.GetFourVector();
    

    h_M12_v_M13->Fill((vj1+vj2).M2()/(vj1+vj2+vj3).M2(), 
		      (vj3+vj2).M2()/(vj1+vj2+vj3).M2());

    // analyze event
    TVector3 MET = LAB_G.GetInvisibleMomentum();    // Get the MET from gen tree
    MET.SetZ(0.);
    vector<TLorentzVector> JETS;                    // Get the Jets from gen tree
    JETS.push_back(V1a_G.GetFourVector());
    JETS.push_back(V2a_G.GetFourVector());
    JETS.push_back(V1b_G.GetFourVector());
    JETS.push_back(V2b_G.GetFourVector());

    // vector<TLorentzVector> JETS;
    // TLorentzVector j1, j2;
    // j1.SetPtEtaPhiM(4.20836e6,-0.240686,2.52994,0.);
    // j2.SetPtEtaPhiM(4.20552e6,0.02966,-0.607627,134116.);
    // JETS.push_back(j1);
    // JETS.push_back(j2);

    // give the signal-like tree the event info and analyze
    LAB_R.ClearEvent();                              // clear the signal-like tree
    INV_R.SetLabFrameThreeVector(MET);               // Set the MET in reco tree
    vector<RFKey> jetID;                    // ID for tracking jets in tree
    for(int i = 0; i < int(JETS.size()); i++) 
      jetID.push_back(VIS_R.AddLabFrameFourVector(JETS[i]));
    LAB_R.AnalyzeEvent();                            // analyze the event

    // give the background-like tree the event info and analyze
    LAB_B.ClearEvent();                                   // clear the bkg-like tree
    INV_B.SetLabFrameThreeVector(MET);                    // Set the MET in tree
    for(int i = 0; i < int(JETS.size()); i++){
      TLorentzVector jet = JETS[i];
      jet.SetPtEtaPhiM(jet.Pt(), 0., jet.Phi(), jet.M()); // only pass transverse info to bkg-like tree
      VIS_B.AddLabFrameFourVector(jet);
    }
    LAB_B.AnalyzeEvent(); // analyze the event

    DecayRecoFrame* G[2];
    DecayRecoFrame* C[2];
    VisibleRecoFrame* VS[2];
    VisibleRecoFrame* VC[2];
    InvisibleRecoFrame* X[2];
    // Randomize the two hemispheres
    int flip = (gRandom->Rndm() > 0.5);
    G[flip] = &Ga_R;
    G[(flip+1)%2] = &Gb_R;
    C[flip] = &Ca_R;
    C[(flip+1)%2] = &Cb_R;
    VS[flip] = &V1a_R;
    VS[(flip+1)%2] = &V1b_R;
    VC[flip] = &V2a_R;
    VC[(flip+1)%2] = &V2b_R;
    X[flip] = &Xa_R;
    X[(flip+1)%2] = &Xb_R;

    //////////////////////////////////////
    // Observable Calculations
    //////////////////////////////////////

    //
    // signal tree observables
    //

    //*** total CM mass
    double shat = GG_R.GetMass();
    //*** 'mass-less' gluino gamma in CM frame
    double gaminv = GG_R.GetVisibleShape();
    
    TVector3 vPGG = GG_R.GetFourVector(LAB_R).Vect();
    
    //*** ratio of CM pT to CM mass
    double RPT = vPGG.Pt() / (vPGG.Pt() + shat/4.);
    //*** ratio of CM pz to CM mass
    double RPZ = vPGG.Pz() / (vPGG.Pz() + shat/4.);
    //*** cos decay angle of GG system
    double cosGG = GG_R.GetCosDecayAngle();
    //*** delta phi between lab and GG decay planes
    double dphiLGG = LAB_R.GetDeltaPhiDecayPlanes(GG_R);
    
    TLorentzVector vV1 = G[0]->GetVisibleFourVector(*G[0]);
    TLorentzVector vV2 = G[1]->GetVisibleFourVector(*G[1]);

    //*** gluino mass
    double MG = (vV1.M2()-vV2.M2())/(2.*(vV1.E()-vV2.E()));
    
    double PG = G[0]->GetMomentum(GG_R);
    double MGG = 2.*sqrt(PG*PG + MG*MG);
    double gaminvGG = 2.*MG/MGG;
    double beta = sqrt(1.- gaminv*gaminv);
    double betaGG = sqrt(1.- gaminvGG*gaminvGG);
    
    //*** velocity difference between 'massive' and 'mass-less'
    double DeltaBetaGG = -(betaGG-beta)/(1.-betaGG*beta);
    //*** delta phi between GG visible decay products and GG decay axis
    double dphiVG = GG_R.GetDeltaPhiDecayVisible();
    //*** delta phi between GG visible decay products and GG momentum
    double dphiVGG = GG_R.GetDeltaPhiBoostVisible();
    
    // 'hemisphere' (one for each 'gluino') observables

    //*** number of visible objects (jets) in hemisphere
    double NV[2];
    //*** cosine gluino decay angle
    double cosG[2];
    //*** cosine intermediate child decay angle
    double cosC[2];
    //*** delta phi between gluino and child decay planes
    double dphiGC[2];
    //*** ratio of child and gluino masses (w/ WIMP masses subtracted)
    double RCG[2];
    //*** 1st leading jet pT _associated with this hemisphere_
    double jet1PT[2];
    //*** 2nd leading jet pT _associated with this hemisphere_
    double jet2PT[2];
    //*** Pinv / HG
    double Pinv[2];
      
    for(int i = 0; i < 2; i++){
      NV[i] =  VIS_R.GetNElementsInFrame(*VS[i]);
      NV[i] += VIS_R.GetNElementsInFrame(*VC[i]);

      TVector3 vP1 = VS[i]->GetFourVector(*G[i]).Vect();
      TVector3 vP2 = VC[i]->GetFourVector(*G[i]).Vect();
      Pinv[i] = 2.*(vP1+vP2).Mag()/(vP1.Mag()+vP2.Mag()+(vP1+vP2).Mag());

      cosG[i] = G[i]->GetCosDecayAngle();

      int N = jetID.size();
      double pTmax[2]; pTmax[0] = -1.; pTmax[1] = -1.;
      for(int j = 0; j < N; j++){
	const RestFrame& frame = VIS_R.GetFrame(jetID[j]);
	if(VS[i]->IsSame(frame) || VC[i]->IsSame(frame)){
	  double pT = VIS_R.GetLabFrameFourVector(jetID[j]).Pt();
	  if(pT > pTmax[0]){
	    pTmax[1] = pTmax[0];
	    pTmax[0] = pT;
	  } else {
	    if(pT > pTmax[1]) pTmax[1] = pT;
	  }
	}
      }
      
      jet1PT[i] = pTmax[0];
      jet2PT[i] = pTmax[1];

      if(NV[i] > 1){
	cosC[i] = C[i]->GetCosDecayAngle();
	dphiGC[i] = G[i]->GetDeltaPhiDecayPlanes(*C[i]);
	RCG[i] = (C[i]->GetMass()-X[i]->GetMass())/(G[i]->GetMass()-X[i]->GetMass());
      } else {
	cosC[i] = -2.;
	dphiGC[i] = -1.;
	RCG[i] = -1.;
	jet2PT[i] = -1.;
      }
    }

    //
    // background tree observables
    //
    TLorentzVector Psib = I_B.GetSiblingFrame().GetFourVector(LAB_B);
    TLorentzVector Pmet = I_B.GetFourVector(LAB_B);
      
    //*** 
    double Rpsib = max(0.,Psib.Vect().Dot(Pmet.Vect().Unit()));
    Rpsib = Rpsib / (Pmet.Pt() + Rpsib);
    
    TVector3 boostQCD = (Pmet+Psib).BoostVector();
    Psib.Boost(-boostQCD);
    double cosQCD = -1.*Psib.Vect().Unit().Dot(boostQCD.Unit());
    cosQCD = (1.-cosQCD)/2.;

    //*** 
    double DeltaQCD = (cosQCD-Rpsib)/(cosQCD+Rpsib);

  }
  
  h_M12_v_M13->Draw("COLZ");

  // tree_plot->WriteOutput(output_name);
}

# ifndef __CINT__ // main function for stand-alone compilation
int main(){
  example_DiGluino_to_bbXbbX();
  return 0;
}
#endif
