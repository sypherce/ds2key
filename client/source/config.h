#pragma once

namespace D2K {namespace Config {

//Loads settings from disk
//@return (0) upon success, else (errno)
int Load();

//Saves settings to disk
//@return (0) upon success, else (errno)
int Save();

}}//D2K::Config
