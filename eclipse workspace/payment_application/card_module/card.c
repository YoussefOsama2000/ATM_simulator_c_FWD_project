/******************************************************************************
 *
 * Module: Card
 *
 * File Name: card.c
 *
 * Description: source file for card module
 *
 * Date : 11/14/2022
 *
 * Author: Youssef Osama
 *
 *******************************************************************************/
#include "card.h"
#include <stdio.h>
/********************************************************
 * 				function implementation 				*
 ********************************************************/
/*
 * this function is implemented to get card holder name from the user
 * */
EN_cardError_t getCardHolderName(ST_cardData_t *cardData){
	uint8 counter=0;
	printf("Enter the card Holder Name  20-24 character including spaces \n");
	gets((char *)(cardData->cardHolderName));
	while((cardData->cardHolderName)[counter]!='\0')/*haven't reach null yet*/{
		if(((cardData->cardHolderName)[counter]<='z'&&(cardData->cardHolderName)[counter]>='a')
				||((cardData->cardHolderName)[counter]<='Z'&&(cardData->cardHolderName)[counter]>='A')||
				((cardData->cardHolderName)[counter]==' '))
			{
			counter++;
			continue;
			}
		else return WRONG_NAME;

	}
	if(counter<20||counter>24)
		return WRONG_NAME;
	else
		return CARD_OK;
}
/*
 * this function is implemented to get card expire date
 * */
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData){
	uint8 counter;
	printf("Enter card Expiry Date MMYY \n");
	for(counter=0;counter<4;counter++)
	{
		scanf("%c",cardData->cardExpirationDate+counter);
		if((cardData->cardExpirationDate)[counter]<'9'&&(cardData->cardExpirationDate)[counter]>'0')
			continue;
		else return WRONG_EXP_DATE;
	}
	/*
	 * the following lines check if the month between 1 and 12
	 * */
	if(
			(
					/*IF first digit is one second digit can be 1 or 2 only*/
					((cardData->cardExpirationDate)[0]=='1')&&
					(((cardData->cardExpirationDate)[1]=='1')||(cardData->cardExpirationDate)[1]=='2')
			)||
			(
					/*IF first digit is zero second digit can be 1-9*/
					(cardData->cardExpirationDate)[0]=='0'&&
					(((cardData->cardExpirationDate)[1]>='1')&&(cardData->cardExpirationDate)[1]<='9')

			)
	)
		return CARD_OK;
	else return WRONG_EXP_DATE;

}
/*
 * this function is implemented to get card PAN
 * */
EN_cardError_t getCardPAN(ST_cardData_t *cardData){
	unsigned char counter;
	printf("Enter card PAN number 16-19 numbers\n");
	gets((char*)(cardData->primaryAccountNumber));
	for(counter=0;counter<20;counter++){
		if((cardData->primaryAccountNumber)[counter]<='9'&&(cardData->primaryAccountNumber)[counter]>='0')
			continue;

		else {
			if(counter>=16&&(cardData->primaryAccountNumber)[counter]==0)
				return CARD_OK;
			else
				return WRONG_PAN;
		}
	}
	return CARD_OK;


}
