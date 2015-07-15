#include <iostream>
using namespace std;

unsigned va, pa;

int main()
{
	while (1)
	{
		char ch;
		for (int i = 0; i < 5; ++i)
			scanf("%c", &ch);
		va = pa = 0;
		for (int i = 0; i < 8; ++i)
		{
			scanf("%c", &ch);
			va = va*16 + (ch<='9' ? ch-'0' : ch-'a'+10);
		}
		for (int i = 0; i < 7; ++i)
			scanf("%c", &ch);
		for (int i = 0; i < 8; ++i)
		{
			scanf("%c", &ch);
			pa = pa*16 + (ch<='9' ? ch-'0' : ch-'a'+10);
		}
		while (ch != '\n')
			scanf("%c", &ch);
		printf("va 0x%08x, pa 0x%08x, ", va, pa);
		printf("pde_idx 0x%08x, ", va>>22);
		printf("pde_ctx 0x%08x, ", ((((va-0xc0000000)>>22)+1)<<12)+3);
		printf("pte_idx 0x%08x, ", (va>>12)&1023);
		printf("pte_ctx 0x%08x\n", ((pa>>12)<<12)+3);
	}
	return 0;
}