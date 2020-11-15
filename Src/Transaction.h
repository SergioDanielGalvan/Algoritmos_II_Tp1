/*
 * Transaction.h
 *
 *  Created on: 25 oct. 2020
 *      Author: Ramiro
 */

#ifndef TRANSACTION_H_
#define TRANSACTION_H_

#include "TransactionInput.h"
#include "TransactionOutput.h"
#include "BlockChainDataTypes.h"
#include "lista.h"

enume class statusTransaction { stOk, stSinChequear, stSF };

class Transaction {
private:
	int n_tx_in; 								 // Indica cantidad total de inputs
	lista <TransactionInput *> ListaTranIn;      // Lista de inputs
	int n_tx_out; 								 // Indica cantidad total de outputs
	lista <TransactionOutput *> ListaTranOut;    // Lista de outputs
	statusTransaction Estatus;
	std::string HashMerkle;
public:
	//---Constructores---//
	Transaction();
	Transaction(int n_tx_in,int n_tx_out);
	Transaction( const raw_t & raw);
	~Transaction();
	//---Getters---//
	int getNumTransactionIn();
	int getNumTransactionOut();
	TransactionInput * getTransactionInput(int index);
	TransactionOutput * getTransactionOutput(int index);
	//---Setters---//
	//---Otros---//
	std::string getConcatenatedTransactions();
	std::string getHashMerkle();
};

#endif /* TRANSACTION_H_ */