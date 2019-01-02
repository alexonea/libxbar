/*
 *  WordNet.cc
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

#include "WordNet.h"

#include <wn.h>

namespace XBar
{
  WordNet::WordNet()
  {
    wninit();
  }

  WordNet&
  WordNet::getInstance()
  {
    static WordNet _wnInstance;
    return _wnInstance;
  }

  POSDescriptor
  WordNet::searchPart(const std::string &sWord)
  {
    POSDescriptor pd = { 0 };
    char *czBase = nullptr;

    /*
     * [01.01.2019] Ugly hacks, must fix. in_wn demands a non-const pointer
     * Also, czBase holds a pointer to a statically allocated buffer maintained
     * by WordNet, according to their documentation. No need to cleanup.
     */
    czBase = morphstr((char *) sWord.c_str(), VERB);
    if (czBase)
      pd.bVerb = pd.bInflected = 1;

    czBase = morphstr((char *) sWord.c_str(), NOUN);
    if (czBase)
      pd.bNoun = pd.bInflected = 1;

    czBase = morphstr((char *) sWord.c_str(), ADV);
    if (czBase)
      pd.bAdverb = pd.bInflected = 1;

    czBase = morphstr((char *) sWord.c_str(), ADJ);
    if (czBase)
      pd.bAdjective = pd.bInflected = 1;


    unsigned int iRes = in_wn((char *) sWord.c_str(), ALL_POS);
    if (iRes & (1 << VERB))
      pd.bVerb = 1;

    if (iRes & (1 << NOUN))
      pd.bNoun = 1;

    if (iRes & (1 << ADJ))
      pd.bAdjective = 1;

    if (iRes & (1 << ADV))
      pd.bAdverb = 1;

    return pd;
  }
}

