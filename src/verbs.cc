/*
 *  verbs.cc
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

#include "verbs.h"

#include <vector>
#include <sstream>
#include <utility>

#include <iostream>

namespace XBar
{
  static
  std::vector<std::string> s_vVerbs =
  {
    "return",
  };

  static
  std::vector<std::pair<std::string, std::string>> s_vModals =
  {
    { "can", "could"},
    { "may", "might"},
    { "will", "would"},
    { "must", "must"},
    { "shall", "should"},
    { "ought to", "ought to"},
  };

  std::string
  Verb::toString() const
  {
    std::stringstream ss;
    ss << "[ " << sRoot;
    if (sSuffix.size() > 0)
      ss << " + " << sSuffix;

    ss << " ]";

    return ss.str();
  }

  Verb*
  inflectVerb(Verb *pVerb, const std::string &sModifier)
  {
    if (!pVerb)
      return nullptr;

    if (sModifier == "been")
    {
      if (!pVerb->tense.bProgressive)
        throw std::runtime_error("Bad tense");

      if (pVerb->sPrefix.size() == 0)
        pVerb->sPrefix = sModifier;
      else
        pVerb->sPrefix = sModifier + " " + pVerb->sPrefix;

      return pVerb;
    }

    if (sModifier == "have")
    {
      if (pVerb->tense.bProgressive)
      {
        if (pVerb->sPrefix.find("been") == std::string::npos)
          throw std::runtime_error("Bad tense");


      }
    }

    return pVerb;
  }

  Verb*
  deduceConjugation(const std::string &sWord)
  {
    for (const auto & sBaseline : s_vVerbs)
    {
      /*
       * Look for regular verbs
       * present tense
       */
      if (sWord == sBaseline + "s" ||
          sWord == sBaseline + "es" ||
          sWord == sBaseline)
      {
        Verb *pV = new Verb{};
        auto end = sBaseline.size();
        pV->sRoot = sBaseline;
        pV->sSuffix = sWord.substr(end);
        pV->tense.eTense = PRESENT;
        pV->tense.bProgressive = false;

        return pV;
      }

      if (sWord == sBaseline + "ed" ||
          sWord == sBaseline + "d" ||
          sWord == sBaseline + "ded" ||
          sWord == sBaseline + "red")
      {
        Verb *pV = new Verb{};

        auto end = sBaseline.size();
        pV->sRoot = sBaseline;
        pV->sSuffix = sWord.substr(end);
        pV->tense.eTense = PAST;
        pV->tense.bProgressive = false;

        return pV;
      }

      if (sWord == sBaseline + "ing" ||
          sWord == sBaseline + "ring" ||
          sWord == sBaseline + "ding")
      {
        Verb *pV = new Verb{};

        auto end = sBaseline.size();
        pV->sRoot = sBaseline;
        pV->sSuffix = sWord.substr(end);
        pV->tense.eTense = PRESENT;
        pV->tense.bProgressive = true;

        return pV;
      }
    }

    return nullptr;
  }
}
