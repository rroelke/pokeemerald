#ifndef __DATA2JSON_TEXT_H__
#define __DATA2JSON_TEXT_H__


#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

EXTERN const char *growthRateToString(int rate);
EXTERN const char *eggGroupToString(int group);
EXTERN const char *bodyColorToString(int color);
EXTERN const char *backAnimToString(int anim);


#endif
