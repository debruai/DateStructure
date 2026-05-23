#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// 数据结构第四章
// 串（字符串）的表示与实现
// 串是由零个或多个字符组成的有限序列，一般记为：S = 'a₁a₂...aₙ' (n≥0)
// 串中任意个连续的字符组成的子序列称为该串的子串

typedef int Status;
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define OVERFLOW -2

// 串的堆分配存储表示
typedef struct {
    char* ch;       // 若是非空串，则按串长分配存储区，否则ch为NULL
    int length;     // 串长度
} HString;

//------------------------------算法4.1---串的初始化
Status InitString(HString* S) {
    if (!S) return ERROR;
    
    S->ch = NULL;
    S->length = 0;
    return OK;
}

//------------------------------算法4.2---串的赋值操作
Status StrAssign(HString* S, const char* chars) {
    if (!S || !chars) return ERROR;
    
    // 如果S已有内容，先释放
    if (S->ch) {
        free(S->ch);
        S->ch = NULL;
        S->length = 0;
    }
    
    int len = strlen(chars);
    if (len == 0) {
        S->ch = NULL;
        S->length = 0;
        return OK;
    }
    
    S->ch = (char*)malloc((len + 1) * sizeof(char));
    if (!S->ch) return OVERFLOW;
    
    strcpy(S->ch, chars);
    S->length = len;
    return OK;
}

//------------------------------算法4.3---串的复制操作
Status StrCopy(HString* T, const HString* S) {
    if (!T || !S) return ERROR;
    
    // 如果T已有内容，先释放
    if (T->ch) {
        free(T->ch);
        T->ch = NULL;
        T->length = 0;
    }
    
    if (S->length == 0) {
        T->ch = NULL;
        T->length = 0;
        return OK;
    }
    
    T->ch = (char*)malloc((S->length + 1) * sizeof(char));
    if (!T->ch) return OVERFLOW;
    
    strcpy(T->ch, S->ch);
    T->length = S->length;
    return OK;
}

//------------------------------算法4.4---判断串是否为空
Status StrEmpty(const HString* S) {
    if (!S) return TRUE;
    return (S->length == 0) ? TRUE : FALSE;
}

//------------------------------算法4.5---比较两个串的大小
int StrCompare(const HString* S, const HString* T) {
    if (!S || !T) return 0;
    
    // 逐字符比较
    int min_len = (S->length < T->length) ? S->length : T->length;
    for (int i = 0; i < min_len; i++) {
        if (S->ch[i] != T->ch[i]) {
            return S->ch[i] - T->ch[i];
        }
    }
    
    // 如果前面字符都相同，比较长度
    return S->length - T->length;
}

//------------------------------算法4.6---求串的长度
int StrLength(const HString* S) {
    if (!S) return 0;
    return S->length;
}

//------------------------------算法4.7---清空串
Status ClearString(HString* S) {
    if (!S) return ERROR;
    
    if (S->ch) {
        free(S->ch);
        S->ch = NULL;
    }
    
    S->length = 0;
    return OK;
}

//------------------------------算法4.8---连接两个串
Status Concat(HString* T, const HString* S1, const HString* S2) {
    if (!T || !S1 || !S2) return ERROR;
    
    // 如果T已有内容，先释放
    if (T->ch) {
        free(T->ch);
        T->ch = NULL;
        T->length = 0;
    }
    
    int total_len = S1->length + S2->length;
    if (total_len == 0) {
        T->ch = NULL;
        T->length = 0;
        return OK;
    }
    
    T->ch = (char*)malloc((total_len + 1) * sizeof(char));
    if (!T->ch) return OVERFLOW;
    
    // 复制S1
    if (S1->length > 0) {
        strcpy(T->ch, S1->ch);
    }
    
    // 连接S2
    if (S2->length > 0) {
        strcpy(T->ch + S1->length, S2->ch);
    }
    
    T->length = total_len;
    return OK;
}

//------------------------------算法4.9---求子串
Status SubString(HString* Sub, const HString* S, int pos, int len) {
    if (!Sub || !S) return ERROR;
    
    // 参数检查
    if (pos < 1 || pos > S->length || len < 0 || pos + len - 1 > S->length) {
        return ERROR;
    }
    
    // 如果Sub已有内容，先释放
    if (Sub->ch) {
        free(Sub->ch);
        Sub->ch = NULL;
        Sub->length = 0;
    }
    
    if (len == 0) {
        Sub->ch = NULL;
        Sub->length = 0;
        return OK;
    }
    
    Sub->ch = (char*)malloc((len + 1) * sizeof(char));
    if (!Sub->ch) return OVERFLOW;
    
    // 复制子串
    for (int i = 0; i < len; i++) {
        Sub->ch[i] = S->ch[pos - 1 + i];
    }
    Sub->ch[len] = '\0';
    Sub->length = len;
    
    return OK;
}

