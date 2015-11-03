/**********************************************************************
* 版权所有 (C)2015, Zhou Zhaoxiong。
*
* 文件名称：BatchReadFile.c
* 文件标识：无
* 内容摘要：分批读取文件中的数据并打印出来
* 其它说明：无
* 当前版本：V1.0
* 作    者：Zhou Zhaoxiong
* 完成日期：20150528
*
**********************************************************************/
#include <stdio.h>

// 重定义数据类型
typedef signed   int    INT32;
typedef unsigned int    UINT32;
typedef unsigned char   UINT8;

// 宏定义
#define ONCE_READ_COUNT    5      // 一次读取的最大数据条数
#define MAX_RECORD_LEN     50     // 每条数据的最大长度

// 函数声明
INT32 ReadRecordFromFile(FILE *fp, UINT8 szRecordSet[][MAX_RECORD_LEN], UINT32 *piReadCnt);


/**********************************************************************
* 功能描述：主函数
* 输入参数：无
* 输出参数：无
* 返 回 值：无
* 其它说明：无
* 修改日期        版本号         修改人           修改内容
* -------------------------------------------------------------------
* 20150528        V1.0     Zhou Zhaoxiong        创建
***********************************************************************/
INT32 main()
{
    UINT8  szFileName[256] = {0};     // 包含完整路径的文件名
    FILE  *fp              = NULL;    // 文件句柄
    UINT32 iReadCnt        = 0;       // 一次读取到的记录数
    UINT32 iRecCnt         = 0;       // 记录数, 循环变量
    UINT32 iReadTimes      = 0;       // 读取文件次数
    INT32  iRetVal         = 0;       // 读取文件函数的返回值

    UINT8  szRecordSet[ONCE_READ_COUNT][MAX_RECORD_LEN] = {0};   // 存放从文件中读取到的记录内容集
    UINT8  szRecordInfo[MAX_RECORD_LEN]                 = {0};   // 存放从文件中读取到的每条记录内容

    // 获取包含完整路径的文件名
    strcpy(szFileName, "/home/zxin10/zhouzx/test/file/TestFile.txt");

    // 打开文件 
    fp = fopen(szFileName, "r");
    if (NULL == fp)   // 打开失败
    {
        printf("Open file %s failed!\n", szFileName);
        return -1;
    }

    // 读取文件内容并打印出来
    while (1)
    {
        iReadCnt = 0;
        memset(szRecordSet, 0x00, sizeof(szRecordSet));  

        iRetVal = ReadRecordFromFile(fp, szRecordSet, &iReadCnt); 
        if (iRetVal == -1)   // 表示函数执行失败, 直接退出
        {
            printf("Exec ReadRecordFromFile failed, please check!\n");
            break;
        }

        if (iReadCnt > 0)
        {
            iReadTimes ++;     // 读取次数加1
            printf("ReadTimes is: %d, the RecordInfo is:\n", iReadTimes);
        }

        for (iRecCnt = 0; iRecCnt < iReadCnt; iRecCnt ++)   // 打印读取到的记录值
        {
            memset(szRecordInfo, 0x00, sizeof(szRecordInfo));
            strncpy(szRecordInfo, szRecordSet[iRecCnt], sizeof(szRecordInfo)-1);

            printf("%s\n", szRecordInfo);
        }

        if (iRetVal == 0)   // 表示文件记录已扫描完, 直接退出
        {
            break;
        }
    }


    return 0; 
}


/**********************************************************************
* 功能描述：从文件中读取记录内容
* 输入参数：fp-文件指针     
* 输出参数：szRecordSet-记录内容信息集
            piReadCnt-读取到的条数
* 返 回 值：1-下一轮继续读取  0-本轮已读取完毕  -1-读取失败
* 其它说明：无
* 修改日期        版本号         修改人           修改内容
* -------------------------------------------------------------------
* 20150528        V1.0     Zhou Zhaoxiong        创建
***********************************************************************/
INT32 ReadRecordFromFile(FILE *fp, UINT8 szRecordSet[][MAX_RECORD_LEN], UINT32 *piReadCnt)
{
    UINT8  szRecordInfo[MAX_RECORD_LEN] = {0};    // 存储读取到的每条记录信息
    UINT32 iRecordLen                   = 0;      // 存储读取到的每条记录信息的长度

    if (fp == NULL || piReadCnt == NULL)
    {
        printf("ReadRecordFromFile: input paramter(s) is NULL!\n");
        return -1;
    }

    // 读取文件记录
    while ((!feof(fp)) && (!ferror(fp)))   // 遇到文件结尾或读取错误则退出
    {
        // 读取一条记录
        memset(szRecordInfo, 0x00, sizeof(szRecordInfo));
        fgets(szRecordInfo, sizeof(szRecordInfo)-1, fp);

        // 去掉记录后面的回车换行符
        iRecordLen = strlen(szRecordInfo);
        while (iRecordLen > 0)
        {
            if (szRecordInfo[iRecordLen-1] == '\n' || szRecordInfo[iRecordLen-1] == '\r')
            {
                szRecordInfo[iRecordLen-1] = '\0';
            }
            else
            {
                break;
            }

            iRecordLen --;
        }

        // 判断是否为空行, 是则继续读取
        if (strlen(szRecordInfo) == 0)
        {
            continue;
        }

        // 将记录信息拷贝到输出缓存中
        strncpy(szRecordSet[(*piReadCnt)++], szRecordInfo, MAX_RECORD_LEN-1);

        // 如果超出最大条数限制, 则直接返回
        if ((*piReadCnt) >= ONCE_READ_COUNT)
        {
            return 1;
        }
    }

    return 0;
}
