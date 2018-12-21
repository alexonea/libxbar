/*
 *  XNode.h
 *
 *  Copyright (C) 2018 Alexandru N. Onea <alexandru.onea@toporcomputing.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef XNODE_H_
#define XNODE_H_

#include "verbs.h"

#include <memory>
#include <string>

namespace XBar
{
  enum XNodeType
  {
    NP, NBAR, N,
    PP, PBAR, P,
    VP, VBAR, V,
    CP, CBAR, C,
    IP, IBAR, I,
    SPECIFIER,
  };

  extern bool         isXP            (XNodeType eType);
  extern bool         isXBAR          (XNodeType eType);
  extern bool         isX             (XNodeType eType);
  extern bool         isSpecifier     (XNodeType eType);
  extern std::string  toString        (XNodeType eType);

  class XNode
  {
  public:
    XNode             (XNodeType eType);
    XNode             (XNodeType eType, const std::string &sValue);

    XNode* getLeft    () const        { return m_pLeft.get();   };
    XNode* getRight   () const        { return m_pRight.get();  };

    std::string printToLatex ();

    XNode* setSpecifier   (const std::string &sValue);
    XNode* setSpecifier   (XNode *pSpecifier);
    XNode* addComplement  (XNode *pComplement);
    XNode* setHead        (const std::string &sValue = "");
    XNode* setHead        (Verb *pValue);
    XNode* getHead        ();

    static XNode* appendToOrCreate  (XNodeType eType, XNode *pNode = nullptr);
    static XNode* merge             (XNode *pFirst, XNode *pLast);
  private:
    XNodeType                 m_eType;
    std::string               m_sValue;
    std::unique_ptr<Verb>     m_pVerb;
    std::unique_ptr<XNode>    m_pLeft;
    std::unique_ptr<XNode>    m_pRight;
  };
}

#endif /* XNODE_H_ */
