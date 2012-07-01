/*This file is part of Rdock.

    Rdock is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Rdock is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Rdock.  If not, see <http://www.gnu.org/licenses/>.*/

#include "RbtBaseBiMolTransform.h"
#include "RbtWorkSpace.h"

//Static data members
RbtString RbtBaseBiMolTransform::_CT("RbtBaseBiMolTransform");

////////////////////////////////////////
//Constructors/destructors
RbtBaseBiMolTransform::RbtBaseBiMolTransform(const RbtString& strClass, const RbtString& strName) :
		RbtBaseTransform(strClass,strName)
{
#ifdef _DEBUG
	cout << _CT << " parameterised constructor" << endl;
#endif //_DEBUG
  _RBTOBJECTCOUNTER_CONSTR_(_CT);
}

RbtBaseBiMolTransform::~RbtBaseBiMolTransform()
{
#ifdef _DEBUG
	cout << _CT << " destructor" << endl;
#endif //_DEBUG
  _RBTOBJECTCOUNTER_DESTR_(_CT);
}

////////////////////////////////////////
//Public methods
////////////////

RbtModelPtr RbtBaseBiMolTransform::GetReceptor() const {return m_spReceptor;}
RbtModelPtr RbtBaseBiMolTransform::GetLigand() const {return m_spLigand;}
RbtModelList RbtBaseBiMolTransform::GetSolvent() const {return m_solventList;}
	
//Override RbtObserver pure virtual
//Notify observer that subject has changed
void RbtBaseBiMolTransform::Update(RbtSubject* theChangedSubject) {
	RbtWorkSpace* pWorkSpace = GetWorkSpace();
	if (theChangedSubject == pWorkSpace) {
		RbtInt numModels = pWorkSpace->GetNumModels();
		//Check if receptor has been updated (model #0)
		if (numModels >= 1) {
			RbtModelPtr spReceptor = pWorkSpace->GetModel(0);
			if (spReceptor != m_spReceptor) {
#ifdef _DEBUG
				cout << _CT << "::Update(): Receptor has been updated" << endl;
#endif //_DEBUG
				m_spReceptor = spReceptor;
				SetupReceptor();
			}
		}
		//Check if ligand has been updated (model #1)
		if (numModels >= 2) {		
			RbtModelPtr spLigand = pWorkSpace->GetModel(1);
			if (spLigand != m_spLigand) {
#ifdef _DEBUG
				cout << _CT << "::Update(): Ligand has been updated" << endl;
#endif //_DEBUG
				m_spLigand = spLigand;
				SetupLigand();
			}
		}
		//check if solvent has been updated (model #2 onwards)
		//At the moment, we only check whether the number of solvent
		//molecules has changed
 		RbtModelList solventList;
    	if (numModels >= 3) {
			solventList = pWorkSpace->GetModels(2);
    	}
    	if (solventList.size() != m_solventList.size()) {
    		m_solventList = solventList;
    		SetupSolvent();
    	}
		SetupTransform();
	}
}
