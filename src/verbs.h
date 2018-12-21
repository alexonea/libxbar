/*
 *  verbs.h
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

#ifndef VERBS_H_
#define VERBS_H_

#include <string>

namespace XBar
{
  enum Tense
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

  struct TenseInfo
  {
    Tense eTense;
    bool  bProgressive;

    TenseInfo() : eTense(INVALID), bProgressive(false) {};
  };

  struct Verb
  {
    std::string sRoot;
    std::string sSuffix;
    std::string sPrefix;
    TenseInfo   tense;

    std::string toString() const;
  };

  extern Verb* deduceConjugation(const std::string &sWord);
  extern Verb* inflectVerb(Verb *pVerb, const std::string &sModifier);
}

#endif /* VERBS_H_ */
