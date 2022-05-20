#include<iostream>
#include<windows.h>
#include<fstream>
#include<cstdlib>
#include<cstdio>
#include<cmath>
#include<iomanip>
using namespace std;
unsigned char* pBmpBuf;//讀入影象資料的指標

int bmpWidth;//影象的寬
int bmpHeight;//影象的高
RGBQUAD* pColorTable;//顏色表指標

int biBitCount;//影象型別，每畫素位數



//-----------------------------------------------------------------------------------------
//給定一個影象點陣圖資料、寬、高、顏色表指標及每畫素所佔的位數等資訊,將其寫到指定檔案中
bool readBmp(char* bmpName)
{
    FILE* fp = fopen(bmpName, "rb");//二進位制讀方式開啟指定的影象檔案

    if (fp == 0)
        return 0;

    //跳過點陣圖檔案頭結構BITMAPFILEHEADER

    fseek(fp, sizeof(BITMAPFILEHEADER), 0);

    //定義點陣圖資訊頭結構變數，讀取點陣圖資訊頭進記憶體，存放在變數head中

    BITMAPINFOHEADER head;

    fread(&head, sizeof(BITMAPINFOHEADER), 1, fp); //獲取影象寬、高、每畫素所佔位數等資訊

    bmpWidth = head.biWidth;

    bmpHeight = head.biHeight;

    biBitCount = head.biBitCount;//定義變數，計算影象每行畫素所佔的位元組數（必須是4的倍數）

    int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;//灰度影象有顏色表，且顏色表表項為256

    if (biBitCount == 8)
    {

        //申請顏色表所需要的空間，讀顏色表進記憶體

        pColorTable = new RGBQUAD[256];

        fread(pColorTable, sizeof(RGBQUAD), 256, fp);

    }

    //申請點陣圖資料所需要的空間，讀點陣圖資料進記憶體

    pBmpBuf = new unsigned char[lineByte * bmpHeight];

    fread(pBmpBuf, 1, lineByte * bmpHeight, fp);

    fclose(fp);//關閉檔案

    return 1;//讀取檔案成功
}

bool saveBmp(char* bmpName, unsigned char* imgBuf, int width, int height, int biBitCount, RGBQUAD* pColorTable)
{

    //如果點陣圖資料指標為0，則沒有資料傳入，函式返回

    if (!imgBuf)
        return 0;

    //顏色表大小，以位元組為單位，灰度影象顏色表為1024位元組，彩色影象顏色表大小為0

    int colorTablesize = 0;

    if (biBitCount == 8)
        colorTablesize = 1024;

    //待儲存影象資料每行位元組數為4的倍數

    int lineByte = (width * biBitCount / 8 + 3) / 4 * 4;

    //以二進位制寫的方式開啟檔案

    FILE* fp = fopen(bmpName, "wb");

    if (fp == 0)
        return 0;

    //申請點陣圖檔案頭結構變數，填寫檔案頭資訊

    BITMAPFILEHEADER fileHead;

    fileHead.bfType = 0x4D42;//bmp型別

    //bfSize是影象檔案4個組成部分之和

    fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + lineByte * height;

    fileHead.bfReserved1 = 0;

    fileHead.bfReserved2 = 0;

    //bfOffBits是影象檔案前3個部分所需空間之和

    fileHead.bfOffBits = 54 + colorTablesize;

    //寫檔案頭進檔案

    fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, fp);

    //申請點陣圖資訊頭結構變數，填寫資訊頭資訊

    BITMAPINFOHEADER head;

    head.biBitCount = biBitCount;

    head.biClrImportant = 0;

    head.biClrUsed = 0;

    head.biCompression = 0;

    head.biHeight = height;

    head.biPlanes = 1;

    head.biSize = 40;

    head.biSizeImage = lineByte * height;

    head.biWidth = width;

    head.biXPelsPerMeter = 0;

    head.biYPelsPerMeter = 0;

    //寫點陣圖資訊頭進記憶體

    fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);

    //如果灰度影象，有顏色表，寫入檔案 

    if (biBitCount == 8)
        fwrite(pColorTable, sizeof(RGBQUAD), 256, fp);

    //寫點陣圖資料進檔案

    fwrite(imgBuf, height * lineByte, 1, fp);

    //關閉檔案

    fclose(fp);

    return 1;

}
void doIt()
{
    char readPath[] = "D:\\1.bmp";
    readBmp(readPath);
    // 輸出整體影象資訊
    cout << "width=" << bmpWidth << "height=" << bmpHeight << "biBitCount=" << biBitCount << endl;
    // 影象的位元組數
    int linebyte1 = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;
    int n = 0, m = 0, count_xiang_su = 0;

    // ofstream outfile("影象畫素.txt",ios::in|ios::trunc);將畫素資料存入TXT檔案。
   //初始化原始畫素的陣列。

    if (biBitCount == 8)
    {
        for (int i = 0; i < bmpHeight / 2; i++)
        {
            for (int j = 0; j < bmpWidth / 2; i++)
                *(pBmpBuf + i * linebyte1 + j) = 0;
        }
    }

    if (biBitCount == 24)
    {
        for (int i = 0; i < bmpHeight; i++)
        {
            for (int j = 0; j < bmpWidth; j++)
            {
                for (int k = 0; k < 3; k++)//每畫素RGB三個分量分別置0才變成黑色
                {
                    m = *(pBmpBuf + i * linebyte1 + j * 3 + k);

                    count_xiang_su++;

                }
                n++;
            }


        }
        cout << "總的畫素個素為:" << n << endl;
        cout << "----------------------------------------------------" << endl;
    }

    // jiang tuxiang shuju cunpan .
    char writePath[] = "D:\\necpy.BMP";
    saveBmp(writePath, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable);

    //清除緩衝區，pBmpBuf和pColorTable是全域性變數，在檔案讀入時申請的空間

    delete[]pBmpBuf;
    if (biBitCount == 8)
        delete[]pColorTable;

}




int main()
{
    doIt();
    system("pause");
    return 0;
}