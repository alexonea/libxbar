/*
 *  XParser.h
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

#ifndef XPARSER_H_
#define XPARSER_H_

#include <string>
#include <vector>
#include <stack>

#include "XNode.h"

namespace XBar
{
  class XParserState;

  class XParser
  {
  public:
    XParser(const std::string &sInput);

    XNode* parse();
  private:
    std::stack<std::string>     m_vsInput;
    std::vector<XParserState*>  m_vStates;
  };

  extern XNode* parseLine(const std::string &sLine);
}

#endif /* XPARSER_H_ */
