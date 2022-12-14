/******************************************************************************
 *
 * Module: server
 *
 * File Name: server.c
 *
 * Description: source file for terminal module
 *
 * Date : 11/14/2022
 *
 * Author: Youssef Osama
 *
 *******************************************************************************/
#include "server.h"
#include <stdio.h>
/***********************SERVER DATA BASE*****************************************/
static uint8 numberOfTransactions=0;
static ST_transaction transactionHistory[255];
static ST_accountsDB_t accountsDB[255] = {
		{2000,             RUNNING,     "4556919032645302" },
		{10000,         RUNNING,     "4024007143355096" },
		{50000,         BLOCKED,     "4024007137097993" },
		{200,             RUNNING,    "4716060009350662" },
		{200000,         RUNNING,     "4024007145204193" },
		{500,             RUNNING,     "4539275583732078" },
		{150000,         BLOCKED,    "4485174944722379" },
		{2000000,         RUNNING,     "4716027348199662" },
		{100,             RUNNING,     "4916760857026698" },
		{0,             BLOCKED,    "4716386979061215" } };
/********************************************************
 * 				function implementation 				*
 ********************************************************/
/*
This function will take all transaction data and validate its data, it contains all server logic.
It checks the account details and amount availability.
If the account does not exist return FRAUD_CARD, if the amount is not available will
return DECLINED_INSUFFECIENT_FUND, if the account is blocked will return DECLINED_STOLEN_CARD, if a transaction can't be saved will return INTERNAL_SERVER_ERROR , else returns APPROVED.
It will update the database with the new balance.
 * */
EN_transState recieveTransactionData(ST_transaction *transData){
	uint8 currentAccountCounter;
	uint8 charCounter=0;
	uint8 cardFound=0;
	ST_accountsDB_t accountRefrence;
	/*
	 checking if account is valid
	 */
	if(isValidAccount(&(transData->cardHolderData),&accountRefrence)!=SERVER_OK)
	{
		transData->transState=FRAUD_CARD;
		printf("ACCOUNT NOT FOUND \n");
		/*
		 *saving transaction state
		 */
		if(saveTransaction(transData)==SERVER_OK)
		{
			printf("Transaction Saved \n");
		}else printf("ERROR \n");
		return FRAUD_CARD;
	}
	if(isBlockedAccount(&accountRefrence)!=SERVER_OK)
	{
		transData->transState=DECLINED_STOLEN_CARD;
		printf("ACOUNT IS BLOCKED \n");
		/*
		 *saving transaction state
		 */
		if(saveTransaction(transData)==SERVER_OK)
		{
			printf("Transaction Saved \n");
		}else printf("ERROR \n");
		return DECLINED_STOLEN_CARD;
	}
	if(isAmountAvailable(&(transData->terminalData), &accountRefrence))
	{
		transData->transState=DECLINED_INSUFFECIENT_FUND;
		printf("DECLINED INSUFFECIENT FUND \n");
		if(saveTransaction(transData)==SERVER_OK)
		{
			printf("Transaction Saved \n");
		}else printf("ERROR \n");
		return DECLINED_INSUFFECIENT_FUND;
	}
	/*
	 making transaction
	 */

	if(saveTransaction(transData)==SERVER_OK)
	{
		printf("Transaction Saved \n");
	}else printf("ERROR \n");
	for (currentAccountCounter=0;currentAccountCounter<255;currentAccountCounter++)
	{
		charCounter=0;
		while(accountsDB[currentAccountCounter].primaryAccountNumber[charCounter]==(transData->cardHolderData.primaryAccountNumber[charCounter]))
		{
			charCounter++;
			if(transData->cardHolderData.primaryAccountNumber[charCounter]=='\0')
			{
				cardFound=1;
				break;
			}
		}
		if(cardFound==1)
			break;
	}
	if(cardFound==1)
	{
		printf("account FUND before transaction = %f \n",accountsDB[currentAccountCounter].balance);
		accountsDB[currentAccountCounter].balance-=transData->terminalData.transAmount;
		printf("current account FUND = %f \n",accountsDB[currentAccountCounter].balance);
	}

return APPROVED;


}
/*
This function will take card data and validate if the account related to this card exists or not.
It checks if the PAN exists or not in the server's database (searches for the card PAN in the DB).
If the PAN doesn't exist will return ACCOUNT_NOT_FOUND and the account reference will be NULL,
 else will return SERVER_OK and return a reference to this account in the DB.
 * */
EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t *accountRefrence){
	uint8 currentAccountCounter;
	uint8 charCounter=0;
	uint8 cardFound=0;
	for (currentAccountCounter=0;currentAccountCounter<255;currentAccountCounter++)
	{
		charCounter=0;
		while(accountsDB[currentAccountCounter].primaryAccountNumber[charCounter]==cardData->primaryAccountNumber[charCounter]){
			if(cardData->primaryAccountNumber[charCounter]=='\0')
			{
				cardFound=1;
				break;
			}
			charCounter++;
		}
		if(cardFound==1)
			break;
	}
	if(cardFound!=1)
	{
		return ACCOUNT_NOT_FOUND;
	}else{
		*accountRefrence=accountsDB[currentAccountCounter];
		return SERVER_OK;
	}
}
/*
This function takes a reference to the account into the database and verifies if it is blocked or not.
If the account is running it will return SERVER_OK, else if the account is blocked it will return BLOCKED_ACCOUNT.
 * */
EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence){
	if(accountRefrence->state==BLOCKED){
		return BLOCKED_ACCOUNT;
	} else return SERVER_OK;
}
/*
This function will take terminal data and a reference to the account in the database and check if the account has a sufficient amount
to withdraw or not.
It checks if the transaction's amount is available or not.
If the transaction amount is greater than the balance in the database will return LOW_BALANCE, else will return SERVER_OK.
 * */
EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, ST_accountsDB_t *accountRefrence){
	if((accountRefrence->balance)>=(termData->transAmount))
		return SERVER_OK;
	else return LOW_BALANCE;
}
/*
This function will store all transaction data in the transactions database.
It gives a sequence number to a transaction, this number is incremented once a transaction is processed into the server,
 you must check the last sequence number in the server to give the new transaction a new sequence number.
It saves any type of transaction, APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD, FRAUD_CARD, INTERNAL_SERVER_ERROR.
It will list all saved transactions using the listSavedTransactions function.
Assuming that the connection between the terminal and server is always connected, then it will return SERVER_OK.
 * */
EN_serverError_t saveTransaction(ST_transaction *transData){
	static uint8 sequenceNumber=0;
	transData->transactionSequenceNumber=sequenceNumber;
	transactionHistory[sequenceNumber]=*transData;
	sequenceNumber++;
	numberOfTransactions++;
	return SERVER_OK;
}
/*
 *This function prints all transactions found in the transactions DB
*/
void listSavedTransactions(void){
	uint8 counter =0;
	for(counter=0;counter<numberOfTransactions;counter++)
	{
	printf("######################### \n");
	printf("Transaction Sequence Number: %d \n",counter);
	printf("Transaction Date: ");
	printf(" %s/%s/%s \n",transactionHistory[counter].terminalData.transactionDate,
			transactionHistory[counter].terminalData.transactionDate+3,
			transactionHistory[counter].terminalData.transactionDate+6);
	printf("Transaction Amount: %f \n",transactionHistory[counter].terminalData.transAmount);
	switch (transactionHistory[counter].transState) {
		case APPROVED:
			printf("transaction state : APPROVED \n");
			break;
		case DECLINED_INSUFFECIENT_FUND:
			printf("transaction state : DECLINED_INSUFFECIENT_FUND \n");
			break;
		case DECLINED_STOLEN_CARD:
			printf("transaction state : DECLINED_STOLEN_CARD \n");
			break;
		case FRAUD_CARD:
			printf("transaction state : FRAUD_CARD \n");
			break;
		case INTERNAL_SERVER_ERROR:
			printf("transaction state : INTERNAL_SERVER_ERROR \n");
			break;
		default:
			printf("transaction state : NULL \n");
			break;
	}
	printf("Terminal Max Amount: %f \n",transactionHistory[counter].terminalData.maxTransAmount);
	printf("Card holder Name: %s \n",transactionHistory[counter].cardHolderData.cardHolderName);
	printf("PAN: %s \n",transactionHistory[counter].cardHolderData.primaryAccountNumber);
	printf("Card Expiration Date: %s \n",transactionHistory[counter].cardHolderData.cardExpirationDate);
	printf("######################### \n");
	}

}

