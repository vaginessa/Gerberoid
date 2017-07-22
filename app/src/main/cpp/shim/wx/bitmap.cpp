/*
 * This file is part of the Gerberoid project.
 *
 * Copyright (C) 2017 Marcus Comstedt <marcus@mc.pp.se>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <wx/wx.h>

wxMask::wxMask(const wxBitmap& bitmap, const wxColour& colour)
{
  /* Stub */
}

wxBitmap::wxBitmap(int width, int height)
  : android::Bitmap(android::Bitmap::createBitmap(width, height, android::Bitmap::Config::ARGB_8888))
{
}

void wxBitmap::SetMask(wxMask *mask)
{
  /* Stub */
}

wxBitmap wxNullBitmap(0, 0);

