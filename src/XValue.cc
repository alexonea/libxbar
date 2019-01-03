/*
 *  XValue.cc
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

#include "XValue.h"

namespace XBar
{
  XValue::XValue()
  : m_ePos{GENERIC}
  {}

  XValue::XValue(const PartOfSpeech &ePos)
  : m_ePos{ePos}
  {}

  XValue::XValue(const std::string &sBase, const PartOfSpeech &ePos)
  : m_sBase{sBase}
  , m_ePos{ePos}
  {}

  std::string
  XValue::toString()
  {
    return doToString();
  }

  std::string
  XValue::doToString()
  {
    return m_sBase;
  }

  XValue*
  XValue::clone()
  {
    return new XValue{m_sBase};
  }
}
