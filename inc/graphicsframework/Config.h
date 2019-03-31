#ifndef GF_CONFIG_H_
#define GF_CONFIG_H_

// platform specific configuration
#ifdef WIN32
	#ifdef GF_SHARED
		#ifdef GF_SHARED_EXPORT
			#define GF_DLL _declspec(dllexport)
		#else
			#define GF_DLL _declspec(dllimport)
		#endif
	#else
		#define GF_DLL
	#endif
#endif

#endif