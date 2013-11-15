#ifndef __CONFIG_H__
#define __CONFIG_H__

namespace D2K {
	namespace Core {
		namespace C {
			class Config {
				public:
					Config();
					~Config();
					int Load();
					int Save();
			};
		}
		extern C::Config *Config;
	}
}

#endif//__CONFIG_H__
