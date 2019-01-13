#pragma once

// Branch prediction hints
#define CQL_LIKELY(expr) __builtin_expect(!!(expr), 1)
#define CQL_UNLIKELY(expr) __builtin_expect(!!(expr), 0)

// convenient macro to tell where is the line that includes this code
// since __func__, __FUNC__, __PRETTY_FUNCTION isn't macro so a helper function is required
#define CQL_CODEINFO cql::CodeInfo(cql::joinString("", "[", __FILE__, ":", __LINE__, ":", __PRETTY_FUNCTION__, "]"))

