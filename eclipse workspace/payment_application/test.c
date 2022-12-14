#include "test.h"
#include "terminal_module/terminal.h"
#include "card_module/card.h"
#include "server_module/server.h"
ST_cardData_t g_cardData;
ST_terminalData_t g_termData;
ST_transaction g_transData;
void testCardModule(ST_cardData_t *cardData){
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	while(getCardPAN(cardData)!=CARD_OK)
		printf("wrong PAN\n");
	printf("Done\n");
	while(getCardHolderName(cardData)!=CARD_OK)
		printf("wrong entry\n");
	printf("Done\n");
	while(getCardExpiryDate(cardData)!=CARD_OK)
		printf("wrong expire Date\n");
	printf("Done\n");
	g_cardData=*cardData;
}
void testTerminalModule(ST_terminalData_t *termData){
	float max=-1;
	while(getTransactionDate(termData)!=TERMINAL_OK)
		printf("invalid input \n");
	printf("Done\n");
	while(setMaxAmount(termData , max)!=TERMINAL_OK)
		{
		printf("Enter maximum amount transaction \n");
		scanf("%f",&max);
		}
	if(isCardExpired(&g_cardData, termData)!=TERMINAL_OK)
	{
		printf("the card is Expired \n");
	}
	printf("Done\n");
	while(getTransactionAmount(termData)!=TERMINAL_OK)
		printf("invalid amount \n");
	if(isBelowMaxAmount(termData)!=TERMINAL_OK){
		printf("Exceeded the allowed limit \n");
		g_termData=*termData;
		return;
	}
	g_termData=*termData;
}

void testServerModule(){
	g_transData.cardHolderData=g_cardData;
	g_transData.terminalData=g_termData;
	switch (recieveTransactionData(&g_transData)) {
		case APPROVED:
			printf("APPROVED \n");
			break;
		case DECLINED_INSUFFECIENT_FUND:
			printf("DECLINED_INSUFFECIENT_FUND \n");
			break;
		case DECLINED_STOLEN_CARD:
			printf("DECLINED_STOLEN_CARD \n");
			break;
		case FRAUD_CARD:
			printf("FRAUD_CARD \n");
			break;
		case INTERNAL_SERVER_ERROR:
			printf("INTERNAL_SERVER_ERROR \n");
			break;
	}
	listSavedTransactions();


}
