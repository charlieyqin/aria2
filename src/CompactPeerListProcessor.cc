/*
 * aria2 - The high speed download utility
 *
 * Copyright (C) 2006 Tatsuhiro Tsujikawa
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 */
/* copyright --> */
#include "CompactPeerListProcessor.h"
#include "Data.h"
#include <netinet/in.h>

bool CompactPeerListProcessor::canHandle(const MetaEntry* peersEntry) const {
  return dynamic_cast<const Data*>(peersEntry) != 0;
}

Peers CompactPeerListProcessor::extractPeer(const MetaEntry* peersEntry) {
  Peers peers;

  const Data* peersData = (const Data*)peersEntry;
  if(peersData->getLen() > 0) {
    for(int i = 0; i < peersData->getLen(); i += 6) {
      unsigned int ipaddr1 = (unsigned char)*(peersData->getData()+i);
      unsigned int ipaddr2 = (unsigned char)*(peersData->getData()+i+1);
      unsigned int ipaddr3 = (unsigned char)*(peersData->getData()+i+2);
      unsigned int ipaddr4 = (unsigned char)*(peersData->getData()+i+3);
      unsigned int port = ntohs(*(unsigned short int*)(peersData->getData()+i+4));
      char ipaddr[16];
      
      snprintf(ipaddr, sizeof(ipaddr), "%d.%d.%d.%d",
	       ipaddr1, ipaddr2, ipaddr3, ipaddr4);
      PeerHandle peer =
	PeerHandle(new Peer(ipaddr, port, pieceLength, totalLength));
      
      peers.push_back(peer);
    }
  }
  return peers;
}
