#include "..\TYPEDEFS.h"

//To be removed
#include "..\Card\card.h"
#include "..\Terminal\terminal.h"

#include "server.h"
#include <math.h>


ST_accountsDB_t validAccounts[255];

void fillTheArray()
{
	int8_t k, i;
	float amount = 129874000.697;
	uint64_t PAN = 10258976315209846;
	EN_accountState_t s = RUNNING;
	for(k = 0; k < 5; k++)
	{
		for (i = 0; i < 51; i++)
		{
			validAccounts[k*51 + i].balance = log10(amount + k*i) * (10000.0/(i+1));
			amount -= 1000.0;
			
			sprintf(validAccounts[k*51 + i].primaryAccountNumber, "%lld", PAN);
			PAN++;
			
			validAccounts[k*51 + i].state = s;
			if (s == RUNNING)
				s = BLOCKED;
			else
				s = RUNNING;
		}
	}
}

ST_transaction_t * ss_transactionsDB[255] = {NULL};

EN_transState_t recieveTransactionData(ST_transaction_t *transData)
{
	int16_t accountIndex;
	if(isValidAccount(&(transData->cardHolderData), &accountIndex) == ACCOUNT_NOT_FOUND)
		return FRAUD_CARD;
	
	if(isAmountAvailable(&(transData->terminalData), accountIndex) == LOW_BALANCE)
		return DECLINED_INSUFFECIENT_FUND;
	
	if (validAccounts[accountIndex].state == BLOCKED)
		return DECLINED_STOLEN_CARD;
	
	if (saveTransaction(transData) == SAVING_FAILED)
		return INTERNAL_SERVER_ERROR;
	
	validAccounts[accountIndex].balance -= transData->terminalData.transAmount;
	return APPROVED;
}

EN_serverError_t isValidAccount(ST_cardData_t *cardData, int16_t * refToAccount)
{
	*refToAccount = -1;
	int16_t i = *refToAccount;
	for (i = 0; i < 255; i++)
		if(!strcmp(cardData->primaryAccountNumber, validAccounts[i].primaryAccountNumber))
		{
			*refToAccount = i;
			return SERVER_OK;
		}
	*refToAccount = -1;
	return ACCOUNT_NOT_FOUND;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, int16_t accountIndex)
{
	int32_t beforePoint = termData->transAmount;
	if (beforePoint > (int32_t)(validAccounts[accountIndex].balance))
		return LOW_BALANCE;
	
	if (beforePoint == (int32_t)(validAccounts[accountIndex].balance))
	{
		beforePoint = (termData->transAmount) * 10000.0;
		if (beforePoint > (int32_t)(validAccounts[accountIndex].balance * 10000.0))
			return LOW_BALANCE;
	}
	
	return SERVER_OK;
}

EN_serverError_t saveTransaction(ST_transaction_t *transData)
{
	static uint32_t sequence = 0;
	transData->transactionSequenceNumber = sequence;
	if(sequence < 255 && (getTransaction(sequence, transData) == TRANSACTION_NOT_FOUND))
	{
		ss_transactionsDB[sequence] = (ST_transaction_t *)malloc(sizeof(ST_transaction_t));
		
		strcpy(ss_transactionsDB[sequence]->cardHolderData.cardHolderName, transData->cardHolderData.cardHolderName);
		strcpy(ss_transactionsDB[sequence]->cardHolderData.primaryAccountNumber, transData->cardHolderData.primaryAccountNumber);
		strcpy(ss_transactionsDB[sequence]->cardHolderData.cardExpirationDate, transData->cardHolderData.cardExpirationDate);
		
		ss_transactionsDB[sequence]->terminalData.transAmount = transData->terminalData.transAmount;
		ss_transactionsDB[sequence]->terminalData.maxTransAmount = transData->terminalData.maxTransAmount;
		strcpy(ss_transactionsDB[sequence]->terminalData.transactionDate, transData->terminalData.transactionDate);
		
		ss_transactionsDB[sequence]->transState = transData->transState;
		
		ss_transactionsDB[sequence]->transactionSequenceNumber = transData->transactionSequenceNumber;
		
		if(getTransaction(sequence, transData) == SERVER_OK)
		{
			sequence++;
			return SERVER_OK;
		}
	}
	
	return SAVING_FAILED;
	
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t *transData)
{
	uint16_t i;
	for (i = 0; i < 255; i++)
	{
		if (ss_transactionsDB[i] != NULL)
			if (transactionSequenceNumber == ss_transactionsDB[i]->transactionSequenceNumber)
			{
				transData = ss_transactionsDB[i];
				return SERVER_OK;
			}
	}
	
	return TRANSACTION_NOT_FOUND;
}

