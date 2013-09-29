/*
DS2Key Client - An application to use your DS as a PC Gamepad
Copyright (C) 2008  Derrick (sypherce) Wirth

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __CONFIG_H__
#define __CONFIG_H__

//normal defines
#define LENGTH_IP 15
#define LENGTH_PORT 5
#define LENGTH_PROFILE 3
#define DEFAULT_IP "0.0.0.0"
#define DEFAULT_PORT "9501"
#define DEFAULT_PROFILE "0"
#define DS2KEY_INI "ds2key.ini"

//function defines
#define readString() \
    if(tmpBuffer[0] != 0) \
    { \
        int i = 0; \
        getLine(tmpBuffer);	\
        tmpString = tmpBuffer; \
        tmpBuffer = tmpBuffer + strlen(tmpBuffer) + 1; \
        while(tmpBuffer[i] == (char)0xa || tmpBuffer[i] == (char)0xd) \
        { \
            i++; \
        } \
    \
        tmpBuffer = &tmpBuffer[i]; \
    }

//variables
extern int ip;
extern int port;
extern int profile;

//functions
extern bool writeConfig();
extern bool writeDefaultConfig();
extern bool getLine(char *buffer);
extern bool readConfig();

#endif //__CONFIG_H__
