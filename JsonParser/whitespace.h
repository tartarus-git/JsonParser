#pragma once

// Filter out character if it is whitespace.
inline bool filterWhitespace(char character) { return character == ' ' || character == '\t' || character == '\n' || character == '\r'; }