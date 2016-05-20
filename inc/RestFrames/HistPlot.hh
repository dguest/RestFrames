/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   HistPlot.hh
///
///  \author Christopher Rogan
///          (crogan@cern.ch)
///
///  \date   2015 Jul
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

#ifndef HistPlot_HH
#define HistPlot_HH

#include <utility>
#include <TH1D.h>
#include <TH2D.h>

#include "RestFrames/RFPlot.hh"

namespace RestFrames {

  class HistPlotVar;
  class HistPlotCategory;

  class HistPlot : public RFPlot {

  public:
    HistPlot(const string& sname, const string& stitle);
    ~HistPlot();

    virtual void Clear();

    HistPlotVar const& GetNewVar(const string& name, const string& title, 
				 double minval, double maxval,
				 const string& unit = "");

    HistPlotCategory const& GetNewCategory(const string& name, const string& title);
    
    void AddPlot(const HistPlotVar& var, 
		 RestFrames::RFList<const HistPlotCategory> cats = 
		 RFList<const HistPlotCategory>());
    void AddPlot(const HistPlotVar& varX,
		 const HistPlotVar& varY,
		 RestFrames::RFList<const HistPlotCategory> cats =
		 RFList<const HistPlotCategory>());
    
    void Fill(double weight = 1.);
    void Fill(const HistPlotCategory& cat, double weight = 1.);

    void Draw();

    void SetPlotLabel(const string& label);

    void SetPlotTitle(const string& title);

    void SetScaleLabel(const string& label);

    void SetScale(double scale = -1);

    void WriteHist(const string& filename);

  private:
    string m_PlotLabel;
    string m_PlotTitle;
    string m_ScaleLabel;
    double m_Scale;
    bool   m_SetScale;
    
    vector<TH1D*> m_1DHists;
    vector<TH2D*> m_2DHists;
    vector<HistPlotVar*>      m_Vars;
    vector<HistPlotCategory*> m_Cats;
    map<const HistPlotCategory*,vector<TH1D*> > m_CatToHist1D;
    map<const HistPlotCategory*,vector<TH2D*> > m_CatToHist2D;
    vector<const HistPlotVar*>                    m_Plot1D_Var;
    vector<RestFrames::RFList<HistPlotCategory> > m_Plot1D_Cats;
    vector<pair<const HistPlotVar*,
		const HistPlotVar*> >             m_Plot2D_Vars;
    vector<const HistPlotCategory*>               m_Plot2D_Cat;

    map<TH1D*,const HistPlotVar*>        m_HistToVar;
    map<TH2D*,pair<const HistPlotVar*,
		   const HistPlotVar*> > m_HistToVars;

    void DrawPlot(const HistPlotVar& var, 
		  const RFList<HistPlotCategory>& cats);
    void DrawPlot(const pair<const HistPlotVar*, const HistPlotVar*>& vars,
		  const HistPlotCategory& cat);

  };

}

#endif
