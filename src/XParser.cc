/*
 *  XParser.cc
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

#include "XParser.h"

#include <vector>
#include <stack>
#include <algorithm>
#include <sstream>
#include <utility>

#include "WordNet.h"
#include "XNoun.h"
#include "XVerb.h"

#include "XParserState.h"

#include <iostream>

namespace XBar
{
  XParser::XParser(const std::string &sInput)
  {
    std::istringstream iss{sInput};
    for (std::string sWord; iss >> sWord;)
    {
      m_vsInput.push(sWord);
    }

    m_vStates.push_back(new XParserState{});
  }

  XNode*
  XParser::parse()
  {
    while (!m_vsInput.empty())
    {
      std::string sWord{m_vsInput.top()};
      m_vsInput.pop();

      std::transform(sWord.begin(), sWord.end(), sWord.begin(), ::tolower);

      POSDescriptor pos = WordNet::getInstance().searchPart(sWord);
      std::vector<XParserState*> vNewStates;

      std::cerr << "[info] processing \"" << sWord << "\" as:";

      if (pos.bNoun)
      {
        std::cerr << " noun";
        for (XParserState* state : m_vStates)
        {
          if (state->nodes.empty() ||
              state->nodes.front()->getType() == VP ||
              state->nodes.front()->getType() == PP)
          {
            XParserState *newState = new XParserState{*state};
//            XNoun *noun = new XNoun{sWord};
            XNode *node = XNode::appendToOrCreate(NP);
            node->setHead(sWord);

            newState->nodes.push_front(node);
            vNewStates.push_back(newState);
          }
        }
      }

      if (pos.bVerb)
      {
        std::cerr << " verb";
        for (XParserState* state : m_vStates)
        {
          if (state->nodes.empty() ||
              state->nodes.front()->getType() == NP ||
              state->nodes.front()->getType() == PP)
          {
            XParserState *newState = new XParserState{*state};
            XNode *node = XNode::appendToOrCreate(VP);
            node->setHead(sWord);

            newState->nodes.push_front(node);
            vNewStates.push_back(newState);
          }
        }
      }

      if (pos.bAdjective)
      {
        std::cerr << " adj";
        for (XParserState* state : m_vStates)
        {
          if (!state->nodes.empty() &&
              state->nodes.front()->getType() == NP)
          {
            XParserState *newState = new XParserState{*state};
            XNode *node = newState->nodes.front();
            if (XNoun *noun = dynamic_cast<XNoun*> (node->getValue()))
              noun->addAttribute(sWord);

            vNewStates.push_back(newState);
          }
        }
      }

      if (pos.bAdverb)
      {
        std::cerr << " adv";
        for (XParserState* state : m_vStates)
        {
          if (!state->nodes.empty() &&
              state->nodes.front()->getType() == VP)
          {
            XParserState *newState = new XParserState{*state};
            XNode *node = newState->nodes.front();
            if (XVerb *verb = dynamic_cast<XVerb*> (node->getValue()))
              verb->addAdverb(sWord);

            vNewStates.push_back(newState);
          }
        }
      }

      if (pos.bDeterminer)
      {
        std::cerr << " det";
        for (XParserState* state : m_vStates)
        {
          if (!state->nodes.empty() &&
              state->nodes.front()->getType() == NP)
          {
            XParserState *newState = new XParserState{*state};
            XNode *node = newState->nodes.front();
            node->setSpecifier(sWord);
            vNewStates.push_back(newState);
          }
        }
      }

      if (pos.bPreposition)
      {
        std::cerr << " prep";
        for (XParserState* state : m_vStates)
        {
          if (!state->nodes.empty() &&
              state->nodes.front()->getType() == NP)
          {
            XParserState *newState = new XParserState{*state};
            XNode *node = XNode::appendToOrCreate(PP);
            node->setHead(sWord);

            newState->nodes.push_front(node);
            vNewStates.push_back(newState);
          }
        }
      }

      std::cerr << "\n";

      /*
       * Ready for next run
       */
      m_vStates.swap(vNewStates);

      std::cerr << "[info] current parse states:\n";
      unsigned int idx = 0;
      for (XParserState* pSt : m_vStates)
      {
        std::cerr << "[info]\t" << idx << "\t:";
        for (XNode* pN : pSt->nodes)
          std::cerr << " " << pN->getType();
        std::cerr << "\n";
      }
    }

    if (m_vStates.empty())
      return nullptr;

    std::vector<XParserState*> vSolution;

    for (XParserState* state : m_vStates)
    {
      if (state->nodes.empty())
        continue;

      bool bDone = false;
      while (!bDone)
      {
        auto pFirst = state->nodes.rbegin();
        bool bModified = false;
        auto pNext = std::next(pFirst, 1);

        while (pNext != state->nodes.rend())
        {
          std::cerr << "[info] merging now...\n";
          std::cerr << "[info]\t\t" << (*pFirst)->getType() << std::endl;
          std::cerr << "[info]\t\t" << (*pNext)->getType() << std::endl;
          XNode *pWinner = XNode::merge(*pNext, *pFirst);
          if (!pWinner)
          {
            std::cerr << "[info]\t\tNOT OK, continue..." << std::endl;
            pFirst = pNext++;
            continue;
          }

          bModified = true;

          if (pWinner == *pFirst)
          {
            state->nodes.erase(std::next(pNext++).base());
          }
          else if (pWinner == *pNext)
          {
            state->nodes.erase(std::next(pFirst).base());
            if (std::prev(pFirst) != state->nodes.rend())
            {
              pFirst = std::prev(pFirst);
            }
            else
              ++pNext;
          }
          else
          {
            *pFirst = pWinner;
            state->nodes.erase(std::next(pNext).base());
          }

          std::cerr << "[info]\t\tWINNER: " << pWinner->getType() << std::endl;
          std::cerr << "[info]\t\tState after merge:";
          for (XNode* pN : state->nodes)
            std::cerr << " " << pN->getType();
          std::cerr << "\n" << std::endl;
        }

        if (!bModified || state->nodes.size() == 1)
          bDone = true;
      }

      if (state->nodes.size() == 1)
        vSolution.push_back(state);
    }

    if (vSolution.empty())
      return nullptr;

    std::cerr << "---> SOLUTION <---" << std::endl << std::endl;
    for (XParserState *state : vSolution)
    {
      std::cerr << "Partial:";
      for (XNode* pN : state->nodes)
        std::cerr << " " << pN->getType();
      std::cerr << "\n" << std::endl;
    }

    return vSolution[0]->nodes.front();
  }

  XNode*
  parseLine(const std::string &sLine)
  {
    XParser parser{sLine};
    return parser.parse();
  }
}
