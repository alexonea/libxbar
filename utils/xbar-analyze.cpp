/*
 *  xbar-analyze.cpp
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

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <sstream>
#include <algorithm>
#include <stack>

#include "XNode.h"

static
std::vector<std::string> s_vPrepositions
{
  "on",
  "in",
  "at",
  "since",
  "for",
  "ago",
  "before",
  "to",
  "past",
  "to",
  "till",
  "until",
  "by",
  "next to",
  "beside",
  "under",
  "below",
  "over",
  "above",
  "across",
  "through",
  "into",
  "towards",
  "onto",
  "from",
  "of",
  "off",
  "out of",
  "about",
};

static
std::vector<std::string> s_vDeterminers
{
  "the",
  "a",
  "an",
  "this",
  "that",
  "these",
  "those",
  "my",
  "your",
  "his",
  "her",
  "its",
  "our",
  "their",
  "a few",
  "a little",
  "much",
  "many",
  "a lot of",
  "most",
  "some",
  "any",
  "enough",
  "one",
  "ten",
  "thirty",
  "all",
  "both",
  "half",
  "either",
  "neither",
  "each",
  "every",
  "other",
  "another",
  "such",
  "what",
  "rather",
  "quite",
};

static
std::vector<std::string> s_vVerbs =
{
  "return",
};

int
main(int argc, const char *argv[])
{
  /*
   * Read a line from standard input
   */
  std::string sInput;
  std::getline(std::cin, sInput);

  /*
   * Split the input line into words (separated by spaces)
   */
  std::vector<std::string> vsWords;
  {
    std::istringstream iss{sInput};
    for (std::string sWord; iss >> sWord;)
    {
      vsWords.push_back(sWord);
    }
  }

  using namespace XBar;

  std::stack<XNode*> nodes;

  int begin = vsWords.size() - 1;
  int end   = 0;
  for (int idx = begin; idx >= end; --idx)
  {
    XNode *pCurrent = nullptr;

    std::string sWord{vsWords[idx]};
    std::transform(sWord.begin(), sWord.end(), sWord.begin(), ::tolower);

    /*
     * Look for determiners
     */
    auto det = std::find(s_vDeterminers.begin(), s_vDeterminers.end(), sWord);
    if (det != s_vDeterminers.end())
    {
      /*
      auto head = idx + 1;
      std::string sHead;

      while (head <= begin)
        sHead += vsWords[head++] + " ";

      if (sHead.size() > 0)
        sHead.erase(sHead.size() - 1);

      begin = idx;
      */

      std::string sHead{vsWords[idx + 1]};

      pCurrent = XNode::appendToOrCreate(NP);
      pCurrent->setSpecifier(sWord)->setHead(sHead);
      /* continue; */
    }

    /*
     * Look for prepositions
     */
    auto pp = std::find(s_vPrepositions.begin(), s_vPrepositions.end(), sWord);
    if (pp != s_vPrepositions.end())
    {
      pCurrent = XNode::appendToOrCreate(PP);
      pCurrent->setHead(sWord);
      /* continue; */
    }

    /*
     * Look for verbs
     */
    auto verb = std::find(s_vVerbs.begin(), s_vVerbs.end(), sWord);
    if (verb != s_vVerbs.end())
    {
      pCurrent = XNode::appendToOrCreate(VP);
      pCurrent->setHead(sWord);
      /* continue; */
    }

    /*
     * Check compatibility with previous nodes
     */
    if (pCurrent)
    {
      while (!nodes.empty())
      {
        XNode *pPrevious = nodes.top();
        XNode *pWinner = XNode::merge(pCurrent, pPrevious);
        if (!pWinner)
          break;

        pCurrent = pWinner;
        nodes.pop();
      }

      nodes.push(pCurrent);
    }
  }

  if (!nodes.empty())
    std::cout << "\\documentclass[margin=5mm]{standalone}\n"
              << "\\usepackage{tikz-qtree}\n"
              << "\\begin{document}\n"
              << "\\Tree " << nodes.top()->printToLatex() << "\n"
              << "\\end{document}\n";

  return 0;
}
