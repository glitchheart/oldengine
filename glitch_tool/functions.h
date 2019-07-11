#ifndef FUNCTIONS
#define FUNCTIONS
#include "types.h"

#define CREATEPROJECT(name)void name(char* Name)
typedef CREATEPROJECT(createproject);

#define INSTALLGLITCH(name)void name()
typedef INSTALLGLITCH(installglitch);

#define BUILDGLITCH(name)void name(b32 WithEngine)
typedef BUILDGLITCH(buildglitch);

#define RUNGLITCH(name)void name(b32 WithEngine)
typedef RUNGLITCH(runglitch);

#define RELEASEGAME(name) void name(b32 WithZip)
typedef RELEASEGAME(releasegame);

#endif