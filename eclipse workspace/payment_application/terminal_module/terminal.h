/******************************************************************************
 *
 * Module: Terminal
 *
 * File Name: terminal.h
 *
 * Description: Header file for terminal module
 *
 * Date : 11/14/2022
 *
 * Author: Youssef Osama
 *
 *******************************************************************************/
#ifndef TERMINAL_H_
#define TERMINAL_H_
#include "../std_types.h"
#include "../card_module/card.h"
/********************************************************
 * 				new types definitions					*
 ********************************************************/

typedef struct ST_terminalData_t
{
	float transAmount;
	float maxTransAmount;
	uint8 transactionDate[11];
}ST_terminalData_t;
typedef enum EN_terminalError_t
{
	TERMINAL_OK, WRONG_DATE, EXPIRED_CARD, INVALID_CARD, INVALID_AMOUNT, EXCEED_MAX_AMOUNT, INVALID_MAX_AMOUNT
}EN_terminalError_t ;
/********************************************************
 * 				function definitions					*
 ********************************************************/
/*
This function will ask for the transaction date and store it in terminal data.
Transaction date is 10 characters string in the format DD/MM/YYYY, e.g 25/06/2022.
If the transaction date is NULL or is less than 10 characters or wrong format will return the WRONG_DATE error, else return TERMINAL_OK
 */
EN_terminalError_t getTransactionDate(ST_terminalData_t *termData);
/*
 This function compares the card expiry date with the transaction date.
If the card expiration date is before the transaction date will return EXPIRED_CARD, else return TERMINAL_OK
 */
EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData);
/*
This function asks for the transaction amount and saves it into terminal data.
If the transaction amount is less than or equal to 0 will return INVALID_AMOUNT, else return TERMINAL_OK.
 * */
EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData);
/*
This function compares the transaction amount with the terminal max allowed amount.
If the transaction amount is larger than the terminal max allowed amount will return EXCEED_MAX_AMOUNT, else return TERMINAL_OK.
 * */
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData);
/*
This function takes the maximum allowed amount and stores it into terminal data.
Transaction max amount is a float number.
If transaction max amount less than or equal to 0 will return the INVALID_MAX_AMOUNT error, else return TERMINAL_OK.
 * */
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float maxAmount);

#endif