//------------------------------算法4.10---朴素模式匹配算法（Brute-Force）
int Index_BF(const HString* S, const HString* T, int pos) {
    if (!S || !T || T->length == 0) return 0;
    if (pos < 1 || pos > S->length) return 0;
    
    int i = pos - 1;  // S的当前位置
    int j = 0;        // T的当前位置
    
    while (i < S->length && j < T->length) {
        if (S->ch[i] == T->ch[j]) {
            i++;
            j++;
        } else {
            i = i - j + 1;  // 回溯到上次匹配开始的下一个位置
            j = 0;
        }
    }
    
    if (j == T->length) {
        return i - j + 1;  // 返回匹配开始的位置（从1开始）
    }
    
    return 0;  // 未找到
}

//------------------------------算法4.11---KMP模式匹配算法（计算next数组）
void GetNext(const HString* T, int next[]) {
    if (!T || T->length == 0) return;
    
    int i = 0;  // 后缀的末尾
    int j = -1; // 前缀的末尾
    next[0] = -1;
    
    while (i < T->length - 1) {
        if (j == -1 || T->ch[i] == T->ch[j]) {
            i++;
            j++;
            
            // 优化next数组（nextval）
            if (T->ch[i] != T->ch[j]) {
                next[i] = j;
            } else {
                next[i] = next[j];
            }
        } else {
            j = next[j];
        }
    }
}

//------------------------------算法4.12---KMP模式匹配算法
int Index_KMP(const HString* S, const HString* T, int pos) {
    if (!S || !T || T->length == 0) return 0;
    if (pos < 1 || pos > S->length) return 0;
    
    // 分配next数组
    int* next = (int*)malloc(T->length * sizeof(int));
    if (!next) return 0;
    
    GetNext(T, next);
    
    int i = pos - 1;  // S的当前位置
    int j = 0;        // T的当前位置
    
    while (i < S->length && j < T->length) {
        if (j == -1 || S->ch[i] == T->ch[j]) {
            i++;
            j++;
        } else {
            j = next[j];
        }
    }
    
    free(next);
    
    if (j == T->length) {
        return i - j + 1;  // 返回匹配开始的位置（从1开始）
    }
    
    return 0;  // 未找到
}

//------------------------------算法4.13---串的替换操作
Status Replace(HString* S, const HString* T, const HString* V) {
    if (!S || !T || !V) return ERROR;
    if (T->length == 0) return ERROR;
    
    HString result;
    InitString(&result);
    
    int pos = 1;
    int last_pos = 1;
    
    while (pos <= S->length) {
        int match_pos = Index_KMP(S, T, pos);
        if (match_pos == 0) {
            // 没有更多匹配，复制剩余部分
            if (last_pos <= S->length) {
                HString remaining;
                InitString(&remaining);
                SubString(&remaining, S, last_pos, S->length - last_pos + 1);
                
                HString temp;
                InitString(&temp);
                Concat(&temp, &result, &remaining);
                
                ClearString(&result);
                StrCopy(&result, &temp);
                
                ClearString(&remaining);
                ClearString(&temp);
            }
            break;
        }
        
        // 复制匹配前的部分
        if (match_pos > last_pos) {
            HString before;
            InitString(&before);
            SubString(&before, S, last_pos, match_pos - last_pos);
            
            HString temp;
            InitString(&temp);
            Concat(&temp, &result, &before);
            
            ClearString(&result);
            StrCopy(&result, &temp);
            
            ClearString(&before);
            ClearString(&temp);
        }
        
        // 复制替换串
        HString temp;
        InitString(&temp);
        Concat(&temp, &result, V);
        
        ClearString(&result);
        StrCopy(&result, &temp);
        ClearString(&temp);
        
        last_pos = match_pos + T->length;
        pos = match_pos + 1;
    }
    
    // 用结果替换原串
    ClearString(S);
    StrCopy(S, &result);
    ClearString(&result);
    
    return OK;
}

//------------------------------算法4.14---串的插入操作
Status StrInsert(HString* S, int pos, const HString* T) {
    if (!S || !T) return ERROR;
    if (pos < 1 || pos > S->length + 1) return ERROR;
    
    HString left, right;
    InitString(&left);
    InitString(&right);
    
    // 获取插入点左边的子串
    if (pos > 1) {
        SubString(&left, S, 1, pos - 1);
    }
    
    // 获取插入点右边的子串
    if (pos <= S->length) {
        SubString(&right, S, pos, S->length - pos + 1);
    }
    
    // 连接 left + T + right
    HString temp1, temp2;
    InitString(&temp1);
    InitString(&temp2);
    
    Concat(&temp1, &left, T);
    Concat(&temp2, &temp1, &right);
    
    // 更新原串
    ClearString(S);
    StrCopy(S, &temp2);
    
    // 清理临时串
    ClearString(&left);
    ClearString(&right);
    ClearString(&temp1);
    ClearString(&temp2);
    
    return OK;
}

