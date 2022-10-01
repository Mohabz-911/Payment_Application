#include "..\TYPEDEFS.h"
#include "..\Card\card.h"
#include "..\Terminal\terminal.h"
#include "..\Server\server.h"

void appStart(void)
{
	EN_transState_t error;
	
	ST_transaction_t * transaction = (ST_transaction_t *)malloc(sizeof(ST_transaction_t));
	
	fillTheArray();
	
	while (getCardHolderName(&(transaction->cardHolderData)) == WRONG_NAME)
		printf("\nPlease try again\n");
	
	while (getCardExpiryDate(&(transaction->cardHolderData)) == WRONG_EXP_DATE)
		printf("\nPlease try again\n");
	
	while (getCardPAN(&(transaction->cardHolderData)) == WRONG_PAN)
		printf("\nPlease try again\n");
	
	getTransactionDate(&(transaction->terminalData));
	if (isCardExpired(transaction->cardHolderData, transaction->terminalData) == EXPIRED_CARD)
	{
		printf("\nCannot make transactions\nExpired Card\n");
		return;
	}
	
	setMaxAmount(&(transaction->terminalData));
	getTransactionAmount(&(transaction->terminalData));
	if (isBelowMaxAmount(&(transaction->terminalData)) == EXCEED_MAX_AMOUNT)
	{
		printf("\nDeclined\nAmount Exceeding Limit\n");
		return;
	}
	
	fflush(stdout);
	error = recieveTransactionData(transaction);
	switch(error)
	{
		case FRAUD_CARD:
		printf("\nDeclined\nInvalid Account\n");
		break;
		
		case DECLINED_INSUFFECIENT_FUND:
		printf("\nDeclined\nInsuffecient funds\n");
		break;
		
		case DECLINED_STOLEN_CARD:
		printf("\nDeclined\nStolen Card!\n");
		break;
		
		case INTERNAL_SERVER_ERROR:
		printf("\nDeclined\nServer error\n");
		break;
		
		case APPROVED:
		printf("\nApproved\nBalance Updated");
		break;
	}
	
	free(transaction);
	clearTransDB();
}