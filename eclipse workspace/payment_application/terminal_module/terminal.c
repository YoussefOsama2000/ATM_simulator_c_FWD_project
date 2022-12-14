/******************************************************************************
 *
 * Module: terminal
 *
 * File Name: terminal.c
 *
 * Description: source file for terminal module
 *
 * Date : 11/14/2022
 *
 * Author: Youssef Osama
 *
 *******************************************************************************/
#include "terminal.h"
#include <stdio.h>
#include <stdlib.h>
/********************************************************
 * 				function implementation 				*
 ********************************************************/
/*
This function will ask for the transaction date and store it in terminal data.
Transaction date is 10 characters string in the format DD/MM/YYYY, e.g 25/06/2022.
If the transaction date is NULL or is less than 10 characters or wrong format will return the WRONG_DATE error, else return TERMINAL_OK
 */
EN_terminalError_t getTransactionDate(ST_terminalData_t *termData){
	uint8 counter;
	printf("Enter transaction Date  DD/MM/YYYY, e.g 25/06/2022 	\n");
	fflush(stdout);
	gets((char *)(termData->transactionDate));
	for(counter=0;counter<10;counter++){
		if(counter==2 || counter==5)
		{
			/*checking slash positions*/
			if((termData->transactionDate)[counter]=='/')
			{
				(termData->transactionDate)[counter]='\0';
				continue;}
			else return WRONG_DATE;
		}else
			/*checking that all other characters are numbers*/
			if((termData->transactionDate)[counter]>='0'&&(termData->transactionDate)[counter]<='9')
			{
				continue;
			}
			else {

				return WRONG_DATE;}
	}
	/*checking valid day*/
	if(atoi((char *)(termData->transactionDate)+0)>31)
	{
		printf("exceeded");
		return WRONG_DATE;}
	/*checking valid month*/
	if(atoi((char *)(termData->transactionDate)+3)>12)
	{
		printf("exceeded2");
		return WRONG_DATE;}
	return TERMINAL_OK;
}
/*
 This function compares the card expiry date with the transaction date.
If the card expiration date is before the transaction date will return EXPIRED_CARD, else return TERMINAL_OK
 */
EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData){
	uint8 cardMonthString[3] = {(cardData->cardExpirationDate)[0],(cardData->cardExpirationDate)[1],'\0'};
	uint8 cardYearString[3]={(cardData->cardExpirationDate)[2],(cardData->cardExpirationDate)[3],'\0'};
	uint8 cardMonthInt = atoi((char *)cardMonthString);
	uint8 cardYearInt= atoi((char *)cardYearString);
	uint8 currentMonthInt =atoi((char *)(termData->transactionDate)+3); /*		DD/MM/YYYY		*/
	uint8 currentYearInt=atoi((char *)(termData->transactionDate)+7);
	if(cardYearInt>=currentYearInt&&cardMonthInt>=currentMonthInt)
		return TERMINAL_OK;
	else return EXPIRED_CARD;
}
/*
This function asks for the transaction amount and saves it into terminal data.
If the transaction amount is less than or equal to 0 will return INVALID_AMOUNT, else return TERMINAL_OK.
 * */
EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData){
	printf("enter the transaction amount \n");
	scanf("%f",&(termData->transAmount));
	if((termData->transAmount)>0)
	return TERMINAL_OK;
	else return INVALID_AMOUNT;
}
/*
This function compares the transaction amount with the terminal max allowed amount.
If the transaction amount is larger than the terminal max allowed amount will return EXCEED_MAX_AMOUNT, else return TERMINAL_OK.
 * */
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData){
	if((termData->transAmount)>(termData->maxTransAmount))
		return EXCEED_MAX_AMOUNT;
	else return TERMINAL_OK;
}
/*
This function takes the maximum allowed amount and stores it into terminal data.
Transaction max amount is a float number.
If transaction max amount less than or equal to 0 will return the INVALID_MAX_AMOUNT error, else return TERMINAL_OK.
 * */
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float maxAmount){
	if(maxAmount>0){
		termData->maxTransAmount=maxAmount;
		return TERMINAL_OK;
	}
	else  return INVALID_MAX_AMOUNT;
}
