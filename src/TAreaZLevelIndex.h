#ifndef MUDLET_TAREA_ZLEVEL_INDEX_H
#define MUDLET_TAREA_ZLEVEL_INDEX_H

/***************************************************************************
 *   Copyright (C) 2026 by Mudlet Makers                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QHash>
#include <QSet>

/*
 * Maintains an incrementally-updated reverse index of room IDs by Z level.
 *
 * This allows the 2D map renderer to iterate only the rooms on a given Z level
 * instead of scanning every room in the area and discarding those on other
 * levels. The index is a mirror of TArea::rooms keyed by Z coordinate.
 *
 * The class has no dependencies on TArea, TRoom, TMap, or any Qt widget
 * infrastructure, which makes it straightforward to unit test.
 */
class TAreaZLevelIndex
{
public:
    // Inserts room id at the given Z level.
    void addRoom(int id, int z);

    // Removes room id from the given Z level.
    // Empty Z-level sets are pruned so that isEmpty() remains accurate.
    void removeRoom(int id, int z);

    // Moves room id from fromZ to toZ. This is a no-op when fromZ == toZ.
    void moveRoom(int id, int fromZ, int toZ);

    // Replaces the entire index with data derived from a roomId-to-Z mapping.
    // Used by TArea::calcSpan() which already iterates all rooms.
    void rebuild(const QHash<int, int>& roomIdToZ);

    // Returns the set of room IDs on the given Z level.
    // Returns a reference to a persistent empty set for Z levels with no rooms,
    // so callers can always iterate the result safely.
    const QSet<int>& roomsForZ(int z) const;

    // Returns true when the index contains no rooms at all.
    bool isEmpty() const { return mIndex.isEmpty(); }

    // Removes all entries from the index.
    void clear() { mIndex.clear(); }

private:
    QHash<int, QSet<int>> mIndex;
    static const QSet<int> kEmptySet;
};

#endif // MUDLET_TAREA_ZLEVEL_INDEX_H
