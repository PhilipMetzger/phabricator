// Author: Philip Metzger
// SPDX-License-Identifier: Apache-2.0
#ifndef SRC_NATIVE_CORE_CHECK_H_
#define SRC_NATIVE_CORE_CHECK_H_

namespace phab::core {

#if defined(NDEBUG)
#define DCHECK(condition) 
#else // !defined(NDEBUG)
#define DCHECK(condition) 
#define DCHECK_EQ(exp,cond)  \
    DCHECK_OPERATOR(expected,condition,=,#exp ##"==" #cond)
#define DCHECK_LT(exp,cond) \
    DCHECK_OPERATOR(exp,condition,<=,#exp ##"<="## #cond) 
#define DCHECK_GT(exp,cond) \
    DCHECK_OPERATOR(exp,condition,>=,#exp ##">="## #cond)
#define DCHECK_NE(exp,cond) \
    DCHECK_OPERATOR(exp,condition,!=,#exp ##"!="## #cond)
#endif
}
#endif
