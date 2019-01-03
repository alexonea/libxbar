/*
 *  XValue.h
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

#ifndef XVALUE_H_
#define XVALUE_H_

#include <string>

#include "WordNet.h"

namespace XBar
{
  class XValue
  {
  public:
    XValue(const std::string &sBase, const PartOfSpeech &ePos = GENERIC);
    XValue(const PartOfSpeech &ePos);
    XValue();
    virtual ~XValue() = default;

    std::string   toString();

    void        setValue(const std::string &sBase) { m_sBase = sBase; };
    std::string getValue()                         { return m_sBase;  };

    virtual PartOfSpeech getPOS()                  { return m_ePos;   };

    virtual XValue*      clone();
  private:
    std::string   m_sBase;
    PartOfSpeech  m_ePos;

    virtual std::string doToString();
  };
}

#endif /* XVALUE_H_ */
