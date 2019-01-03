/*
 *  XVerb.h
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

#ifndef XVERB_H_
#define XVERB_H_

#include <string>
#include <vector>

#include "XValue.h"

namespace XBar
{
  enum VerbTense
  {
    INVALID = 0,
    PRESENT,
    PAST,
    PRESENT_PERFECT,
    PAST_PERFECT,
    FUTURE_I,
    FUTURE_II,
    CONDITIONAL_I,
    CONDITIONAL_II,
  };

  struct VerbTenseInfo
  {
    VerbTense eTense;
    bool  bProgressive;

    VerbTenseInfo() : eTense(INVALID), bProgressive(false) {};
  };


  class XVerb : public XValue
  {
  public:
    XVerb();
    XVerb(const std::string &sBase);
    virtual ~XVerb()  = default;

    void setTense(bool bProgressive, const VerbTense &eTense = INVALID);
    void setTense(const VerbTense &eTense);
    void setTense(const VerbTenseInfo &tenseInfo);

    void addAdverb(const std::string &sAdverb);

    virtual XValue* clone() override;
  private:
    VerbTenseInfo             m_tense;
    std::vector<std::string>  m_vAdverbs;
  };
}

#endif /* XVERB_H_ */
