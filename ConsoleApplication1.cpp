#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <math.h>
#include <ctime>
#include <string.h>
#include <cstdlib>
using namespace std;

typedef unsigned short BASE;
typedef unsigned long long int DBASE;
#define BASE_SIZE (sizeof(BASE)*8)

class BN
{
    int len, maxlen;
    BASE* coef;
public:
    BN();
    BN(int size, bool flag = false);
    BN(const BN&);

    ~BN() 
    { 
        if (coef) delete[] coef; 
        coef = NULL;
    }

    BN& operator = (const BN&);
    friend ostream& operator <<(ostream&, BN&);
    friend istream& operator >>(istream&, BN&);

    bool operator ==(const BN&);
    bool operator !=(const BN&);
    bool operator <(const BN&);
    bool operator <=(const BN&);
    bool operator >(const BN&);
    bool operator >=(const BN&);

    BN operator +(const BN&);
    BN& operator +=(const BN&);
    BN operator +(BASE);
    BN& operator +=(BASE);

    BN operator -(const BN&);
    BN& operator -=(const BN&);

    BN operator *(BASE);
    BN operator *(const BN&);
    BN& operator *=(const BN&);

    BN operator /(BASE);
    BASE operator %(BASE);
    void output10();
    void input10();

    BN operator /(BN);
    BN operator %(BN);
};

BN::BN()
{
    maxlen = 1, len = 1;
    
    coef = new BASE[maxlen];
    coef[0] = 0;
}

BN::BN(int size, bool flag)
{
    if (size < 1) exit(EXIT_FAILURE);

    maxlen = size, len = size;
    coef = new BASE[maxlen];

    if (flag) 
    {
        //srand(time(NULL));
        
        for (int i = 0; i < maxlen; i++) 
        {
            coef[i] = (rand() << 16) + rand(); 
            //coef[i] = rand();
        }
        
        for (int i = maxlen - 1; i > 0 && coef[i] == 0; i--) 
        {
            len--;
        }
    }
    else 
    {
        len = 1;

        for (int i = 0; i < maxlen; i++) 
        {
            coef[i] = 0;
        }
    }
}

BN::BN(const BN& BigN)
{
    if (!BigN.coef || BigN.maxlen < 1 || BigN.len < 1) exit(0);

    maxlen = BigN.maxlen;
    len = BigN.len;

    coef = new BASE[maxlen];
    for (int i = 0; i < maxlen; i++)
    {
        coef[i] = BigN.coef[i];
    }
}

BN& BN::operator=(const BN& BigN) 
{
    if (this == &BigN) return *this;
    if(BigN.maxlen < 1 || BigN.len < 1 || !BigN.coef) exit(0);

    delete[] coef;
        
    maxlen = BigN.maxlen;
    len = BigN.len;
        
    coef = new BASE[maxlen];
    for (int i = 0; i < maxlen; i++) 
    {
        coef[i] = BigN.coef[i];
    }
    
    return *this;
}

ostream& operator <<(ostream& r, BN& BN)
{
    if (!BN.coef) return r;

    int i = 0, k = BASE_SIZE - 4;
    char* str = new char[BN.len * BASE_SIZE / 4 + 1];
    unsigned int tmp;

    for (int j = BN.len - 1; j >= 0; i++)
    {
        tmp = (BN.coef[j] >> k) & (0xf);
        if (0 <= tmp && tmp <= 9) str[i] = (char)(tmp + '0');
        if (10 <= tmp && tmp <= 15) str[i] = (char)(tmp - 10 + 'A');

        k -= 4;
        if (k < 0) 
        {
            k = BASE_SIZE - 4;
            j--;
        }
    }
    /*
    int g = 0, l = 0;
    while(str[g] == '0') g++;

    while(str[l] != '\0')
    {
        str[l] = str[g];
        l++; g++;
    }
    */
    str[BN.len * BASE_SIZE / 4] = '\0';
    r << str << "\n";

    return r;
}