void clearTransDB()
{
	int16_t i;
	for (i = 0; i < 255; i++)
	{
		free(ss_transactionsDB[i]);
	}
}

/*
void main()
{
	int16_t i;
	fillTheArray();
	system("cls");
	printf("Test------------------\n");
	
	ST_transaction_t * transData = (ST_transaction_t *)malloc(sizeof(ST_transaction_t));
	{
	transData->cardHolderData.cardHolderName[0] = 'M'; 
	transData->cardHolderData.cardHolderName[1] = 'o';
	transData->cardHolderData.cardHolderName[2] = 'h';
	transData->cardHolderData.cardHolderName[3] = 'a';
	transData->cardHolderData.cardHolderName[4] = 'b';
	transData->cardHolderData.cardHolderName[5] = ' ';
	transData->cardHolderData.cardHolderName[6] = 'M';
	transData->cardHolderData.cardHolderName[7] = 'o';
	transData->cardHolderData.cardHolderName[8] = 'h';
	transData->cardHolderData.cardHolderName[9] = 'a';
	transData->cardHolderData.cardHolderName[10] = 'm';
	transData->cardHolderData.cardHolderName[11] = 'e';
	transData->cardHolderData.cardHolderName[12] = 'd';
	transData->cardHolderData.cardHolderName[13] = ' '; 
	transData->cardHolderData.cardHolderName[14] = 'Z';
	transData->cardHolderData.cardHolderName[15] = 'a';
	transData->cardHolderData.cardHolderName[16] = 'g';
	transData->cardHolderData.cardHolderName[17] = 'h';
	transData->cardHolderData.cardHolderName[18] = 'l';
	transData->cardHolderData.cardHolderName[19] = 'o';
	transData->cardHolderData.cardHolderName[20] = 'u';
	transData->cardHolderData.cardHolderName[21] = 'l';
	}
	
	{
	//PAN: 10258976315209846
	transData->cardHolderData.primaryAccountNumber[0] = '2';
	transData->cardHolderData.primaryAccountNumber[1] = '0';
	transData->cardHolderData.primaryAccountNumber[2] = '2';
	transData->cardHolderData.primaryAccountNumber[3] = '5';
	transData->cardHolderData.primaryAccountNumber[4] = '8';
	transData->cardHolderData.primaryAccountNumber[5] = '9';
	transData->cardHolderData.primaryAccountNumber[6] = '7';
	transData->cardHolderData.primaryAccountNumber[7] = '6';
	transData->cardHolderData.primaryAccountNumber[8] = '3';
	transData->cardHolderData.primaryAccountNumber[9] = '1';
	transData->cardHolderData.primaryAccountNumber[10] = '5';
	transData->cardHolderData.primaryAccountNumber[11] = '2';
	transData->cardHolderData.primaryAccountNumber[12] = '0';
	transData->cardHolderData.primaryAccountNumber[13] = '9';
	transData->cardHolderData.primaryAccountNumber[14] = '8';
	transData->cardHolderData.primaryAccountNumber[15] = '4';
	transData->cardHolderData.primaryAccountNumber[16] = '6';
	}
	
	{
	transData->cardHolderData.cardExpirationDate[0] = '1';
	transData->cardHolderData.cardExpirationDate[1] = '0';
	transData->cardHolderData.cardExpirationDate[2] = '/';
	transData->cardHolderData.cardExpirationDate[3] = '2';
	transData->cardHolderData.cardExpirationDate[4] = '2';
	}
	
	
	EN_serverError_t	valid, avlbl, saved;
	EN_transState_t		trans;
	
	getTransactionDate(&(transData->terminalData));
	getTransactionAmount(&(transData->terminalData));
	setMaxAmount(&(transData->terminalData));
	
	trans = recieveTransactionData(transData);
	
	
	valid = isValidAccount(&(transData->cardHolderData), &i);
	if(valid == ACCOUNT_NOT_FOUND && i == -1)
		printf("\nAccount not found!");
	
	avlbl = isAmountAvailable(&(transData->terminalData), i);
	if (avlbl == LOW_BALANCE)
		printf("\nLow Balance!");
	
	saved = saveTransaction(transData);
	if (saved == SAVING_FAILED)
		printf("\nSAVING FAILED!");
	
	if (valid == SERVER_OK && avlbl == SERVER_OK && saved == SERVER_OK)
		printf("\n\nTransaction succeeded!!");
	
	
	clearTransDB();
}

}
*/


