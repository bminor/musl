#undef FIOQSIZE
#define FIOQSIZE 0x545e

#undef SIOCGSTAMP
#undef SIOCGSTAMPNS
#define SIOCGSTAMP      _IOR(0x89, 6, char[16])
#define SIOCGSTAMPNS    _IOR(0x89, 7, char[16])
