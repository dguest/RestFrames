/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   ResonanceGenFrame.cc
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 Jan
///
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

#include <stdlib.h>
#include "RestFrames/ResonanceGenFrame.hh"

using namespace std;

namespace RestFrames {

  ///////////////////////////////////////////////
  // ResonanceGenFrame class
  ///////////////////////////////////////////////

  ResonanceGenFrame::ResonanceGenFrame(const string& sname, const string& stitle) : 
    RestFrame(sname,stitle),
    DecayGenFrame(sname,stitle)
  {
    Init();
  }

  ResonanceGenFrame::ResonanceGenFrame() : 
    RestFrame(),
    DecayGenFrame("Empty","Empty")
  {
    Init();
  }
  
  ResonanceGenFrame::~ResonanceGenFrame(){ }

  void ResonanceGenFrame::Init(){
    m_PoleMass = 0.;
    m_Width = 0.;
  }

  void ResonanceGenFrame::ResetFrame(){
    SetSpirit(false);
    m_MassSet = false;
    ResetDecayAngles();
  }

  void ResonanceGenFrame::SetMass(double val){
    if(val < 0.){
      m_Log << LogWarning;
      m_Log << "Unable to set mass to negative value ";
      m_Log << val << ". Setting to zero." << m_End;
      m_PoleMass = 0.;
    } else {
      m_PoleMass = val;
    }
  }

  void ResonanceGenFrame::SetEvtMass(double val){
    if(val < 0.){
      m_Log << LogWarning;
      m_Log << "Unable to set mass to negative value ";
      m_Log << val << ". Setting to zero." << m_End;
      m_Mass = 0.;
    } else {
      m_Mass = val;
    }
    m_MassSet = true;
  }

  void ResonanceGenFrame::SetWidth(double val){
    if(val < 0.){
      m_Log << LogWarning;
      m_Log << "Unable to set width to negative value ";
      m_Log << val << ". Setting to zero." << m_End;
      m_Width = 0.;
    } else 
      m_Width = val;
  }

  double ResonanceGenFrame::GetMass() const {
    if(m_MassSet) return m_Mass;

    if(m_Width <= 0.)
      m_Mass = m_PoleMass;
    else {
      double min = GetMinimumMass();
      m_Mass = -1.;
      while(m_Mass < min)
	m_Mass = GenerateMass();
    }

    m_MassSet = true;
    return m_Mass;
  }

  double ResonanceGenFrame::GetPoleMass() const {
    return m_PoleMass;
  }

  double ResonanceGenFrame::GetMinimumMass() const {
    if(!IsSoundBody()) 
      return 0.;
    double Mass = 0.;
    int N = GetNChildren();
    for(int i = 0; i < N; i++){
      ResonanceGenFrame* framePtr  = 
	dynamic_cast<ResonanceGenFrame*>(&GetChildFrame(i));
      if(framePtr){
	Mass += framePtr->GetMinimumMass();
      } else {
	Mass += GetChildFrame(i).GetMass();
      }
    }
    return Mass;
  }

  double ResonanceGenFrame::GetWidth() const {
    return m_Width;
  }

  double ResonanceGenFrame::GetProb(double mass) const {
    if(m_PoleMass <= 0. || mass < 0.)
      return 0.;

    double den = (mass*mass-m_PoleMass*m_PoleMass)*(mass*mass-m_PoleMass*m_PoleMass)
      + mass*mass*mass*mass*m_Width*m_Width/m_PoleMass/m_PoleMass;
    if(den <= 0.)
      return 0.;
    else 
      return mass*mass/den;
  }

  double ResonanceGenFrame::GenerateMass(double min_mass) const {
    if(m_Width <= 0. || m_PoleMass <= 0.)
      return 0.;
    double A = -m_PoleMass/m_Width;
    double Mass = atan(A) + GetRandom()*(TMath::Pi()/2.-atan(A));
    return sqrt(Mass);
  }

}
