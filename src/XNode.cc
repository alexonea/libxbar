/*
 *  XNode.cc
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

#include "XNode.h"

#include <sstream>
#include <stdexcept>

namespace XBar
{
  bool
  isXP(XNodeType eType)
  {
    switch (eType)
    {
    case NP:
    case PP:
    case VP:
    case IP:
    case CP:
      return true;
    default:
      return false;
    }
  }

  bool
  isXBAR(XNodeType eType)
  {
    switch (eType)
    {
    case NBAR:
    case PBAR:
    case VBAR:
    case IBAR:
    case CBAR:
      return true;
    default:
      return false;
    }
  }

  bool
  isX(XNodeType eType)
  {
    switch (eType)
    {
    case N:
    case P:
    case V:
    case I:
    case C:
      return true;
    default:
      return false;
    }
  }

  bool
  isSpecifier(XNodeType eType)
  {
    return eType == SPECIFIER;
  }

  std::string
  toString(XNodeType eType)
  {
    switch (eType)
    {
      case NP:
        return "NP";
      case PP:
        return "PP";
      case VP:
        return "VP";
      case IP:
        return "IP";
      case CP:
        return "CP";
      case NBAR:
        return "\\=N";
      case PBAR:
        return "\\=P";
      case VBAR:
        return "\\=V";
      case IBAR:
        return "\\=I";
      case CBAR:
        return "\\=C";
      case I:
        return "I";
      case C:
        return "C";
      case N:
        return "N";
      case V:
        return "V";
      case P:
        return "P";
      case SPECIFIER:
        return "Specifier";
      default:
        throw std::runtime_error("Unknown type");
    }
  }

  XNode::XNode(enum XNodeType eType)
  : m_eType{eType}
  , m_sValue{toString(eType)}
  {}

  XNode::XNode(XNodeType eType, const std::string &sValue)
  : m_eType{eType}
  , m_sValue{sValue}
  {}

  std::string
  XNode::printToLatex()
  {
    std::stringstream ss;

    ss << "[.";
    if (isSpecifier(m_eType) || isX(m_eType))
      ss << toString(m_eType) << " ";

    ss << m_sValue << " ";

    if (m_pLeft)
      ss << m_pLeft->printToLatex();

    if (m_pRight)
      ss << m_pRight->printToLatex();

    ss << " ]";

    return ss.str();
  }

  XNode*
  XNode::appendToOrCreate(XNodeType eType, XNode *pNode)
  {
    XNode *pOut = new XNode{eType};

    if (pNode)
      pOut->addComplement(pNode);

    pOut->setHead();

    return pOut;
  }

  XNode*
  XNode::merge(XNode *pFirst, XNode *pLast)
  {
    if (!pFirst || !pLast)
      return nullptr;

    switch (pFirst->m_eType)
    {
      case NP:
      {
        if (pLast->m_eType == VP)
        {
          XNode* pInflection = XNode::appendToOrCreate(IP, pLast);
          return pInflection->setSpecifier(pFirst);
        }
        else if (pLast->m_eType == IP)
          return pLast->setSpecifier(pFirst);
        else
          return nullptr;
      }
      case PP:
      {
        if (pLast->m_eType == NP)
          return pFirst->addComplement(pLast);
        else
          return nullptr;
      }
      case VP:
      {
        if (pLast->m_eType == NP || pLast->m_eType == PP)
          return pFirst->addComplement(pLast);
        else
          return nullptr;
      }
      case IP:
      {
        if (pLast->m_eType == VP)
          return pFirst->addComplement(pLast);
        else
          return nullptr;
      }
      case CP:
        return nullptr;
      default:
        throw std::runtime_error("Bad operation");
    }
  }

  XNode*
  XNode::setSpecifier(const std::string &sValue)
  {
    return setSpecifier(new XNode{SPECIFIER, sValue});
  }

  XNode*
  XNode::setSpecifier(XNode *pSpecifier)
  {
    if (!isXP(m_eType))
      throw std::runtime_error("Bad operation");

    m_pLeft.reset(pSpecifier);

    return this;
  }

  XNode*
  XNode::addComplement(XNode *pComplement)
  {
    if (!isXP(m_eType))
      throw std::runtime_error("Bad operation");

    if (!m_pRight || (m_pRight && m_pRight->m_pRight))
    {
      /*
       * We rely on the fact that XBAR comes right after XP for each type.
       */
      XNodeType eComplementType = static_cast<XNodeType> (m_eType + 1);
      XNode *pBar = new XNode(eComplementType);

      pBar->m_pRight.reset(pComplement);

      if (m_pRight)
        pBar->m_pLeft = std::move(m_pRight);

      m_pRight.reset(pBar);
    }
    else
      m_pRight->m_pRight.reset(pComplement);

    return this;
  }

  XNode*
  XNode::setHead(const std::string &sValue)
  {
    if (isX(m_eType))
    {
      m_sValue = sValue;
      return this;
    }

    if (isXP(m_eType))
    {
      if (!m_pRight)
        addComplement(nullptr);

      return m_pRight->setHead(sValue);
    }

    if (isXBAR(m_eType))
    {
      if (!m_pLeft)
      {
        /*
         * We again rely on the relative position of XBAR and X in the
         * definition of the enumeration.
         */
        XNodeType eHeadType = static_cast<XNodeType> (m_eType + 1);
        m_pLeft.reset(new XNode{eHeadType});
      }

      return m_pLeft->setHead(sValue);
    }

   throw std::runtime_error("Bad operation");
  }
}
