/*
 *  WordNet.h
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
 *
 *  The WordNet class provides access to the WordNet functions through a single
 *  access point.
 */

#ifndef WORDNET_H_
#define WORDNET_H_

#include <string>
#include <cstdint>

namespace XBar
{
  enum PartOfSpeech
  {
    GENERIC     = 0,
    NOUN        = 1,
    VERB        = 2,
    ADJECTIVE   = 3,
    ADVERB      = 4,
    PREPOSITION,
    DETERMINER,
  };

  /* Descriptor of part of speech which tells what parts of speech are
   * represented a word.
   *
   * Also, below, the meaning of all bits within the PartDescriptor
   */
  struct POSDescriptor
  {
    std::uint16_t bInflected : 1;
    std::uint16_t bNoun : 1;
    std::uint16_t bVerb : 1;
    std::uint16_t bAdjective : 1;
    std::uint16_t bAdverb : 1;
    std::uint16_t bPreposition : 1;
    std::uint16_t bDeterminer : 1;
  };

  class WordNet
  {
  public:
    static WordNet& getInstance();

    WordNet(const WordNet &)        = delete;
    void operator=(const WordNet &) = delete;

    POSDescriptor  searchPart(const std::string &sWord);
  private:
    WordNet();
  };
}

#endif /* WORDNET_H_ */
