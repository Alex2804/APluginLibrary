#include "../infomanager.h"

#include <ctype.h>

#undef ACUTILS_SYMBOL_ATTRIBUTES
#define ACUTILS_SYMBOL_ATTRIBUTES APLUGINSDK_NO_EXPORT
#include "../../libs/ACUtils/src/dynarray.c"

#ifdef __cplusplus
    #define PRIVATE_APLUGINSDK_STRUCT_NO_EXPORT APLUGINSDK_NO_EXPORT
#else
    #define PRIVATE_APLUGINSDK_STRUCT_NO_EXPORT
#endif

PRIVATE_APLUGINSDK_OPEN_PRIVATE_NAMESPACE

    A_DYNAMIC_ARRAY_DEFINITION(PRIVATE_APLUGINSDK_STRUCT_NO_EXPORT APluginSDKFeatureInfoDynArray, struct APLUGINLIBRARY_NAMESPACE APluginFeatureInfo*);
    A_DYNAMIC_ARRAY_DEFINITION(PRIVATE_APLUGINSDK_STRUCT_NO_EXPORT APluginSDKClassInfoDynArray, struct APLUGINLIBRARY_NAMESPACE APluginClassInfo*);
    A_DYNAMIC_ARRAY_DEFINITION(PRIVATE_APLUGINSDK_STRUCT_NO_EXPORT APluginSDKCString, char);

    static void _private_APluginSDK_destructInfoManager();
    static struct APLUGINLIBRARY_NAMESPACE APluginInfo* _private_APluginSDK_constructPluginInfo();
    static void _private_APluginSDK_destructPluginInfo(struct APLUGINLIBRARY_NAMESPACE APluginInfo*);

    static char** _private_APluginSDK_splitParameterList(const char *parameterList);

    struct PRIVATE_APLUGINSDK_STRUCT_NO_EXPORT _private_APluginSDK_InfoManager {
        struct APLUGINLIBRARY_NAMESPACE APluginInfo *pluginInfo;
        struct APluginSDKFeatureInfoDynArray *featureInfos;
        struct APluginSDKClassInfoDynArray *classInfos;

#ifdef __cplusplus
        ~_private_APluginSDK_InfoManager() {
            _private_APluginSDK_destructInfoManager();
        }
#endif
    };

    static struct _private_APluginSDK_InfoManager *_private_APluginSDK_infoManager = NULL;

    static void _private_APluginSDK_destructInfoManager()
    {
        size_t i;
        if(_private_APluginSDK_infoManager == NULL)
            return;
        _private_APluginSDK_destructPluginInfo(_private_APluginSDK_infoManager->pluginInfo);
        for(i = 0; i < aDynArraySize(_private_APluginSDK_infoManager->featureInfos); ++i) {
            struct APLUGINLIBRARY_NAMESPACE APluginFeatureInfo* featureInfo = aDynArrayGet(_private_APluginSDK_infoManager->featureInfos, i);
            free(featureInfo->parameterTypes);
            free(featureInfo->parameterNames);
            free(featureInfo);
        }
        aDynArrayDestruct(_private_APluginSDK_infoManager->featureInfos);
        for(i = 0; i < aDynArraySize(_private_APluginSDK_infoManager->classInfos); ++i)
            free(aDynArrayGet(_private_APluginSDK_infoManager->classInfos, i));
        aDynArrayDestruct(_private_APluginSDK_infoManager->classInfos);
#ifndef __cplusplus
        free(_private_APluginSDK_infoManager);
#endif
        _private_APluginSDK_infoManager = NULL;
    }
    static struct _private_APluginSDK_InfoManager* _private_APluginSDK_constructInfoManager()
    {
        if(_private_APluginSDK_infoManager == NULL) {
#ifdef __cplusplus
            static struct _private_APluginSDK_InfoManager infoManager;
            _private_APluginSDK_infoManager = &infoManager;
#else
            _private_APluginSDK_infoManager = (struct _private_APluginSDK_InfoManager*) malloc(sizeof(struct _private_APluginSDK_InfoManager));
#endif
            if(_private_APluginSDK_infoManager != NULL) {
                _private_APluginSDK_infoManager->pluginInfo = _private_APluginSDK_constructPluginInfo();
                _private_APluginSDK_infoManager->featureInfos = aDynArrayConstruct(struct APluginSDKFeatureInfoDynArray);
                _private_APluginSDK_infoManager->classInfos = aDynArrayConstruct(struct APluginSDKClassInfoDynArray);
                if(_private_APluginSDK_infoManager->pluginInfo == NULL
                   || _private_APluginSDK_infoManager->featureInfos == NULL
                   || _private_APluginSDK_infoManager->classInfos == NULL)
                {
                    _private_APluginSDK_destructInfoManager();
                }
            }
        }
        return _private_APluginSDK_infoManager;
    }

