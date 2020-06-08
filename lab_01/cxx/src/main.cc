#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>

#include "Picard.h"
#include "MTPicard.h"
#include "FFTPicard.h"
#include "EulerMethod.h"

int maxNumLen(std::vector<double> nums, int precision,
              int first=0, char *newFormat=nullptr);

int main(int argc, char **argv)
{
    mmlabs::APicard *picard = new mmlabs::Picard;

    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-mt") == 0)
        {
            delete picard;
            picard = new mmlabs::MTPicard;
        }
        if (strcmp(argv[i], "-fft") == 0)
        {
            delete picard;
            picard = new mmlabs::FFTPicard;
        }
    }

    std::vector<double> X;
    std::vector<double> A1;
    std::vector<double> A2;
    std::vector<double> A3;
    std::vector<double> A4;
    std::vector<double> AN;
    std::vector<double> EX;
    std::vector<double> IM;

    int approx;
    double h;
    double tableStep;
    const double EPS = 1e-2;

    printf("Введите приближение метода Пикара: ");
    scanf("%d", &approx);
    printf("Введите шаг метода Эйлера: ");
    scanf("%lf", &h);
    printf("Введите шаг таблицы: ");
    scanf("%lf", &tableStep);
    printf("\n");

    double x = 0;
    picard->computePol(approx);

    do
    {
        X.push_back(x);

        A1.push_back((*picard)(x, 1));
        A2.push_back((*picard)(x, 2));
        A3.push_back((*picard)(x, 3));
        A4.push_back((*picard)(x, 4));
        AN.push_back((*picard)(x));

        EX.push_back(mmlabs::EulerMethod::explicitMethod(x, h));
        IM.push_back(mmlabs::EulerMethod::implicitMethod(x, h));

        x += tableStep;
    }
    while (fabs(*EX.rbegin() - *IM.rbegin()) < EPS);

    char titleX[256] = "x\0";
    char titleA1[256] = "1-ое приближение\0";
    char titleA2[256] = "2-ое приближение\0";
    char titleA3[256] = "3-ое приближение\0";
    char titleA4[256] = "4-ое приближение\0";
    char titleAN[256];
    sprintf(titleAN, "%d-ое приближение", approx);
    char titleEX[256] = " Явный\0";
    const char titleIM[] = "Неявный\0";


    char formatX[265];
    char formatA1[265];
    char formatA2[265];
    char formatA3[265];
    char formatA4[265];
    char formatAN[265];
    char formatEX[265];
    char formatIM[265];

    int cellLenX = maxNumLen(X, 2, strlen(titleX)/2, formatX);
    int cellLenA1 = maxNumLen(A1, 4, strlen(titleA1)/2, formatA1);
    int cellLenA2 = maxNumLen(A2, 4, strlen(titleA2)/2, formatA2);
    int cellLenA3 = maxNumLen(A3, 4, strlen(titleA3)/2, formatA3);
    int cellLenA4 = maxNumLen(A4, 4, strlen(titleA4)/2, formatA4);
    int cellLenAN = maxNumLen(AN, 4, strlen(titleAN)/2, formatAN);
    int cellLenEX = maxNumLen(EX, 4, strlen(titleEX)/2, formatEX);
    int cellLenIM = maxNumLen(IM, 4, (int)strlen(titleIM)/2, formatIM);

    printf(" %*s ", cellLenX, titleX);
    printf("|");
    printf(" %*s ", cellLenA1, titleA1);
    printf("|");
    printf(" %*s ", cellLenA2, titleA2);
    printf("|");
    printf(" %*s ", cellLenA3, titleA3);
    printf("|");
    printf(" %*s ", cellLenA4, titleA4);
    printf("|");
    printf(" %*s ", cellLenAN, titleAN);
    printf("|");
    printf(" %*s ", cellLenEX, titleEX);
    printf("|");
    printf(" %*s ", cellLenIM, titleIM);
    printf("\n");

    char t_hrule[256];
    int rowLen = 0;

    memset(t_hrule, '-', sizeof(t_hrule));

    rowLen += cellLenX + 2;
    t_hrule[rowLen] = '+';
    rowLen += cellLenA1 + 5;
    t_hrule[rowLen] = '+';
    rowLen += cellLenA2 + 5;
    t_hrule[rowLen] = '+';
    rowLen += cellLenA3 + 5;
    t_hrule[rowLen] = '+';
    rowLen += cellLenA4 + 5;
    t_hrule[rowLen] = '+';
    rowLen += cellLenAN + 5;
    t_hrule[rowLen] = '+';
    rowLen += cellLenEX + 3;
    t_hrule[rowLen] = '+';
    rowLen += cellLenIM + 2;
    t_hrule[rowLen + 1] = '\0';

    printf("%s\n", t_hrule);

    for (int i = 0; i < (int)X.size(); ++i)
    {
        printf(" ");
        printf(formatX, X[i]);
        printf(" ");
        printf("|");

        printf("   ");
        printf(formatA1, A1[i]);
        printf(" ");
        printf("|");

        printf("   ");
        printf(formatA2, A2[i]);
        printf(" ");
        printf("|");

        printf("   ");
        printf(formatA3, A3[i]);
        printf(" ");
        printf("|");

        printf("   ");
        printf(formatA4, A4[i]);
        printf(" ");
        printf("|");

        printf("   ");
        printf(formatAN, AN[i]);
        printf(" ");
        printf("|");

        printf(" ");
        printf(formatEX, EX[i]);
        printf(" ");
        printf("|");

        printf(" ");
        printf(formatIM, IM[i]);
        printf(" ");

        printf("\n");
    }

    delete picard;

    return 0;
}

int maxNumLen(std::vector<double> nums, int precision,
              int first, char *newFormat)
{
    int len;
    int maxLen = first;
    char buf[256];
    char numFormat[256];
    const char *baseFormat = "%%.%dlf";

    snprintf(numFormat, sizeof(numFormat), baseFormat, precision);

    for (double num: nums)
    {
        snprintf(buf, sizeof(buf), numFormat, num);

        if (maxLen < (len = strlen(buf)))
            maxLen = len;

        //printf("%s-\n", buf);

    }

    if (newFormat)
        sprintf(newFormat, "%%%d.%dlf", maxLen, precision);

    return maxLen;
}

