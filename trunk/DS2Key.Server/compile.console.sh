#!/bin/sh
gcc DS2Key/*.c -Dstricmp=strcasecmp -Dstrnicmp=strncasecmp -lXtst -o ds2key