istream& operator >>(istream& r, BN& BN2)
{
    char* str = new char[100];
    gets_s(str, 100);

    int size = (strlen(str) - 1) / (BASE_SIZE / 4) + 1;
    BN BN1(size);
    BN1.len = size;

    unsigned int tmp; int k = 0, j = 0;
    bool flag = true;
    for (int i = strlen(str) - 1; i >= 0; i--)
    {
        flag = false;
        if ('0' <= str[i] && str[i] <= '9') 
        {
            tmp = str[i] - '0'; 
            flag = true;
        }
        if ('a' <= str[i] && str[i] <= 'f')
        {
            tmp = str[i] - 'a' + 10;
            flag = true;
        }
        if ('A' <= str[i] && str[i] <= 'F')
        {
            tmp = str[i] - 'A' + 10;
            flag = true;
        }
        if (!flag) 
        {
            puts("ERROR: Wrong numbers imputed");
            exit(EXIT_FAILURE);
        }

        BN1.coef[j] |= tmp << (k * 4);
        k++;

        if (k >= BASE_SIZE / 4)
        {
            k = 0; j++;
        }
    }
    
    for (int i = BN1.maxlen - 1; i > 0 && BN1.coef[i] == 0; i--)
    {
        BN1.len--;
    }
    
    BN2 = BN1;

    delete[] str;
    return r;
}

bool BN::operator ==(const BN& BN2)
{
    if (len != BN2.len) return false;

    int i = len - 1;
    while (i >= 0 && coef[i] == BN2.coef[i])
    {
        i--;
    }
    
    if (i == -1) return true;
        else return false;
}

bool BN::operator !=(const BN& BN2)
{
    if (len != BN2.len) return true;

    int i = len - 1;
    while (i >= 0 && coef[i] == BN2.coef[i])
    {
        i--;
    }

    if (i == -1) return false;
    else return true;
}

bool BN::operator <(const BN& BN2)
{
    if (len < BN2.len) return true;
    if (len > BN2.len) return false;

    int i = len - 1;
    while (i >= 0 && coef[i] == BN2.coef[i])
    {
        i--;
    }

    if (i >= 0 && coef[i] < BN2.coef[i]) return true;
        else return false;
}

bool BN::operator <=(const BN& BN2)
{
    if (len < BN2.len) return true;
    if (len > BN2.len) return false;

    int i = len - 1;
    while (i > 0 && coef[i] == BN2.coef[i])
    {
        i--;
    }

    if (coef[i] <= BN2.coef[i]) return true;
    else return false;
}

bool BN::operator >(const BN& BN2)
{
    if (len > BN2.len) return true;
    if (len < BN2.len) return false;

    int i = len - 1;
    while (i >= 0 && coef[i] == BN2.coef[i])
    {
        i--;
    }

    if (i >= 0 && coef[i] > BN2.coef[i]) return true;
    else return false;
}

bool BN::operator >=(const BN& BN2)
{
    if (len > BN2.len) return true;
    if (len < BN2.len) return false;

    int i = len - 1;
    while (i > 0 && coef[i] == BN2.coef[i])
    {
        i--;
    }

    if (coef[i] >= BN2.coef[i]) return true;
    else return false;
}
    /*
    if(*this == BN2 || *this > BN2) return true;
    return false;
    */
BN BN::operator +(const BN& BN2)
{
    if (!BN2.coef) exit(EXIT_FAILURE);

    DBASE tmp; bool k = false; int min_len, max_len;
    BASE* MaxCoef;

    if (len > BN2.len)
    {
        max_len = len + 1;
        min_len = BN2.len;
        MaxCoef = coef;
    }
    else
    {
        max_len = BN2.len + 1;
        min_len = len;
        MaxCoef = BN2.coef;
    }
    
    BN result(max_len, 0);

    for (int i = 0; i < min_len; i++)
    {
        tmp = (DBASE)(coef[i] + BN2.coef[i]) + k;
        result.coef[i] = (BASE)tmp;
        k = (bool)(tmp >> BASE_SIZE);
    }

    for (int i = min_len; i < max_len - 1; i++) 
    {
        tmp = (DBASE)MaxCoef[i] + k;
        result.coef[i] = (BASE)tmp;
        k = (bool)(tmp >> BASE_SIZE);
    }

    result.coef[max_len - 1] = k;
    
    result.len = result.maxlen;
    for (int i = result.maxlen - 1; i > 0 && result.coef[i] == 0; i--)
    {
        result.len--;
    }

    return result;
}

