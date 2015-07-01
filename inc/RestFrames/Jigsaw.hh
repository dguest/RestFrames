/////////////////////////////////////////////////////////////////////////
//   RestFrames: particle physics event analysis library
//   --------------------------------------------------------------------
//   Copyright (c) 2014-2015, Christopher Rogan
/////////////////////////////////////////////////////////////////////////
///
///  \file   Jigsaw.hh
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

#ifndef Jigsaw_HH
#define Jigsaw_HH

#include "RestFrames/RFBase.hh"
#include "RestFrames/StateList.hh"

using namespace std;

namespace RestFrames {

  class Group;

  enum JigsawType { JInvisible, JCombinatoric };

  ///////////////////////////////////////////////
  // Jigsaw class
  ///////////////////////////////////////////////
  class Jigsaw : public RFBase {
  public:
    Jigsaw(const string& sname, const string& stitle);
    
    virtual ~Jigsaw();
  
    virtual void Clear();

    virtual bool IsSoundBody() const;

    JigsawType GetType() const { return m_Type; }
  
    /// \brief String of information associated with object
    virtual string PrintString(LogType type = LogVerbose) const;

    int GetNOutputStates() const;
    int GetNDependancyStates() const;

    bool IsInvisibleJigsaw() const;
    bool IsCombinatoricJigsaw() const;

    virtual void SetGroup(Group* groupPtr);
    Group* GetGroup() const;

    bool CanSplit(const State* statePtr);
    bool CanSplit(const RestFrames::RFList<RestFrame>* framesPtr);
  
    virtual StateList* InitializeOutputStates(State* statePtr);
    virtual bool InitializeDependancyStates(const StateList* statesPtr, const RestFrames::RFList<Group>* groupsPtr);
    virtual bool InitializeDependancyJigsaws();

    virtual int GetNChildStates() const;
    virtual State* GetChildState(int i) const;
    virtual RFList<RestFrame>* GetChildFrames(int i) const;
    virtual RFList<RestFrame>* GetChildFrames() const;

    virtual void FillGroupJigsawDependancies(RestFrames::RFList<Jigsaw>* jigsawsPtr);
    virtual void FillStateJigsawDependancies(RestFrames::RFList<Jigsaw>* jigsawsPtr);

    virtual bool InitializeJigsawExecutionList(RestFrames::RFList<Jigsaw>* chain_jigsawPtr) = 0;
    bool DependsOnJigsaw(Jigsaw* jigsawPtr);

    virtual bool AnalyzeEvent() = 0;
  
  protected:
    static int m_class_key;

    JigsawType m_Type;

    Group* m_GroupPtr;
    State* m_InputStatePtr;

    vector<RestFrames::RFList<RestFrame> > m_OutputFrames;
    vector<RestFrames::RFList<RestFrame> > m_DependancyFrames;

    StateList m_OutputStates;
    vector<StateList> m_DependancyStates;

    RestFrames::RFList<Jigsaw> m_DependancyJigsaws;

    virtual State* NewOutputState();
    void AddOutputFrame(RestFrame* framePtr, int i = 0);
    void AddOutputFrame(RFList<RestFrame>* framesPtr, int i = 0);
    void AddDependancyFrame(RestFrame* framePtr, int i = 0);
    void AddDependancyFrame(RFList<RestFrame>* framesPtr, int i = 0);

  private:
    void Init();
    int GenKey();
    void ClearOutputStates();
    void ClearDependancyStates();
  };
}

#endif
