gcc DS2Key/*.c -Dsocklen_t=int -Dstricmp=strcasecmp -Dstrnicmp=strncasecmp -lwsock32 -o ds2key.exe