BN& BN::operator +=(const BN& BN2)
{
    *this = *this + BN2;

    return *this;
}

BN BN::operator+(BASE Num)
{
    BN result(len + 1);
    bool k = false;
    
    DBASE tmp = (DBASE)(coef[0]) + Num;
    k = (bool)(tmp >> BASE_SIZE);
    result.coef[0] = (BASE)tmp;

    for (int i = 1; i < len; i++)
    {
        tmp = (DBASE)(coef[i]) + k;
        k = (bool)(tmp >> BASE_SIZE);
        result.coef[i] = (BASE)tmp;
    }
    result.coef[len] = k;

    result.len = result.maxlen;
    for (int i = result.maxlen - 1; i > 0 && result.coef[i] == 0; i--)
    {
        result.len--;
    }

    return result;
}

BN& BN::operator+=(BASE Num)
{
    *this = *this + Num;
    return *this;
}

BN BN::operator -(const BN& BN2)
{
    if (!BN2.coef) exit(EXIT_FAILURE);
    if (*this < BN2) exit(EXIT_FAILURE);
    //if (len == BN2.len && coef[len-1] < BN2.coef[len-1]) exit(EXIT_FAILURE);

    DBASE tmp; bool k = false;

    BN result(maxlen, 0);

    for (int i = 0; i < BN2.len; i++)
    {
        tmp = ((DBASE)1<<BASE_SIZE)|coef[i];
        tmp = tmp - BN2.coef[i] - k;
        result.coef[i] = (BASE)tmp;
        k = !(bool)(tmp >> BASE_SIZE);
    }

    for (int i = BN2.len; i < maxlen; i++)
    {
        tmp = ((DBASE)1 << BASE_SIZE) | coef[i];
        tmp -= k;
        result.coef[i] = (BASE)tmp;
        k = !(bool)(tmp >> BASE_SIZE);
    }

    result.len = result.maxlen;
    for (int i = result.maxlen - 1; i > 0 && result.coef[i] == 0; i--)
    {
        result.len--;
    }

    return result;
}

BN& BN::operator -=(const BN& BN2)
{
    *this = *this - BN2;

    return *this;
}

BN BN::operator *(BASE Num)
{
    DBASE tmp; BASE k = 0;

    //bool extra = (bool)((((DBASE)coef[len - 1]*(DBASE)Num))>>BASE_SIZE);
    //DBASE gg = (DBASE)coef[len - 1] * (DBASE)Num + ((DBASE)1 << BASE_SIZE) - 1;
    //gg = gg >> BASE_SIZE;
    //bool extra = (bool)gg;
    
    //int t = len;
    //if (extra) t++;
    //BN result(t, 0);

    BN result(maxlen + 1, 0);

    for (int i = 0; i < len; i++)
    {
        tmp = (DBASE)coef[i] * Num + k;
        k = (BASE)(tmp>>BASE_SIZE);
        result.coef[i] = (BASE)tmp;
    }
    result.coef[len] = k;

    result.len = result.maxlen;
    for (int i = result.maxlen - 1; i > 0 && result.coef[i] == 0; i--)
    {
        result.len--;
    }

    return result;
}

