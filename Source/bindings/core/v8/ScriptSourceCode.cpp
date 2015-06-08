// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "config.h"
#include "bindings/core/v8/ScriptSourceCode.h"

namespace blink {

ScriptSourceCode::ScriptSourceCode()
    : m_startPosition(TextPosition::minimumPosition())
{
}

ScriptSourceCode::ScriptSourceCode(const String& source, const KURL& url, const TextPosition& startPosition)
    : m_source(source)
    , m_url(url)
    , m_startPosition(startPosition)
{
    treatNullSourceAsEmpty();
    if (!m_url.isEmpty())
        m_url.removeFragmentIdentifier();
}


ScriptSourceCode::~ScriptSourceCode()
{
}

const KURL& ScriptSourceCode::url() const
{
    return m_url;
}

String ScriptSourceCode::sourceMapUrl() const
{
    return String();
}

void ScriptSourceCode::treatNullSourceAsEmpty()
{
    // ScriptSourceCode allows for the representation of the null/not-there-really ScriptSourceCode value.
    // Encoded by way of a m_source.isNull() being true, with the nullary constructor to be used to
    // construct such a value.
    //
    // Should the other constructors be passed a null string, that is interpreted as representing
    // the empty script. Consequently, we need to disambiguate between such null string occurrences.
    // Do that by converting the latter case's null strings into empty ones.
    if (m_source.isNull())
        m_source = "";
}

} // namespace blink
