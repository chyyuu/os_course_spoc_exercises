#include <iostream>
#include <cstdlib>
#include <string.h>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

class MMU {
    public:
        MMU();
        ~MMU();
        void getMemory(string vr);
    private:
        string mystob(string s);
        string mybtoh(string s); //二进制字符串转16进制字符串
        int mybtod(string s); //二进制字符串转10进制整数
        void getSeg(string s); //由虚地址获得二进制的pde，pte和offset

        string memory[4096];
	string disk[4096];
        string htob[16]; //16进制转为二进制字符串

        string pde;
        string pte;
        string offset; //二进制字符串记录的各个部分

};

MMU::MMU() {

    htob[0] = "0000";
    htob[1] = "0001";
    htob[2] = "0010";
    htob[3] = "0011";
    htob[4] = "0100";
    htob[5] = "0101";
    htob[6] = "0110";
    htob[7] = "0111";
    htob[8] = "1000";
    htob[9] = "1001";
    htob[10] = "1010";
    htob[11] = "1011";
    htob[12] = "1100";
    htob[13] = "1101";
    htob[14] = "1110";
    htob[15] = "1111";

    ifstream fin("datapage.txt");
    string read = "";
    int count = 0;
    int bytecount = 0;
    stringstream ss;
    while (getline(fin,read)) {
        ss.clear();
        ss << read;
        string s1 = "", s2="", s3="";
        ss >> s1 >> s2;
        //循环读入各个byte
        for (int i=0; i<32; i++) {
            ss >> s3;
            string num = mystob(s3);
            //if (bytecount>=64 && bytecount<96) cout << s3 << " ";
            memory[bytecount++] = num;
        }

        count++;
    }
    cout << "内存读入完毕，共：" << count << "个page， " << bytecount << " 个Byte" << endl;
    fin.close();

    fin.open("datadisk.txt");
    read = "";
    count = 0;
    bytecount = 0;
    ss.clear();
    while (getline(fin,read)) {
        ss.clear();
        ss << read;
        string s1 = "", s2="", s3="";
        ss >> s1 >> s2;
        //循环读入各个byte
        for (int i=0; i<32; i++) {
            ss >> s3;
            string num = mystob(s3);
            //if (bytecount<20) cout << num << endl;

            disk[bytecount++] = num;
        }

        count++;
    }
    cout << "disk读入完毕，共：" << count << "个sector， " << bytecount << " 个Byte" << endl;
    fin.close();

}

MMU::~MMU(){}

string MMU::mystob(string s) {
    int num0 = 0;
    int num1 = 0;
    if (s[0]>='0' && s[0] <= '9') {
        num0 = s[0] - '0';
    } else {
        num0 = s[0]-'a'+10;
    }

    if (s[1]>='0' && s[1] <= '9') {
        num1 = s[1] - '0';
    } else {
        num1 = s[1]-'a'+10;
    }
    return htob[num0] + htob[num1] ;
}

void MMU::getSeg(string s) {
    string all = "";
    int num0, num1, num2, num3;
    if (s[0]>='0' && s[0] <= '9') {
        num0 = s[0] - '0';
    } else {
        num0 = s[0]-'a'+10;
    }

    if (s[1]>='0' && s[1] <= '9') {
        num1 = s[1] - '0';
    } else {
        num1 = s[1]-'a'+10;
    }

    if (s[2]>='0' && s[2] <= '9') {
        num2 = s[2] - '0';
    } else {
        num2 = s[2]-'a'+10;
    }

    if (s[3]>='0' && s[3] <= '9') {
        num3 = s[3] - '0';
    } else {
        num3 = s[3]-'a'+10;
    }

    all += htob[num0] + htob[num1] + htob[num2] + htob[num3];
    //cout << all << endl;
    int len = all.length();
    pte = "";
    pde = "";
    offset = "";
    int i;
    for (i=1; i<6; i++) {
        pde += all[i];
    }
    for (i=6; i<11; i++) {
        pte += all[i];
    }
    for (i=11; i<16; i++) {
        offset += all[i];
    }


}

string MMU::mybtoh(string s) { //二进制字符串转16进制字符串
    string change[16] = {"0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f"};
    int num = mybtod(s);

    int num1 = num / 16;
    int num2 = num % 16;

    string ans = change[num1]+change[num2];
    return ans;

}

int MMU::mybtod(string s) { //二进制字符串转10进制整数
    int len = s.length();
    int num = 0;
    int scal = 1;
    for (int i=len-1; i>=0; i--) {
        num += (s[i]-'0')*scal;
        scal *= 2;
    }
    return num;
}

void MMU::getMemory(string vr) {
    getSeg(vr);

    //pde
    string pdeindexh = mybtoh(pde) ;
    int pdeindexd = mybtod(pde);
    string pdecont = memory[3456+pdeindexd];
    string ptbaseb = "";
    for (int i=1; i<8; i++) ptbaseb += pdecont[i];
    string ptbaseh = mybtoh(ptbaseb);

    cout << "pde index: 0x" << pdeindexh << "  pde  contentes:(valid " << pdecont[0] << " , pt  0x" << ptbaseh << ")" << endl;
    if (pdecont[0] == '0') {
        cout << "Fault (page directory entry not valid)" << endl;
    } else {
         //pte
        int ptbased = mybtod(ptbaseb+"00000");
        string ptindexh = mybtoh(pte);
        int ptindexd = mybtod(pte);

        string ptecont = memory[ptbased+ptindexd];

              //page
        string pfnb = "";
        for (int i=1; i<8; i++) pfnb += ptecont[i];
        int pfnd = mybtod(pfnb); //获得物理页号
        string pfnh =mybtoh(pfnb);

        cout << "pte index: 0x" << ptindexh << "  pte  contentes:(valid " << ptecont[0] << " , pfn  0x" << pfnh << ")" << endl;
        int pa = pfnd*32+mybtod(offset); //实际物理地址

        cout << "pfn:" << pfnh << " " << offset << endl;

        if (ptecont[0] == '0') { //查找交换分区是否存在
            if (pa>=0 && pa<4096 && pfnd != 127) {
                cout << "To Disk Sector Address : " << pa << " value:" << mybtod(disk[pa]) << endl;
            } else {
                cout << "Fault (page table entry not valid) the pa is:" << pa << endl;
            }
        } else {
            int pa = pfnd*32+mybtod(offset); //实际物理地址
            cout << "Translates to pa: " << pa << " value:" << mybtod(memory[pa]) << endl;

        }
    }


}

int main() {
    MMU myMMU;
    string vr;
    cin >> vr;
    myMMU.getMemory(vr);
    return 0;
}