BN BN::operator*(const BN& BN2)
{
    if (!BN2.coef) exit(EXIT_FAILURE);

    DBASE tmp; BASE k;
    BN result(maxlen + BN2.maxlen, 0);

    for (int i = 0; i < len; i++)
    {
        if (coef[i] != 0)
        {
            k = 0;
            for (int j = 0; j < BN2.len; j++)
            {
                tmp = (DBASE)coef[i] * (DBASE)BN2.coef[j] + (DBASE)result.coef[i + j] + (DBASE)k;
                result.coef[i + j] = (BASE)tmp;
                k = (BASE)(tmp >> BASE_SIZE);
            }
            result.coef[i + BN2.len] = k;
        }
    }
    

    result.len = result.maxlen;
    for (int i = result.maxlen - 1; i > 0 && result.coef[i] == 0; i--)
    {
        result.len--;
    }

    return result;
}

BN& BN::operator *=(const BN& BN2)
{
    *this = *this * BN2;
    return *this;
}

BN BN::operator /(BASE Num)
{
    DBASE tmp; DBASE r = 0;
    BN result(len, 0);

    for (int j = len - 1; j >= 0; j--)
    {
        tmp = (r << BASE_SIZE) + coef[j];
        result.coef[j] = tmp / Num;
        r = tmp % Num;
    }

    result.len = result.maxlen;
    for (int i = result.maxlen - 1; i > 0 && result.coef[i] == 0; i--)
    {
        result.len--;
    }

    return result;
}

BASE BN::operator %(BASE Num)
{
    DBASE tmp; DBASE r = 0;

    for (int j = len - 1; j >= 0; j--)
    {
        tmp = (r << BASE_SIZE) | coef[j];
        r = tmp % Num;
    }

    return r;
}

void BN::input10()
{
    char* str = new char[1000];
    gets_s(str, 1000);

    BN result((strlen(str) - 1)/(BASE_SIZE/4) + 1);
    bool flag;

    for (int i = 0; str[i] != '\0'; i++)
    {
        flag = false;
        if (str[i] >= '0' && str[i] <= '9')
        {
            result = result * (BASE)10;
            //result.output10();
            BASE k = (BASE)(str[i] - '0'); // i = 40
            result += k; // 8450486 16720
            // result.coef[0] += k;
            //result.output10(); // -252 вместо +4
             // 64569874894479645641343798798345132449794984311246879641432465
            flag = true;
        }
        if (!flag)
        {
            puts("ERROR: Wrong numbers imputed");
            exit(EXIT_FAILURE);
        }
    }

    *this = result;
}

void BN::output10()
{
    char* str = new char[10000];
    BN tmp(*this), null(maxlen);
    
    if (tmp == null)
    {
        cout << 0;
    }

    int j;
    for (j = 0; tmp != null; j++)
    {
        str[j] = (char)(tmp % 10) + '0';
        tmp = tmp / 10;
    }

    for(int i = j - 1; i >= 0; i--)
    {
        cout << str[i];
    }
    cout << '\n';

    delete[] str;
}

BN BN::operator /(BN BN2)
{
    if (BN2.len == 1 && BN2.coef[0] == 0)
    {
        puts("ERROR: it is impossible to divide by zero\n");
        exit(1);
    }
    if (*this < BN2)
    {
        BN result;
        return result;
    }
    if (BN2.len == 1 && BN2.coef[0] != 0)
    {
        BN result = *this / BN2.coef[0];
        return result;
    }

    DBASE d = ((DBASE)1 << BASE_SIZE) / ((DBASE)BN2.coef[BN2.len - 1] + 1);
    
    BN BN_u;
    if (d == 1)
    {
        BN BN_tmp(len + 1);
        BN_tmp += *this;
        BN_u = BN_tmp;
    }
    else
    {
        BN_u = *this * (BASE)d;
    }

    if (len == BN_u.len)
    {
        BN BN_tmp(len + 1);
        BN_tmp += BN_u;
        BN_u = BN_tmp;
    }

    BN BN_v = BN2 * (BASE)d;

    int m = len - BN2.len; // t = n + m
    BN result(m + 1); DBASE q, r;
    DBASE b = (DBASE)1 << BASE_SIZE;
    bool k = false;
    for (int j = m; j >= 0; j--)
    {
        int i = j + BN2.len;
        q = ((DBASE)BN_u.coef[i] * b + BN_u.coef[i - 1]) / (DBASE)BN_v.coef[BN_v.len - 1]; // << BASE_SIZE
        r = ((DBASE)BN_u.coef[i] * b + BN_u.coef[i - 1]) % (DBASE)BN_v.coef[BN_v.len - 1];

        if (q == b || (DBASE)BN_v.coef[BN_v.len - 2] * q > b * r + BN_u.coef[i - 2])
        {
            q--;
            r += BN_v.coef[BN_v.len - 1];
            if (r < b && (q == b || (DBASE)BN_v.coef[BN_v.len - 2] * q > b * r + BN_u.coef[i - 2]))
            {
                q--;
                r += BN_v.coef[BN_v.len - 1];
            }
        }
        
        if (BN_u < (BN_v * (BASE)q)) k = true;
        BN_u -= BN_v * (BASE)q;

        result.coef[j] = (BASE)q;

        if (k)
        {
            result.coef[j]--;
            BN_u += BN_v;
            k = false;
        }
    }

    result.len = result.maxlen;
    for (int i = result.maxlen - 1; i > 0 && result.coef[i] == 0; i--)
    {
        result.len--;
    }

    return result;
}

