//
//  main.c
//  kaydet
//
//  Created by Ahmet Zumberoglu on 19.12.2020.
//

#include <stdio.h>

int main(int argc, const char * argv[]) {
    // insert code here...
    FILE *file;
    file = fopen("result.txt","w");
    fprintf(file, "%s\n",argv[1]);
    printf("result.txt created.. \n");
    fclose(file);
    
    return 0;
}
