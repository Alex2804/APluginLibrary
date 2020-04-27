#ifndef APLUGINSDK_MACROS_H
#define APLUGINSDK_MACROS_H

#include "exports.h"

#ifdef __cplusplus
#   include "cpp/macros.h"
#else
#   include "c/macros.h"
#endif

/* private plugin initialization function macro */
#ifndef PRIVATE_APLUGINSDK_INIT_FUNCTION
#   define PRIVATE_APLUGINSDK_INIT_FUNCTION void PRIVATE_APLUGINSDK_API_NAMESPACE initAPlugin()
#endif

/* private plugin finalization function macro*/
#ifndef PRIVATE_APLUGINSDK_FINI_FUNCTION
#   define PRIVATE_APLUGINSDK_FINI_FUNCTION void PRIVATE_APLUGINSDK_API_NAMESPACE finiAPlugin()
#endif

#endif /* APLUGINSDK_MACROS_H */