//------------------------------算法4.15---串的删除操作
Status StrDelete(HString* S, int pos, int len) {
    if (!S) return ERROR;
    if (pos < 1 || pos > S->length || len < 0 || pos + len - 1 > S->length) {
        return ERROR;
    }
    
    if (len == 0) return OK;
    
    HString left, right;
    InitString(&left);
    InitString(&right);
    
    // 获取删除点左边的子串
    if (pos > 1) {
        SubString(&left, S, 1, pos - 1);
    }
    
    // 获取删除点右边的子串
    if (pos + len - 1 < S->length) {
        SubString(&right, S, pos + len, S->length - (pos + len) + 1);
    }
    
    // 连接 left + right
    HString result;
    InitString(&result);
    Concat(&result, &left, &right);
    
    // 更新原串
    ClearString(S);
    StrCopy(S, &result);
    
    // 清理临时串
    ClearString(&left);
    ClearString(&right);
    ClearString(&result);
    
    return OK;
}

//------------------------------算法4.16---串的销毁操作
Status DestroyString(HString* S) {
    return ClearString(S);
}

//------------------------------辅助函数：打印串
void PrintString(const HString* S) {
    if (!S) {
        printf("NULL");
        return;
    }
    
    if (StrEmpty(S)) {
        printf("(空串)");
    } else {
        printf("\"%s\"", S->ch);
    }
    printf(" [长度: %d]", S->length);
}

