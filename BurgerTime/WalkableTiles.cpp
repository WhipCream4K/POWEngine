#include "WalkableTiles.h"

bool WalkableTiles::IsWalkable(int index) const
{
	if(m_TileData.contains(index))
	{
		return m_TileData.at(index).isWalkable;
	}

	return false;
}
