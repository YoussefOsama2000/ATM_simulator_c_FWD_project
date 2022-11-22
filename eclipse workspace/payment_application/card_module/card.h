 /******************************************************************************
 *
 * Module: Card
 *
 * File Name: card.h
 *
 * Description: Header file for card module
 *
 * Date : 11/14/2022
 *
 * Author: Youssef Osama
 *
 *******************************************************************************/
#ifndef CARD_H_
#define CARD_H_
#include "../std_types.h"
/********************************************************
 * 				new types definitions					*
 ********************************************************/
typedef struct ST_cardData_t
{
     uint8 cardHolderName[25];
     uint8 primaryAccountNumber[20];
     uint8 cardExpirationDate[6];
}ST_cardData_t;
typedef enum EN_cardError_t
{
     CARD_OK, WRONG_NAME, WRONG_EXP_DATE, WRONG_PAN
}EN_cardError_t;
/********************************************************
 * 				function definitions					*
 ********************************************************/
/*
 * this function is implemented to get card holder name from the user
 * */
EN_cardError_t getCardHolderName(ST_cardData_t *cardData);
/*
 * this function is implemented to get card expire date
 * */
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData);
/*
 * this function is implemented to get card PAN
 * */
EN_cardError_t getCardPAN(ST_cardData_t *cardData);
#endif
