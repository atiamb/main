/*
* 
*
* This file is part of BRAT 
*
* BRAT is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* BRAT is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#if !defined(_TreeField_h_)
#define _TreeField_h_

#include <string>

#include "common/tools/brathl_error.h"
#include "brathl.h" 

#include "List.h"
#include "ObjectTree.h"
#include "Field.h"

using namespace brathl;

namespace brathl
{

//typedef ObjectTree<std::string, CField*> CTree_field;

/** \addtogroup product Products
  @{ */

/** 
  Tree fields management class.


 \version 1.0
*/



class CTreeField : public CObjectTree
{

public:
    
  /// Empty CTreeField ctor
  CTreeField();

  /// Destructor
  virtual ~CTreeField();

  virtual CObjectTreeIterator AddChild (CObjectTreeNode* parent, const std::string& nm, CField* x, bool goCurrent = false);

  virtual CObjectTreeIterator AddChild (CObjectTreeIterator& parent, const std::string& nm, CField* x, bool goCurrent = false);

  virtual CObjectTreeIterator AddChild (const std::string& nm, CField* x, bool goCurrent = false); 

  void ResetHiddenFlag ();

  void DumpDictionary(std::ostream& fOut = std::cout);
  void DumpDictionary(const std::string& outputFileName);


  /// Dump function
  virtual void Dump(std::ostream& fOut = std::cerr);

  static CFieldRecord * GetDataAsFieldRecordObject(CObjectTreeNode* node, bool withExcept  = true);
  static CField * GetDataAsFieldObject(CObjectTreeNode* node, bool withExcept  = true);
  CField * GetCurrentData(bool withExcept = true);
  CField * GetParentData(bool withExcept  = true);

  CField* FindParent(CField* field);

  CField* GetRootData();



protected:

private:
  

public:
  static const std::string m_keyDelimiter;

protected:
		
		
private:
  

};

/** @} */

}

#endif // !defined(_TreeField_h_)