//------------------------------测试主函数
int main() {
    printf("======= 串（字符串）数据结构测试 =======\n");
    
    // 1. 初始化串
    printf("\n1. 初始化串测试:\n");
    HString str1, str2, str3;
    InitString(&str1);
    InitString(&str2);
    InitString(&str3);
    
    printf("   串str1初始化: "); PrintString(&str1); printf("\n");
    printf("   串str2初始化: "); PrintString(&str2); printf("\n");
    
    // 2. 赋值操作
    printf("\n2. 赋值操作测试:\n");
    StrAssign(&str1, "Hello World!");
    StrAssign(&str2, "数据结构");
    printf("   str1赋值后: "); PrintString(&str1); printf("\n");
    printf("   str2赋值后: "); PrintString(&str2); printf("\n");
    
    // 3. 复制操作
    printf("\n3. 复制操作测试:\n");
    StrCopy(&str3, &str1);
    printf("   复制str1到str3: "); PrintString(&str3); printf("\n");
    
    // 4. 比较操作
    printf("\n4. 比较操作测试:\n");
    HString str4, str5;
    InitString(&str4);
    InitString(&str5);
    StrAssign(&str4, "abc");
    StrAssign(&str5, "abd");
    
    printf("   比较\"abc\"和\"abd\": %d\n", StrCompare(&str4, &str5));
    printf("   比较\"abd\"和\"abc\": %d\n", StrCompare(&str5, &str4));
    printf("   比较\"abc\"和\"abc\": %d\n", StrCompare(&str4, &str4));
    
    ClearString(&str4);
    ClearString(&str5);
    
    // 5. 连接操作
    printf("\n5. 连接操作测试:\n");
    HString concat_result;
    InitString(&concat_result);
    StrAssign(&str4, "Hello ");
    StrAssign(&str5, "World!");
    Concat(&concat_result, &str4, &str5);
    printf("   连接\"Hello \"和\"World!\": "); PrintString(&concat_result); printf("\n");
    
    // 6. 求子串操作
    printf("\n6. 求子串操作测试:\n");
    HString sub;
    InitString(&sub);
    SubString(&sub, &str1, 7, 5);
    printf("   str1=\"Hello World!\"从第7个字符开始取5个字符: ");
    PrintString(&sub); printf("\n");
    
    // 7. 朴素模式匹配算法（Brute-Force）
    printf("\n7. 朴素模式匹配算法（Brute-Force）测试:\n");
    HString pattern1;
    InitString(&pattern1);
    StrAssign(&pattern1, "World");
    int pos_bf = Index_BF(&str1, &pattern1, 1);
    printf("   在\"Hello World!\"中查找\"World\"（BF算法）: ");
    if (pos_bf > 0) {
        printf("找到，位置: %d\n", pos_bf);
    } else {
        printf("未找到\n");
    }
    
    // 8. KMP模式匹配算法
    printf("\n8. KMP模式匹配算法测试:\n");
    HString text, pattern2;
    InitString(&text);
    InitString(&pattern2);
    StrAssign(&text, "ababcabcacbab");
    StrAssign(&pattern2, "abcac");
    
    int pos_kmp = Index_KMP(&text, &pattern2, 1);
    printf("   在\"ababcabcacbab\"中查找\"abcac\"（KMP算法）: ");
    if (pos_kmp > 0) {
        printf("找到，位置: %d\n", pos_kmp);
    } else {
        printf("未找到\n");
    }
    
    // 9. 替换操作
    printf("\n9. 替换操作测试:\n");
    HString str6, old_str, new_str;
    InitString(&str6);
    InitString(&old_str);
    InitString(&new_str);
    
    StrAssign(&str6, "I like apple, apple is delicious.");
    StrAssign(&old_str, "apple");
    StrAssign(&new_str, "banana");
    
    printf("   原串: "); PrintString(&str6); printf("\n");
    Replace(&str6, &old_str, &new_str);
    printf("   将\"apple\"替换为\"banana\"后: "); PrintString(&str6); printf("\n");
    
    // 10. 插入操作
    printf("\n10. 插入操作测试:\n");
    HString str7, insert_str;
    InitString(&str7);
    InitString(&insert_str);
    
    StrAssign(&str7, "Hello !");
    StrAssign(&insert_str, "World");
    printf("   原串: "); PrintString(&str7); printf("\n");
    StrInsert(&str7, 7, &insert_str);
    printf("   在第7个位置插入\"World\"后: "); PrintString(&str7); printf("\n");
    
    // 11. 删除操作
    printf("\n11. 删除操作测试:\n");
    HString str8;
    InitString(&str8);
    StrAssign(&str8, "Hello World!");
    printf("   原串: "); PrintString(&str8); printf("\n");
    StrDelete(&str8, 7, 6);
    printf("   从第7个位置删除6个字符后: "); PrintString(&str8); printf("\n");
    
    // 12. KMP算法next数组测试
    printf("\n12. KMP算法next数组测试:\n");
    HString test_pattern;
    InitString(&test_pattern);
    StrAssign(&test_pattern, "ababaaababaa");
    
    int* next = (int*)malloc(test_pattern.length * sizeof(int));
    if (next) {
        GetNext(&test_pattern, next);
        printf("   模式串\"ababaaababaa\"的next数组: ");
        for (int i = 0; i < test_pattern.length; i++) {
            printf("%d ", next[i]);
        }
        printf("\n");
        free(next);
    }
    
    // 13. 性能对比：BF vs KMP
    printf("\n13. 性能对比测试（BF算法 vs KMP算法）:\n");
    HString long_text, long_pattern;
    InitString(&long_text);
    InitString(&long_pattern);
    
    // 构造一个较长的文本和模式
    char text_buf[1001];
    char pattern_buf[11];
    for (int i = 0; i < 1000; i++) {
        text_buf[i] = 'a' + (i % 26);
    }
    text_buf[1000] = '\0';
    
    for (int i = 0; i < 10; i++) {
        pattern_buf[i] = 'a' + ((i + 20) % 26);
    }
    pattern_buf[10] = '\0';
    
    StrAssign(&long_text, text_buf);
    StrAssign(&long_pattern, pattern_buf);
    
    printf("   在长度为1000的文本中查找长度为10的模式串...\n");
    int bf_result = Index_BF(&long_text, &long_pattern, 1);
    int kmp_result = Index_KMP(&long_text, &long_pattern, 1);
    printf("   BF算法结果: %s\n", bf_result > 0 ? "找到" : "未找到");
    printf("   KMP算法结果: %s\n", kmp_result > 0 ? "找到" : "未找到");
    
    // 14. 清理所有串
    printf("\n14. 清理所有串:\n");
    DestroyString(&str1);
    DestroyString(&str2);
    DestroyString(&str3);
    DestroyString(&concat_result);
    DestroyString(&sub);
    DestroyString(&pattern1);
    DestroyString(&text);
    DestroyString(&pattern2);
    DestroyString(&str6);
    DestroyString(&old_str);
    DestroyString(&new_str);
    DestroyString(&str7);
    DestroyString(&insert_str);
    DestroyString(&str8);
    DestroyString(&test_pattern);
    DestroyString(&long_text);
    DestroyString(&long_pattern);
    
    printf("   所有串已销毁\n");
    
    printf("\n======= 测试完成 =======\n");
    
    return 0;
}
