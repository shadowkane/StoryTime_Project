#include <assert.h>
#include <stdio.h>

int main(){

    printf("++++ encryption_rsa.c Test ++++\n");
    assert(test_encryption_rsa()==0);
    printf("++++ math.c Test ++++\n");
    assert(test_math()==0);

    return 0;
}