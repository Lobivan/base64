#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int b64_encode_f(int N, char *infilename, char *outfilename) {
    FILE *inp_file,
            *outp_file;
    char signs[] = "ABCDEFGHIJKLMNOPQRSTUVWSYZabcdefghijklmnopqrstuvwsyz0123456789+/";
    char input[1000],
            *pointer = input;
    int three_bytes = 0,
            i = 0,
            inp_len = 0;;
    if ((inp_file = fopen(infilename, "rb")) == NULL) {
        printf("cannot open inp_file");
        return 1;
    }
    if ((outp_file = fopen(outfilename, "w")) == NULL) {
        printf("cannot open outp_file");
        return 1;
    }
    inp_len = fread(input, 1, 100, inp_file);
    printf("%d", inp_len);

    int k,
        c = 0;
    while (i < (inp_len / 3)) {
        three_bytes = (*pointer << 16) + (*(pointer + 1) << 8) + *(pointer + 2);
        k = 3;
        while (k >= 0){
            fputc(signs[(three_bytes >> (6 * k)) & 0x3F], outp_file);
            c++;
            if(c == N){
                fputc('\n', outp_file);
                c = 0;
            }
            k--;
        }
        /*fputc(signs[three_bytes >> 18], outp_file);
        fputc(signs[(three_bytes >> 12) & 0x3F], outp_file);
        fputc(signs[(three_bytes >> 6) & 0x3F], outp_file);
        fputc(signs[three_bytes & 0x3F], outp_file);*/
        i++;
        pointer = pointer + 3;
    }

    if ((inp_len % 3) != 0) {
        three_bytes = (*pointer << 16);
        if ((inp_len % 3) == 2) {
            k = 3;
            while (k > 0){
                fputc(signs[(three_bytes >> (6 * k))& 0x3F], outp_file);
                if(c++ == N){
                    fputc('\n', outp_file);
                    c = 0;
                }
                k--;
            }
            /*three_bytes = three_bytes + (*(pointer + 1) << 8);
            fputc(signs[three_bytes >> 18], outp_file);
            fputc(signs[(three_bytes >> 12) & 0x3F], outp_file);
            fputc(signs[(three_bytes >> 6) & 0x3F], outp_file);*/
            fputc('=', outp_file);
            if(c++ == N)
                fputc('\n', outp_file);
            fputc('\0', outp_file);
        }
        else {
            fputc(signs[three_bytes >> 18], outp_file);
            if(c++ == N){
                fputc('\n', outp_file);
                c = 0;
            }
            fputc(signs[(three_bytes >> 12) & 0x3F], outp_file);
            if(c++ == N){
                fputc('\n', outp_file);
                c = 0;
            }
            fputc('=', outp_file);
            if(c++ == N){
                fputc('\n', outp_file);
                c = 0;
            }
            fputc('=', outp_file);
            if(c++ == N)
                fputc('\n', outp_file);
            fputc('\0', outp_file);
        }
    }

    fclose(inp_file);
    fclose(outp_file);
    return 0;
}

int b64_encode(char *infilename, char *outfilename) {
    FILE *inp_file,
            *outp_file;
    char signs[] = "ABCDEFGHIJKLMNOPQRSTUVWSYZabcdefghijklmnopqrstuvwsyz0123456789+/";
    char input[1000],
            *pointer = input;
    int three_bytes = 0,
            i = 0,
            inp_len = 0;;
    if ((inp_file = fopen(infilename, "rb")) == NULL) {
        printf("cannot open inp_file");
        return 1;
    }
    if ((outp_file = fopen(outfilename, "w")) == NULL) {
        printf("cannot open outp_file");
        return 1;
    }
    inp_len = fread(input, 1, 100, inp_file);


    while (i < (inp_len / 3)) {
        three_bytes = (*pointer << 16) + (*(pointer + 1) << 8) + *(pointer + 2);
        fputc(signs[three_bytes >> 18], outp_file);
        fputc(signs[(three_bytes >> 12) & 0x3F], outp_file);
        fputc(signs[(three_bytes >> 6) & 0x3F], outp_file);
        fputc(signs[three_bytes & 0x3F], outp_file);
        i++;
        pointer = pointer + 3;
    }

    if ((inp_len % 3) != 0) {
        three_bytes = (*pointer << 16);
        if ((inp_len % 3) == 2) {
            three_bytes = three_bytes + (*(pointer + 1) << 8);
            fputc(signs[three_bytes >> 18], outp_file);
            fputc(signs[(three_bytes >> 12) & 0x3F], outp_file);
            fputc(signs[(three_bytes >> 6) & 0x3F], outp_file);
            fputc('=', outp_file);
            fputc('\0', outp_file);
        }
        else {
            fputc(signs[three_bytes >> 18], outp_file);
            fputc(signs[(three_bytes >> 12) & 0x3F], outp_file);
            fputc('=', outp_file);
            fputc('=', outp_file);
            fputc('\0', outp_file);
        }
    }

    fclose(inp_file);
    fclose(outp_file);
    return 0;
}