BN BN::operator %(BN BN2)
{
    if (BN2.len == 1 && BN2.coef[0] == 0)
    {
        puts("ERROR: it is impossible to divide by zero\n");
        exit(1);
    }
    if (BN2.len == 1 && BN2.coef[0] != 0)
    {
        BN result;
        result += *this % BN2.coef[0];
        return result;
    }
    if (*this == BN2)
    {
        BN result;
        return result;
    }
    if (*this < BN2)
    {
        return *this;
    }

    DBASE d = ((DBASE)1 << BASE_SIZE) / ((DBASE)BN2.coef[BN2.len - 1] + 1);

    BN BN_u;
    if (d == 1)
    {
        BN BN_tmp(len + 1);
        BN_tmp += *this;
        BN_u = BN_tmp;
    }
    else BN_u = *this * (BASE)d;

    if (len == BN_u.len)
    {
        BN BN_tmp(len + 1);
        BN_tmp += BN_u;
        BN_u = BN_tmp;
    }

    BN BN_v = BN2 * (BASE)d;

    int m = len - BN2.len; // t = n + m
    DBASE q, r;
    DBASE b = (DBASE)1 << BASE_SIZE;
    bool k = false;
    for (int j = m; j >= 0; j--)
    {
        int i = j + BN2.len;
        q = ((DBASE)BN_u.coef[i] * b + BN_u.coef[i - 1]) / (DBASE)BN_v.coef[BN_v.len - 1];
        r = ((DBASE)BN_u.coef[i] * b + BN_u.coef[i - 1]) % (DBASE)BN_v.coef[BN_v.len - 1];

        if (q == b || q * (DBASE)BN_v.coef[BN_v.len - 2] > b * r + BN_u.coef[i - 2])
        {
            q--;
            r += BN_v.coef[BN_v.len - 1];
            if (r < b && (q == b || q * (DBASE)BN_v.coef[BN_v.len - 2] > b * r + BN_u.coef[i - 2]))
            {
                q--;
                r += BN_v.coef[BN_v.len - 1];
            }
        }

        if (BN_u < (BN_v * (BASE)q)) k = true;
        BN_u -= BN_v * (BASE)q;

        if (k)
        {
            BN_u += BN_v;
            k = false;
        }
    }

    BN_u = BN_u / (BASE)d;
    return BN_u;
}

int main()
{
    srand(time(NULL));
    BN BN1(5,1), BN2(3,1), r;
    //cout << BN1;
    //cin >> BN1;
    //cout << BN2;
    //BN1.input10();
    //BN2.input10();
    BN1.output10();
    BN2.output10();// 8450486 16720

    r = BN1 % BN2;
    BN1 = BN1 / BN2;
    BN1.output10();
    r.output10();

    //BN1 = (BN1 * BN2) + r;
    //BN1.output10();

    //cout << BN1;

    return 0;
}
