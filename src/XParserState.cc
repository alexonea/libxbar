/*
 *  XParserState.cc
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

#include "XParserState.h"

namespace XBar
{
  XParserState::XParserState(const XParserState &other)
  {
    for(XNode* node : other.nodes)
      nodes.push_back(node->clone());
  }

  XParserState::XParserState(XParserState &&other)
  : nodes(std::move(other.nodes))
  {}

  XParserState&
  XParserState::operator=(const XParserState &other)
  {
    nodes.clear();

    for(XNode* node : other.nodes)
      nodes.push_back(node->clone());

    return *this;
  }

  XParserState&
  XParserState::operator=(XParserState &&other)
  {
    std::swap(nodes, other.nodes);
    return *this;
  }
}