#if PRIVATE_APLUGINSDK_INTEGRATED_PLUGIN
    void* _private_APluginSDK_initAPluginFunctionPtr = NULL;
    bool _private_APluginSDK_registerInitAPluginFunction(void *functionPointer)
    {
        _private_APluginSDK_initAPluginFunctionPtr = functionPointer;
        return true;
    }

    void* _private_APluginSDK_finiAPluginFunctionPtr = NULL;
    bool _private_APluginSDK_registerFiniAPluginFunction(void *functionPointer)
    {
        _private_APluginSDK_finiAPluginFunctionPtr = functionPointer;
        return true;
    }
#endif

    struct APLUGINLIBRARY_NAMESPACE APluginInfo* _private_APluginSDK_getPluginInfo()
    {
        struct _private_APluginSDK_InfoManager* infoManager = _private_APluginSDK_constructInfoManager();
        if(infoManager == NULL)
            return NULL;
        return infoManager->pluginInfo;
    }

    bool _private_APluginSDK_setPluginName(const char *name)
    {
        struct _private_APluginSDK_InfoManager* infoManager = _private_APluginSDK_constructInfoManager();
        if(infoManager == NULL)
            return false;
        infoManager->pluginInfo->pluginName = name;
        return true;
    }
    bool _private_APluginSDK_setPluginVersion(size_t major, size_t minor, size_t patch)
    {
        struct _private_APluginSDK_InfoManager* infoManager = _private_APluginSDK_constructInfoManager();
        if(infoManager == NULL)
            return false;
        infoManager->pluginInfo->pluginVersionMajor = major;
        infoManager->pluginInfo->pluginVersionMinor = minor;
        infoManager->pluginInfo->pluginVersionPatch = patch;
        return true;
    }

    bool _private_APluginSDK_registerFeature(const char* featureGroup, const char* featureName, const char* returnType,
                                             const char* parameterList, void* functionPtr)
    {
        struct APLUGINLIBRARY_NAMESPACE APluginFeatureInfo* info;
        char **splittedParameterList;
        struct _private_APluginSDK_InfoManager* infoManager = _private_APluginSDK_constructInfoManager();
        if(infoManager == NULL)
            return false;
        info = (struct APLUGINLIBRARY_NAMESPACE APluginFeatureInfo*) malloc(sizeof(struct APLUGINLIBRARY_NAMESPACE APluginFeatureInfo));
        if(info != NULL) {
            info->pluginInfo = infoManager->pluginInfo;
            info->featureGroup = featureGroup;
            info->featureName = featureName;
            info->returnType = returnType;
            info->parameterList = parameterList;
            splittedParameterList = _private_APluginSDK_splitParameterList(parameterList);
            info->parameterTypes = splittedParameterList[0];
            info->parameterNames = splittedParameterList[1];
            free(splittedParameterList);
            info->functionPointer = functionPtr;
            aDynArrayAdd(infoManager->featureInfos, info);
        }
        return info != NULL;
    }
    bool _private_APluginSDK_registerClass(const char* interfaceClassName, const char* featureClassName,
                                           void* createInstance, void* deleteInstance)
    {
        struct APLUGINLIBRARY_NAMESPACE APluginClassInfo* info;
        struct _private_APluginSDK_InfoManager* infoManager = _private_APluginSDK_constructInfoManager();
        if(infoManager == NULL)
            return false;
        info = (struct APLUGINLIBRARY_NAMESPACE APluginClassInfo*) malloc(sizeof(struct APLUGINLIBRARY_NAMESPACE APluginFeatureInfo));
        if(info == NULL)
            return false;
        info->pluginInfo = infoManager->pluginInfo;
        info->interfaceName = interfaceClassName;
        info->className = featureClassName;
        info->createInstance = createInstance;
        info->deleteInstance = deleteInstance;
        aDynArrayAdd(infoManager->classInfos, info);
        return true;
    }


    static void* _private_APluginSDK_allocateMemory(size_t size)
    {
        return malloc(size);
    }
    static void _private_APluginSDK_freeMemory(void* ptr)
    {
        free(ptr);
    }

    static size_t _private_APluginSDK_getFeatureCount()
    {
        if(_private_APluginSDK_infoManager == NULL)
            return 0;
        return aDynArraySize(_private_APluginSDK_infoManager->featureInfos);
    }
    static const struct APLUGINLIBRARY_NAMESPACE APluginFeatureInfo* _private_APluginSDK_getFeatureInfo(size_t index)
    {
        if(_private_APluginSDK_infoManager == NULL || index >= aDynArraySize(_private_APluginSDK_infoManager->featureInfos))
            return NULL;
        return aDynArrayGet(_private_APluginSDK_infoManager->featureInfos, index);
    }
    static const struct APLUGINLIBRARY_NAMESPACE APluginFeatureInfo* const* _private_APluginSDK_getFeatureInfos()
    {
        if(_private_APluginSDK_infoManager == NULL)
            return NULL;
        return (const struct APLUGINLIBRARY_NAMESPACE APluginFeatureInfo* const*) _private_APluginSDK_infoManager->featureInfos->buffer;
    }

    static size_t _private_APluginSDK_getClassCount()
    {
        if(_private_APluginSDK_infoManager == NULL)
            return 0;
        return aDynArraySize(_private_APluginSDK_infoManager->classInfos);
    }
    static const struct APLUGINLIBRARY_NAMESPACE APluginClassInfo* _private_APluginSDK_getClassInfo(size_t index)
    {
        if(_private_APluginSDK_infoManager == NULL || index >= aDynArraySize(_private_APluginSDK_infoManager->classInfos))
            return NULL;
        return aDynArrayGet(_private_APluginSDK_infoManager->classInfos, index);
    }
    static const struct APLUGINLIBRARY_NAMESPACE APluginClassInfo* const* _private_APluginSDK_getClassInfos()
    {
        if(_private_APluginSDK_infoManager == NULL)
            return NULL;
        return (const struct APLUGINLIBRARY_NAMESPACE APluginClassInfo* const*) _private_APluginSDK_infoManager->classInfos->buffer;
    }

    static struct APLUGINLIBRARY_NAMESPACE APluginInfo* _private_APluginSDK_constructPluginInfo()
    {
        struct APLUGINLIBRARY_NAMESPACE APluginInfo* info = (struct APLUGINLIBRARY_NAMESPACE APluginInfo*) malloc(sizeof(struct APLUGINLIBRARY_NAMESPACE APluginInfo));
        info->apiVersionMajor = APLUGINLIBRARY_NAMESPACE A_PLUGIN_API_VERSION_MAJOR;
        info->apiVersionMinor = APLUGINLIBRARY_NAMESPACE A_PLUGIN_API_VERSION_MINOR;
        info->apiVersionPatch = APLUGINLIBRARY_NAMESPACE A_PLUGIN_API_VERSION_PATCH;
        info->pluginLanguage = PRIVATE_APLUGINSDK_PLUGIN_LANGUAGE;
        info->releasePlugin = _private_APluginSDK_destructInfoManager;
        info->allocateMemory = _private_APluginSDK_allocateMemory;
        info->freeMemory = _private_APluginSDK_freeMemory;
        info->getFeatureCount = _private_APluginSDK_getFeatureCount;
        info->getFeatureInfo = _private_APluginSDK_getFeatureInfo;
        info->getFeatureInfos = _private_APluginSDK_getFeatureInfos;
        info->getClassCount = _private_APluginSDK_getClassCount;
        info->getClassInfo = _private_APluginSDK_getClassInfo;
        info->getClassInfos = _private_APluginSDK_getClassInfos;
        info->pluginName = "";
        info->pluginVersionMajor = info->pluginVersionMinor = info->pluginVersionPatch = 0;
        return info;
    }
    static void _private_APluginSDK_destructPluginInfo(struct APLUGINLIBRARY_NAMESPACE APluginInfo* info)
    {
        free(info);
    }

