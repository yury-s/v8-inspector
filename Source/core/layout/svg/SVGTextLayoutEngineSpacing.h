/*
 * Copyright (C) Research In Motion Limited 2010. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef SVGTextLayoutEngineSpacing_h
#define SVGTextLayoutEngineSpacing_h

#include "wtf/Noncopyable.h"
#include "wtf/unicode/Unicode.h"

namespace blink {

class Font;

// Helper class used by SVGTextLayoutEngine to handle 'letter-spacing' and 'word-spacing'.
class SVGTextLayoutEngineSpacing {
    WTF_MAKE_NONCOPYABLE(SVGTextLayoutEngineSpacing);
public:
    SVGTextLayoutEngineSpacing(const Font&, float effectiveZoom);

    float calculateCSSSpacing(UChar currentCharacter);

private:
    const Font& m_font;
    UChar m_lastCharacter;
    float m_effectiveZoom;
};

} // namespace blink

#endif
