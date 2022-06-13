// Author: Philip Metzger
// SPDX-License-Identifer: Apache-2.0
#include "src/common/utilities.h"

#include <string>
#include <string_view>
#include <vector>

#include "absl/containers/flat_hash_map.h"
#include "absl/status/status.h"

#include "src/core/context.h"
#include "src/core/threadpool.h"
#include "src/core/metric.h"

namespace phab::commmon {
namespace {

static constexpr std::string_view g_default_page = R("
<html>
<head>
<meta author="$author">
<title>$title</title>
</head>
<body>
</body>
<footer> Phabricator internal Status Page, built on $creation_date </footer>
");


}


}
