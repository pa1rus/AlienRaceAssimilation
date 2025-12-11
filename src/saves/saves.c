#include "saves.h"

bool SaveValue(float value)
{
    #if defined(PLATFORM_WEB)
        EM_ASM_({ localStorage.setItem("highscore", $0); }, value);
        return true;
    #else
        unsigned char *data = (unsigned char *)&value;
        unsigned int size = sizeof(float);
        return SaveFileData("save/save.data", data, size);
    #endif
}

float LoadValue()
{
    #if defined(PLATFORM_WEB)
        float result = EM_ASM_DOUBLE({
            let v = localStorage.getItem("highscore");
            if (!v) return 0;
            return parseFloat(v);
        });
        return result;
    #else
        int dataSize = 0;
        unsigned char *fileData = LoadFileData("save/save.data", &dataSize);
        if (fileData == NULL || dataSize < sizeof(float))
            return 0;

        float value = *((float *)fileData);
        UnloadFileData(fileData);
        return value;
    #endif
}