/* ===================================== _private_APluginSDK_splitParameterList ===================================== */

    static char** _private_APluginSDK_splitParameterList(const char* parameterList)
    {
        struct APluginSDKCString *tmpParameterList, *typeString, *tmpString, *tmpTypesString, *tmpNamesString;
        size_t tmp, parameterListLength = strlen(parameterList);
        char** returnArray;
        tmpParameterList = aDynArrayConstruct(struct APluginSDKCString);
        typeString = aDynArrayConstruct(struct APluginSDKCString);
        tmpString = aDynArrayConstruct(struct APluginSDKCString);
        tmpTypesString = aDynArrayConstruct(struct APluginSDKCString);
        tmpNamesString = aDynArrayConstruct(struct APluginSDKCString);
        aDynArrayAddArray(tmpParameterList, parameterList, parameterListLength);
        bool typeFront = true, firstRun = true;
        char last = ' ';
        aDynArrayAdd(tmpParameterList, ','); /* terminate with ',' to flush last type and name */
        for(tmp = 0; tmp < parameterListLength + 1; ++tmp) {
            char current = aDynArrayGet(tmpParameterList, tmp);
            if(typeFront) {
                if(current != ',' && (isalnum(current) || ispunct(current))) {
                    aDynArrayAdd(typeString, current);
                } else if(isspace(current) && strcmp(typeString->buffer, "const") == 0) {
                    aDynArrayAdd(typeString, ' ');
                } else if(current == '*' || current == '&') {
                    aDynArrayAdd(typeString, current);
                    typeFront = false;
                } else if(isspace(current) && !isspace(last) && aDynArraySize(typeString) != 0) {
                    typeFront = false;
                }
            } else {
                if(current != ',' && (isalnum(current) || ispunct(current))) {
                    aDynArrayAdd(tmpString, current);
                } else if(current == '*' || current == '&') {
                    char typeStringLastChar = aDynArrayGet(typeString, aDynArraySize(typeString) - 1);
                    if(aDynArraySize(tmpString) != 0 && typeStringLastChar != '*' && typeStringLastChar != '&') {
                        aDynArrayAdd(typeString, ' ');
                    }
                    aDynArrayAddDynArray(typeString, tmpString);
                    aDynArrayAdd(typeString, current);
                    aDynArrayClear(tmpString);
                } else if((isspace(current) || current == ',') && strcmp(tmpString->buffer, "const") == 0) {
                    aDynArrayAddDynArray(typeString, tmpString);
                    aDynArrayClear(tmpString);
                }
                if(current == ',') {
                    if(!firstRun) {
                        aDynArrayAddArray(tmpTypesString, ", ", 2);
                        aDynArrayAddArray(tmpNamesString, ", ", 2);
                    }
                    aDynArrayAddDynArray(tmpTypesString, typeString);
                    aDynArrayAddDynArray(tmpNamesString, tmpString);
                    aDynArrayClear(typeString);
                    aDynArrayClear(tmpString);
                    typeFront = true;
                    firstRun = false;
                    current = ' '; /* 'last' should be ' ' */
                }
            }
            last = current;
        }
        tmp = aDynArraySize(tmpTypesString);
        char* types = (char*) malloc(sizeof(char) * (tmp + 1));
        memcpy(types, tmpTypesString->buffer, sizeof(char) * tmp);
        types[tmp] = '\0';

        tmp = aDynArraySize(tmpNamesString);
        char* names = (char*) malloc(sizeof(char) * (tmp + 1));
        memcpy(names, tmpNamesString->buffer, sizeof(char) * tmp);
        names[tmp] = '\0';

        aDynArrayDestruct(tmpParameterList);
        aDynArrayDestruct(typeString);
        aDynArrayDestruct(tmpString);
        aDynArrayDestruct(tmpTypesString);
        aDynArrayDestruct(tmpNamesString);

        returnArray = (char**) malloc(sizeof(char*) * 2);
        returnArray[0] = types;
        returnArray[1] = names;
        return returnArray;
    }

PRIVATE_APLUGINSDK_CLOSE_PRIVATE_NAMESPACE
