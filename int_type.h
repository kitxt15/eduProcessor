#include <stdio.h>
#include <stdlib.h>
#include "../eduStack/stack.h"

void PrintInt(const void* ptr);
int VerifyInt(const void* ptr, const struct TypeInfo);

void IntInfoInit(struct TypeInfo *type);


//------------------------------------------------------------------------------------------------//

void PrintInt(const void* ptr){
    printf("%d", *(int*)ptr);
}

int VerifyInt(const void* ptr, const struct TypeInfo type){
    for (int i = 0; i < type.size; i++){
        if ((*((char*)ptr + i)) != type.poizonFillByte ){
            return -1;
        }
    }
    return 0;
}

void IntInfoInit(struct TypeInfo *type){
    type->size = sizeof(int);
    type->PrintElementOfType = PrintInt;
    type->VerifyType = VerifyInt;
    type->poizonFillByte = 0xDE;
    return;
}
