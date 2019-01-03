/*
 *  XVerb.cc
 *
 *  Copyright (C) 2019 Alexandru N. Onea <alexandru.onea@toporcomputing.com>
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

#include "XVerb.h"

namespace XBar
{
  XVerb::XVerb()
  : XValue{VERB}
  {}

  XVerb::XVerb(const std::string &sBase)
  : XValue{sBase, VERB}
  {}

  void
  XVerb::setTense(bool bProgressive, const VerbTense &eTense)
  {
    m_tense.bProgressive = bProgressive;
    if (eTense != INVALID)
      m_tense.eTense = eTense;
  }

  void
  XVerb::setTense(const VerbTense &eTense)
  {
    m_tense.eTense = eTense;
  }

  void
  XVerb::setTense(const VerbTenseInfo &tenseInfo)
  {
    m_tense = tenseInfo;
  }

  XValue*
  XVerb::clone()
  {
    return new XVerb{*this};
  }

  void
  XVerb::addAdverb(const std::string &sAdverb)
  {
    m_vAdverbs.push_back(sAdverb);
  }
}
