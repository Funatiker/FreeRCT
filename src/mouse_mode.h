/*
 * This file is part of FreeRCT.
 * FreeRCT is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * FreeRCT is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with FreeRCT. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file mouse_mode.h Mouse mode handling. */

#ifndef MOUSE_MODE_H
#define MOUSE_MODE_H

#include "geometry.h"

/**
 * Available cursor types.
 * @ingroup viewport_group
 */
enum CursorType {
	CUR_TYPE_NORTH,    ///< Show a N corner highlight.
	CUR_TYPE_EAST,     ///< Show a E corner highlight.
	CUR_TYPE_SOUTH,    ///< Show a S corner highlight.
	CUR_TYPE_WEST,     ///< Show a W corner highlight.
	CUR_TYPE_TILE,     ///< Show a tile highlight.
	CUR_TYPE_ARROW_NE, ///< Show a build arrow in the NE direction.
	CUR_TYPE_ARROW_SE, ///< Show a build arrow in the SE direction.
	CUR_TYPE_ARROW_SW, ///< Show a build arrow in the SW direction.
	CUR_TYPE_ARROW_NW, ///< Show a build arrow in the NW direction.
	CUR_TYPE_EDGE_NE,  ///< Show a NE edge sprite highlight.
	CUR_TYPE_EDGE_SE,  ///< Show a SE edge sprite highlight.
	CUR_TYPE_EDGE_SW,  ///< Show a SW edge sprite highlight.
	CUR_TYPE_EDGE_NW,  ///< Show a NW edge sprite highlight.

	CUR_TYPE_INVALID = 0xFF, ///< Invalid/unused cursor.
};

static const uint32 INVALID_TILE_INDEX = 0xFFFFFFFFu; ///< Invalid selector tile index.

/** Base class for displaying and handling mouse modes from a window. */
class MouseModeSelector {
public:
	MouseModeSelector();
	MouseModeSelector(CursorType cur_cursor);
	virtual ~MouseModeSelector();

	virtual void MarkDirty() = 0;
	virtual CursorType GetCursor(const XYZPoint16 &voxel_pos) = 0;

	/**
	 * Get the offset of the tile position in the area. Parameters are unchecked.
	 * @param x Horizontal relative offset in the area.
	 * @param y Vertical relative offset in the area.
	 * @return Index in the tile data.
	 */
	inline uint32 GetTileOffset(int rel_x, int rel_y) const
	{
		return rel_x * this->area.height + rel_y;
	}

	/**
	 * Get the index of the tile position in the area.
	 * @param x Horizontal coordinate of the absolute world position.
	 * @param y Vertical coordinate of the absolute world position.
	 * @return Index in the tile data, or #INVALID_TILE_INDEX if out of bounds.
	 */
	inline uint32 GetTileIndex(int x, int y) const
	{
		x -= this->area.base.x;
		if (x < 0 || x >= this->area.width) return INVALID_TILE_INDEX;
		y -= this->area.base.y;
		if (y < 0 || y >= this->area.height) return INVALID_TILE_INDEX;
		return GetTileOffset(x, y);
	}

	/**
	 * Get the index of the tile position in the area.
	 * @param pos Absolute world position.
	 * @return Index in the tile data, or #INVALID_TILE_INDEX if out of bounds.
	 */
	inline uint32 GetTileIndex(const Point16 &pos) const
	{
		return this->GetTileIndex(pos.x, pos.y);
	}

	/**
	 * Rough estimate whether the selector wants to render something in the voxel stack at the given coordinate.
	 * @param x X position of the stack.
	 * @param y Y position of the stack.
	 * @return Whether the selector wants to contribute to the graphics in the given stack.
	 */
	inline bool IsInsideArea(int x, int y) const
	{
		return this->GetTileIndex(x, y) != INVALID_TILE_INDEX;
	}

	Rectangle16 area;      ///< Position and size of the selected area (over-approximation of voxel stacks).
	CursorType cur_cursor; ///< Cursor to return at the #GetCursor call.
};

#endif
