#ifndef __CONFIG_H__
#define __CONFIG_H__

namespace D2K {
	namespace Config {
			//Loads settings from disk
			//@return (0) upon success, else (errno)
			int Load();

			//Saves settings to disk
			//@return (0) upon success, else (errno)
			int Save();
	}
}

#endif//__CONFIG_H__