int b64_decode_i(char *infilename, char *outfilename){
    FILE *inp_file,
            *outp_file;
    char signs[] = "ABCDEFGHIJKLMNOPQRSTUVWSYZabcdefghijklmnopqrstuvwsyz0123456789+/";
    char input[1000];
    int inp_len = 0;;
    if ((inp_file = fopen(infilename, "rb")) == NULL) {
        printf("cannot open inp_file");
        return 1;
    }
    if ((outp_file = fopen(outfilename, "w")) == NULL) {
        printf("cannot open outp_file");
        return 1;
    }
    inp_len = fread(input, 1, 100, inp_file);
    int j = 0,
            k = 0,
            q = 0,
            four = 0,
            ignored = 0;
    while (j < (inp_len / 4 - 1)) {
        k = 0;
        while (k < 4) {
            q = 0;
            while (q < 64) {
                if (input[j * 4 + k + ignored] == signs[q]) {
                    four = four + (q << 6 * (3 - k));
                    break;
                }
                q++;
            }
            if (q == 64){
                ignored++;
                k--;
            }
            k++;
        }
        k = 0;
        while (k < 3) {
            fputc((four >> 8 * (2 - k)) & 0xFF, outp_file);
            k++;
        }

        j++;
    }
    four = 0;
    k = 0;
    int last = 4;
    while (k < 4) {
        q = 0;
        if (input[j * 4 + k + ignored] == '=') {
            last = k - 1;
            break;
        }
        while (q < 64) {
            if (input[j * 4 + k + ignored] == signs[q]) {
                four = four + (q << 6 * (3 - k));
                break;
            }
            q++;
        }
        if (q == 64){
            ignored++;
            k--;
        }
        k++;
    }
    k = 0;
    while ((k < 3) && (k != last)) {
        fputc((four >> 8 * (2 - k)) & 0xFF, outp_file);
        k++;
    }
    fclose(inp_file);
    fclose(outp_file);
    return 0;
}

int b64_decode(char *infilename, char *outfilename){
    FILE *inp_file,
            *outp_file;
    char signs[] = "ABCDEFGHIJKLMNOPQRSTUVWSYZabcdefghijklmnopqrstuvwsyz0123456789+/";
    char input[1000];
    int inp_len = 0;;
    if ((inp_file = fopen(infilename, "rb")) == NULL) {
        printf("cannot open inp_file");
        return 1;
    }
    if ((outp_file = fopen(outfilename, "w")) == NULL) {
        printf("cannot open outp_file");
        return 1;
    }
    inp_len = fread(input, 1, 100, inp_file);
    int j = 0,
            k = 0,
            q = 0,
            four = 0;
    while (j < (inp_len / 4 - 1)) {
        k = 0;
        while (k < 4) {
            q = 0;
            while (q < 64) {
                if (input[j * 4 + k] == signs[q]) {
                    four = four + (q << 6 * (3 - k));
                    break;
                }
                q++;
            }
            if (q == 64){
                printf("ERROR: incorrect input");
                return 1;
            }
            k++;
        }
        k = 0;
        while (k < 3) {
            fputc((four >> 8 * (2 - k)) & 0xFF, outp_file);
            k++;
        }

        j++;
    }
    four = 0;
    k = 0;
    int last = 4;
    while (k < 4) {
        q = 0;
        if (input[j * 4 + k] == '=') {
            last = k - 1;
            break;
        }
        while (q < 64) {
            if (input[j * 4 + k] == signs[q]) {
                four = four + (q << 6 * (3 - k));
                break;
            }
            q++;
        }
        if (q == 64){
            printf("ERROR: incorrect input");
            return 1;
        }
        k++;
    }
    k = 0;
    while ((k < 3) && (k != last)) {
        fputc((four >> 8 * (2 - k)) & 0xFF, outp_file);
        k++;
    }
    fclose(inp_file);
    fclose(outp_file);
    return 0;
}

int main(int argc, char *argv[]) {
    //b64_encode(argv[2], argv[3]);
    char help[] = "help:\n"
                  "    Usage:\n"
                  "        base64coder.exe -e infilename outfilename\n"
                  //"закодировать произвольный файл infilename в Base64-encoded текстовый файл outfilename\n"
                  "---------------------------------\n"
                  "        base64coder.exe -d infilename outfilename\n"
                  //"декодировать Base64-encoded текстовый файл infilename в бинарный outfilename\n"
                  //"Если при декодировании встретились символы не из Base64-алфавита, то выдать сообщение об ошибке\n"
                  "---------------------------------\n"
                  "        base64coder.exe -i -d infilename outfilename\n"
                  // "декодировать Base64-encoded текстовый файл infilename в бинарный outfilename\n"
                  // "Если есть опция -i, то при декодировании игнорировать символы не из Base64-алфавита\n"
                  "--------------------------------\n"
                  "        base64coder.exe -e -f 80 infilename outfilename\n";
                  // "Закодировать произвольный файл infilename в Base64-encoded текстовый файл outfilename\n"
                  // "Если передана опция -f N, то в выходном файле через каждые N символов вставить перевод строки\n"
    if ((argc < 4) || (argv[1][0] != '-')){
        printf("%s", help);
        return 1;
    }
    int N,
        res;
    switch(argv[1][1]){
        case 'e' :
            if ((strcmp(argv[2],"-f") == 0) && (argc == 6) && ((N = atoi(argv[3])) > 0)){
                printf("lol");
               res = b64_encode_f(N, argv[4], argv[5]);
            }
            else{
                if (argc == 4){
                   res = b64_encode(argv[2], argv[3]);
                }
                else{
                    printf("%s", help);
                    return 1;
                }
            }
            break;
        case 'd' :
            if (argc == 4){
                res = b64_decode(argv[2], argv[3]);
            }
            else{
                printf("%s", help);
                return 1;
            }
            break;
        case 'i' :
            if ((argc == 5) && (strcmp(argv[2],"-d") == 0)){
                res = b64_decode_i(argv[3], argv[4]);
            }
            else{
                printf("%s", help);
                return 1;
            }
            break;
        default :
            printf("%s", help);
            return 1;
    }

    if (res){
        printf("%s", help);
        return 1;
    }

    return 0;
}