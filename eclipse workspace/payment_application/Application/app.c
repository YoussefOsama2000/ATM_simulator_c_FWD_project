/******************************************************************************
 *
 * Module: APPLICATION
 *
 * File Name: application.c
 *
 * Description: source file for application module
 *
 * Date : 22/11/2022
 *
 * Author: Youssef Osama
 *
 *******************************************************************************/
#include "../terminal_module/terminal.h"
#include "../card_module/card.h"
#include "../server_module/server.h"
#include <stdio.h>
ST_cardData_t g_cardData;
ST_terminalData_t g_termData;
ST_transaction g_transData;
void appStart(void){
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	while(1){
		fflush(stdout);
	/*
	 Getting card data ...
	 */

	while(getCardPAN(&g_cardData)!=CARD_OK)
		printf("wrong PAN\n");
	printf("Done\n");
	while(getCardHolderName(&g_cardData)!=CARD_OK)
		printf("wrong entry\n");
	printf("Done\n");
	while(getCardExpiryDate(&g_cardData)!=CARD_OK)
		printf("wrong expire Date\n");
	printf("Done\n");

	/*
	Getting terminal data ...
	 */
	float max=-1;
	fflush(stdout);
	while(getTransactionDate(&g_termData)!=TERMINAL_OK)
		printf("invalid input \n");
	printf("Done\n");
	while(1) {
		printf("Enter maximum amount transaction \n");
		scanf("%f",&max);
		if(setMaxAmount(&g_termData , max)!=TERMINAL_OK)
		{printf("invalid input \n");
		continue;}
		else break;
	} ;
	if(isCardExpired(&g_cardData, &g_termData)!=TERMINAL_OK)
	{
		printf("the card is Expired \n");
		appStart();
	}
	printf("Done\n");
	while(getTransactionAmount(&g_termData)!=TERMINAL_OK)
		printf("invalid amount \n");
	if(isBelowMaxAmount(&g_termData)!=TERMINAL_OK){
		printf("Exceeded the allowed limit \n");
appStart();
	}
	printf("Done \n");
	/*
	 Server started to validate the transaction
	 */
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
}
